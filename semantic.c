/*
 * semantic.c  –  Semantic Analysis for VoiceLang
 *
 * Responsibilities
 * ────────────────
 *  1. Symbol-table management   (declare / lookup variables)
 *  2. Type checking             (operand compatibility, undeclared use)
 *  3. Initialisation checking   (use before init warning)
 *  4. Constant folding          (compile-time evaluation of literals)
 *  5. Annotated-AST construction & pretty printing
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "semantic.h"

/* ══════════════════════════════════════════════════════════
 *  Symbol Table (simple linked list)
 * ══════════════════════════════════════════════════════════ */

static Symbol *symTable = NULL;   /* head of the list */

void initSymbolTable(void) {
    symTable = NULL;
}

Symbol *lookupSymbol(const char *name) {
    for (Symbol *s = symTable; s; s = s->next)
        if (strcmp(s->name, name) == 0)
            return s;
    return NULL;
}

Symbol *insertSymbol(const char *name, DataType type) {
    Symbol *s = malloc(sizeof(Symbol));
    s->name          = strdup(name);
    s->type          = type;
    s->isInitialised = 0;
    s->next          = symTable;
    symTable         = s;
    return s;
}

/* ══════════════════════════════════════════════════════════
 *  Helper – allocate an AnnotatedNode
 * ══════════════════════════════════════════════════════════ */

static AnnotatedNode *newAnnotated(ASTNode *ast) {
    AnnotatedNode *an = calloc(1, sizeof(AnnotatedNode));
    an->astNode   = ast;
    an->dataType  = TYPE_UNKNOWN;
    an->hasConst  = 0;
    an->left = an->right = NULL;
    return an;
}

/* ══════════════════════════════════════════════════════════
 *  Utility
 * ══════════════════════════════════════════════════════════ */

const char *dataTypeName(DataType t) {
    switch (t) {
        case TYPE_INT:     return "int";
        case TYPE_FLOAT:   return "float";
        case TYPE_UNKNOWN: return "unknown";
        case TYPE_ERROR:   return "ERROR";
        default:           return "?";
    }
}

static int semanticErrors = 0;

static void semError(const char *msg) {
    fprintf(stderr, "[Semantic Error] %s\n", msg);
    semanticErrors++;
}

static void semWarning(const char *msg) {
    fprintf(stderr, "[Semantic Warning] %s\n", msg);
}

/* ══════════════════════════════════════════════════════════
 *  Core analyser  (recursive)
 * ══════════════════════════════════════════════════════════ */

AnnotatedNode *analyseAST(ASTNode *node) {
    if (!node) return NULL;

    AnnotatedNode *an = newAnnotated(node);
    char errBuf[256];

    switch (node->type) {

    /* ── NUMBER literal ────────────────────────────────── */
    case NODE_NUMBER:
        an->dataType    = TYPE_INT;
        an->hasConst    = 1;
        an->constValue  = node->value;
        break;

    /* ── IDENTIFIER (rvalue use) ───────────────────────── */
    case NODE_IDENTIFIER: {
        Symbol *sym = lookupSymbol(node->name);
        if (!sym) {
            snprintf(errBuf, sizeof errBuf,
                     "Undeclared variable '%s'", node->name);
            semError(errBuf);
            an->dataType = TYPE_ERROR;
        } else {
            an->dataType      = sym->type;
            an->isInitialised = sym->isInitialised;
            if (!sym->isInitialised) {
                snprintf(errBuf, sizeof errBuf,
                         "Variable '%s' used before initialisation", node->name);
                semWarning(errBuf);
            }
        }
        break;
    }

    /* ── BINARY OPERATION  (+  -  *  /) ───────────────── */
    case NODE_BINARY_OP: {
        AnnotatedNode *L = analyseAST(node->left);
        AnnotatedNode *R = analyseAST(node->right);
        an->left  = L;
        an->right = R;

        /* Type resolution */
        if (L->dataType == TYPE_ERROR || R->dataType == TYPE_ERROR) {
            an->dataType = TYPE_ERROR;
        } else if (L->dataType == TYPE_FLOAT || R->dataType == TYPE_FLOAT) {
            an->dataType = TYPE_FLOAT;   /* float promotion */
        } else {
            an->dataType = TYPE_INT;
        }

        /* Division by zero check */
        if (node->op == '/' && R->hasConst && R->constValue == 0) {
            semError("Division by zero detected");
            an->dataType = TYPE_ERROR;
        }

        /* Constant folding – only when both sides are known ints */
        if (an->dataType == TYPE_INT && L->hasConst && R->hasConst) {
            an->hasConst = 1;
            switch (node->op) {
                case '+': an->constValue = L->constValue + R->constValue; break;
                case '-': an->constValue = L->constValue - R->constValue; break;
                case '*': an->constValue = L->constValue * R->constValue; break;
                case '/': an->constValue = L->constValue / R->constValue; break;
            }
        }
        break;
    }

    /* ── DECLARATION  (DECLARE x AS INT VALUE expr) ────── */
    case NODE_DECLARATION: {
        /* Check for redeclaration */
        if (lookupSymbol(node->name)) {
            snprintf(errBuf, sizeof errBuf,
                     "Redeclaration of variable '%s'", node->name);
            semError(errBuf);
            an->dataType = TYPE_ERROR;
        } else {
            Symbol *sym = insertSymbol(node->name, TYPE_INT);

            /* Analyse the initialiser expression */
            AnnotatedNode *initExpr = analyseAST(node->left);
            an->left = initExpr;

            /* Type-compatibility check */
            if (initExpr && initExpr->dataType != TYPE_ERROR) {
                if (initExpr->dataType != TYPE_INT) {
                    snprintf(errBuf, sizeof errBuf,
                             "Type mismatch in declaration of '%s': "
                             "expected int, got %s",
                             node->name, dataTypeName(initExpr->dataType));
                    semError(errBuf);
                    an->dataType = TYPE_ERROR;
                } else {
                    an->dataType      = TYPE_INT;
                    sym->isInitialised = 1;
                    an->isInitialised = 1;

                    /* Propagate folded constant */
                    if (initExpr->hasConst) {
                        an->hasConst   = 1;
                        an->constValue = initExpr->constValue;
                    }
                }
            } else {
                an->dataType = TYPE_ERROR;
            }
        }
        break;
    }

    /* ── ASSIGNMENT  (SET x = expr) ────────────────────── */
    case NODE_ASSIGNMENT: {
        Symbol *sym = lookupSymbol(node->name);
        if (!sym) {
            snprintf(errBuf, sizeof errBuf,
                     "Assignment to undeclared variable '%s'", node->name);
            semError(errBuf);
            an->dataType = TYPE_ERROR;
        } else {
            AnnotatedNode *rhs = analyseAST(node->left);
            an->left = rhs;

            if (rhs && rhs->dataType != TYPE_ERROR) {
                if (rhs->dataType != sym->type) {
                    snprintf(errBuf, sizeof errBuf,
                             "Type mismatch in assignment to '%s': "
                             "expected %s, got %s",
                             node->name,
                             dataTypeName(sym->type),
                             dataTypeName(rhs->dataType));
                    semError(errBuf);
                    an->dataType = TYPE_ERROR;
                } else {
                    sym->isInitialised = 1;
                    an->dataType      = sym->type;
                    an->isInitialised = 1;
                    if (rhs->hasConst) {
                        an->hasConst   = 1;
                        an->constValue = rhs->constValue;
                    }
                }
            } else {
                an->dataType = TYPE_ERROR;
            }
        }
        break;
    }

    /* ── PRINT ─────────────────────────────────────────── */
    case NODE_PRINT: {
        Symbol *sym = lookupSymbol(node->name);
        if (!sym) {
            snprintf(errBuf, sizeof errBuf,
                     "PRINT of undeclared variable '%s'", node->name);
            semError(errBuf);
            an->dataType = TYPE_ERROR;
        } else {
            if (!sym->isInitialised) {
                snprintf(errBuf, sizeof errBuf,
                         "PRINT of uninitialised variable '%s'", node->name);
                semWarning(errBuf);
            }
            an->dataType      = sym->type;
            an->isInitialised = sym->isInitialised;
        }
        break;
    }

    default:
        an->dataType = TYPE_UNKNOWN;
        break;
    }

    return an;
}

