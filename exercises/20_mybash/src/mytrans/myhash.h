// hash_table.h
#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "../../include/common.h"

#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define HASH_TABLE_SIZE 10000003  // 应选一个较大的质数，有利于分布
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
// 哈希节点
typedef struct HashNode {
    char *key;              // 英文单词，如 "a", "a.m."
    char *value;            // 中文翻译，如 "art. 一;字母A"
    struct HashNode *next;  // 用于解决冲突（链地址法）
} HashNode;

// 哈希表
typedef struct {
    HashNode *buckets[HASH_TABLE_SIZE];
} HashTable;

// 函数声明
HashTable* create_hash_table();
void free_hash_table(HashTable *table);
int hash_table_insert(HashTable *table, const char *key, const char *value);
const char* hash_table_lookup(HashTable *table, const char *key);

int load_dictionary(const char *filename, HashTable *table, uint64_t* dict_count);

#endif
