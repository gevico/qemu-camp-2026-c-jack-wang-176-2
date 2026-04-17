#ifndef __MYFILE_H__
#define __MYFILE_H__

#include "../../include/common.h"

#include <elf.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
char* safe_safe_strdup(const char* s) {
    if (s == NULL) return NULL;
    size_t len = strlen(s);
    char* dup = (char*)malloc(len + 1);
    if (dup != NULL) {
        strcpy(dup, s);
    }
    return dup;
}

void print_elf_type(uint16_t e_type);

#endif //! __MYFILE_H__