/* ══════════════════════════════════════════════════════════
 *  Pretty-print the annotated AST
 * ══════════════════════════════════════════════════════════ */

static void indent(int level) {
    for (int i = 0; i < level; i++) printf("  ");
}

/*
 * Each node is printed with:
 *   NodeKind  [type=<type>] [value=<v>] [folded=<c>] [init=<yes/no>]
 */
void printAnnotatedAST(AnnotatedNode *an, int level) {
    if (!an) return;

    ASTNode *n = an->astNode;
    indent(level);

    switch (n->type) {

    case NODE_DECLARATION:
        printf("Declaration  name='%s'  [type=%s]  [init=%s]",
               n->name,
               dataTypeName(an->dataType),
               an->isInitialised ? "yes" : "no");
        if (an->hasConst) printf("  [folded=%d]", an->constValue);
        printf("\n");
        printAnnotatedAST(an->left, level + 1);
        break;

    case NODE_ASSIGNMENT:
        printf("Assignment   name='%s'  [type=%s]  [init=%s]",
               n->name,
               dataTypeName(an->dataType),
               an->isInitialised ? "yes" : "no");
        if (an->hasConst) printf("  [folded=%d]", an->constValue);
        printf("\n");
        printAnnotatedAST(an->left, level + 1);
        break;

    case NODE_PRINT:
        printf("Print        name='%s'  [type=%s]  [init=%s]\n",
               n->name,
               dataTypeName(an->dataType),
               an->isInitialised ? "yes" : "no");
        break;

    case NODE_NUMBER:
        printf("Number       value=%d   [type=%s]  [folded=%d]\n",
               n->value,
               dataTypeName(an->dataType),
               an->constValue);
        break;

    case NODE_IDENTIFIER:
        printf("Identifier   name='%s'  [type=%s]  [init=%s]\n",
               n->name,
               dataTypeName(an->dataType),
               an->isInitialised ? "yes" : "no");
        break;

    case NODE_BINARY_OP:
        printf("BinaryOp     op='%c'    [type=%s]",
               n->op,
               dataTypeName(an->dataType));
        if (an->hasConst) printf("  [folded=%d]", an->constValue);
        printf("\n");
        printAnnotatedAST(an->left,  level + 1);
        printAnnotatedAST(an->right, level + 1);
        break;

    default:
        printf("(unknown node)\n");
        break;
    }
}

/* ══════════════════════════════════════════════════════════
 *  Semantic-error count accessor
 * ══════════════════════════════════════════════════════════ */

int getSemanticErrorCount(void) {
    return semanticErrors;
}