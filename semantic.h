#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "ast.h"

/* ─── Data type enum ─────────────────────────────────────── */
typedef enum {
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_UNKNOWN,
    TYPE_ERROR
} DataType;

/* ─── Annotated AST node ─────────────────────────────────── */
typedef struct AnnotatedNode {
    ASTNode          *astNode;       /* pointer to original AST node  */
    DataType          dataType;      /* inferred / declared type       */
    int               isInitialised; /* 1 = variable has been assigned */
    int               constValue;    /* folded value (if computable)   */
    int               hasConst;      /* 1 = constValue is valid        */

    struct AnnotatedNode *left;
    struct AnnotatedNode *right;
} AnnotatedNode;

/* ─── Symbol-table entry ─────────────────────────────────── */
typedef struct Symbol {
    char        *name;
    DataType     type;
    int          isInitialised;
    struct Symbol *next;
} Symbol;

/* ─── Public API ─────────────────────────────────────────── */
void           initSymbolTable(void);
Symbol        *lookupSymbol(const char *name);
Symbol        *insertSymbol(const char *name, DataType type);

AnnotatedNode *analyseAST(ASTNode *node);   /* main entry point */
void           printAnnotatedAST(AnnotatedNode *node, int level);

const char    *dataTypeName(DataType t);

#endif /* SEMANTIC_H */