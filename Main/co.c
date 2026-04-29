#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TEMPS 256

typedef struct {
    char name[32];
    char value[64];
    int known;
} TempValue;

static TempValue temps[MAX_TEMPS];
static int tempCount = 0;

static void trim_newline(char *line) {
    size_t len = strlen(line);
    if (len > 0 && line[len - 1] == '\n') {
        line[len - 1] = '\0';
    }
}

static int is_word_boundary(char c) {
    return c == '\0' || !(isalnum((unsigned char)c) || c == '_');
}

static const char *lookup_temp(const char *name) {
    for (int i = 0; i < tempCount; i++) {
        if (temps[i].known && strcmp(temps[i].name, name) == 0) {
            return temps[i].value;
        }
    }
    return NULL;
}

static void set_temp(const char *name, const char *value) {
    for (int i = 0; i < tempCount; i++) {
        if (strcmp(temps[i].name, name) == 0) {
            strcpy(temps[i].value, value);
            temps[i].known = 1;
            return;
        }
    }

    if (tempCount < MAX_TEMPS) {
        strcpy(temps[tempCount].name, name);
        strcpy(temps[tempCount].value, value);
        temps[tempCount].known = 1;
        tempCount++;
    }
}

static int is_integer_literal(const char *text) {
    while (*text == ' ' || *text == '\t' || *text == '(') {
        text++;
    }
    if (!text || !*text) {
        return 0;
    }
    if (*text == '-' || *text == '+') {
        text++;
    }
    if (!*text) {
        return 0;
    }
    while (*text) {
        if (*text == ')') {
            text++;
            continue;
        }
        if (!isdigit((unsigned char)*text)) {
            return 0;
        }
        text++;
    }
    return 1;
}

static void replace_known_temps(const char *input, char *output, size_t outputSize) {
    const char *start = input;
    size_t pos = 0;

    while (*input && pos + 1 < outputSize) {
        if (*input == 't' && isdigit((unsigned char)input[1]) &&
            (input == start || is_word_boundary(*(input - 1)))) {
            char tempName[32];
            int nameLen = 0;
            const char *value;

            while ((input[nameLen] == 't' || isdigit((unsigned char)input[nameLen])) &&
                   nameLen + 1 < (int)sizeof(tempName)) {
                tempName[nameLen] = input[nameLen];
                nameLen++;
            }
            tempName[nameLen] = '\0';

            if (is_word_boundary(input[nameLen]) && (value = lookup_temp(tempName)) != NULL) {
                int written = snprintf(output + pos, outputSize - pos, "%s", value);
                pos += (size_t)written;
                input += nameLen;
                continue;
            }
        }

        output[pos++] = *input++;
    }

    output[pos] = '\0';
}

static int fold_binary_expr(const char *expr, char *result, size_t resultSize) {
    int left;
    int right;
    char op1[3];
    char normalized[256];
    size_t j = 0;

    for (size_t i = 0; expr[i] != '\0' && j + 1 < sizeof(normalized); i++) {
        if (expr[i] != '(' && expr[i] != ')') {
            normalized[j++] = expr[i];
        }
    }
    normalized[j] = '\0';

    if (sscanf(normalized, "%d %2s %d", &left, op1, &right) != 3) {
        return 0;
    }

    if (strcmp(op1, "+") == 0) {
        snprintf(result, resultSize, "%d", left + right);
        return 1;
    }
    if (strcmp(op1, "-") == 0) {
        snprintf(result, resultSize, "%d", left - right);
        return 1;
    }
    if (strcmp(op1, "*") == 0) {
        snprintf(result, resultSize, "%d", left * right);
        return 1;
    }
    if (strcmp(op1, "/") == 0 && right != 0) {
        snprintf(result, resultSize, "%d", left / right);
        return 1;
    }
    if (strcmp(op1, "%") == 0 && right != 0) {
        snprintf(result, resultSize, "%d", left % right);
        return 1;
    }
    if (strcmp(op1, "<") == 0) {
        snprintf(result, resultSize, "%d", left < right);
        return 1;
    }
    if (strcmp(op1, ">") == 0) {
        snprintf(result, resultSize, "%d", left > right);
        return 1;
    }
    if (strcmp(op1, "==") == 0) {
        snprintf(result, resultSize, "%d", left == right);
        return 1;
    }
    if (strcmp(op1, "!=") == 0) {
        snprintf(result, resultSize, "%d", left != right);
        return 1;
    }

    return 0;
}

char *optimizeCode(char *input) {
    char *copy = strdup(input);
    char *line = strtok(copy, "\n");
    char buffer[20000] = "";
    size_t pos = 0;

    tempCount = 0;

    while (line) {
        char replaced[512];
        char folded[128];
        char name[32];
        char expr[256];

        trim_newline(line);
        replace_known_temps(line, replaced, sizeof(replaced));

        if (sscanf(replaced, "TMP %31s = %255[^\n]", name, expr) == 2) {
            if (is_integer_literal(expr) || fold_binary_expr(expr, folded, sizeof(folded))) {
                set_temp(name, is_integer_literal(expr) ? expr : folded);
                if (is_integer_literal(expr)) {
                    snprintf(replaced, sizeof(replaced), "TMP %s = %s", name, expr);
                } else {
                    snprintf(replaced, sizeof(replaced), "TMP %s = %s", name, folded);
                }
            }
        }

        pos += (size_t)snprintf(buffer + pos, sizeof(buffer) - pos, "%s\n", replaced);
        line = strtok(NULL, "\n");
    }

    free(copy);
    return strdup(buffer);
}
