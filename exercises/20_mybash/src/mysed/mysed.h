#ifndef MYSED_H
#define MYSED_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "common.h"

#define MAX_LINE_LENGTH 1024
// 确保这两行必须在文件顶部！


// 加上 static inline，让它成为内联安全函数
static inline char* safe_strdup(const char* s) {
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