#ifndef MYSED_H
#define MYSED_H

#include <stdio.h>

#include "../../include/common.h"

#define MAX_LINE_LENGTH 1024
char* safe_safe_strdup(const char* s) {
    if (s == NULL) return NULL;
    size_t len = strlen(s);
    char* dup = (char*)malloc(len + 1);
    if (dup != NULL) {
        strcpy(dup, s);
    }
    return dup;
}

int parse_replace_command(const char* cmd, char** old_str, char** new_str);
void replace_first_occurrence(char* str, const char* old, const char* new);

#endif //! MYSED_H