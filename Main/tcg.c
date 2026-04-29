#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

static void append_line(char *buffer, size_t *pos, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    *pos += vsnprintf(buffer + *pos, 20000 - *pos, fmt, args);
    va_end(args);
}

char *generateTargetCode(char *input) {
    char *copy = strdup(input);
    char *line = strtok(copy, "\n");
    char *result = malloc(20000);
    size_t pos = 0;
    int indent = 1;

    result[0] = '\0';

    while (line) {
        char name[64];
        char expr[512];

        if (sscanf(line, "DECLSET %63s = %511[^\n]", name, expr) == 2) {
            append_line(result, &pos, "%*sint %s = %s;\n", indent * 4, "", name, expr);
        } else if (sscanf(line, "DECL %63s", name) == 1) {
            append_line(result, &pos, "%*sint %s;\n", indent * 4, "", name);
        } else if (sscanf(line, "SET %63s = %511[^\n]", name, expr) == 2) {
            append_line(result, &pos, "%*s%s = %s;\n", indent * 4, "", name, expr);
        } else if (sscanf(line, "TMP %63s = %511[^\n]", name, expr) == 2) {
            append_line(result, &pos, "%*sint %s = %s;\n", indent * 4, "", name, expr);
        } else if (sscanf(line, "PRINT %511[^\n]", expr) == 1) {
            append_line(result, &pos, "%*sprintf(\"%%d\\n\", %s);\n", indent * 4, "", expr);
        } else if (sscanf(line, "INPUT %63s", name) == 1) {
            append_line(result, &pos, "%*sscanf(\"%%d\", &%s);\n", indent * 4, "", name);
        } else if (sscanf(line, "IF %511[^\n]", expr) == 1) {
            append_line(result, &pos, "%*sif (%s) {\n", indent * 4, "", expr);
            indent++;
        } else if (strcmp(line, "ELSE") == 0) {
            indent--;
            append_line(result, &pos, "%*s} else {\n", indent * 4, "");
            indent++;
        } else if (strcmp(line, "ENDIF") == 0) {
            indent--;
            append_line(result, &pos, "%*s}\n", indent * 4, "");
        } else if (sscanf(line, "WHILE %511[^\n]", expr) == 1) {
            append_line(result, &pos, "%*swhile (%s) {\n", indent * 4, "", expr);
            indent++;
        } else if (strcmp(line, "ENDWHILE") == 0) {
            indent--;
            append_line(result, &pos, "%*s}\n", indent * 4, "");
        } else if (sscanf(line, "EXPR %511[^\n]", expr) == 1) {
            append_line(result, &pos, "%*s%s;\n", indent * 4, "", expr);
        }

        line = strtok(NULL, "\n");
    }

    free(copy);
    return result;
}
