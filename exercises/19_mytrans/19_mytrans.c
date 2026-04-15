// main.c
#include "myhash.h"
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>


void to_lowercase(char *str) {
  for (; *str; ++str)
    *str = tolower((unsigned char)*str);
}
const char *delimiters = " \t\r\n.,";
int main() {
  HashTable *table = create_hash_table();
  if (!table) {
    fprintf(stderr, "无法创建哈希表\n");
    return 1;
  }

  printf("=== 哈希表版英语翻译器（支持百万级数据）===\n");
  uint64_t dict_count = 0;
  if (load_dictionary("dict.txt", table, &dict_count) != 0) {
    fprintf(stderr, "加载词典失败，请确保 dict.txt 存在。\n");
    free_hash_table(table);
    return 1;
  }
  printf("词典加载完成，共计%lld词条。\n", dict_count);

  FILE* file = fopen("text.txt", "r");
  if (file == NULL) {
    fprintf(stderr, "无法打开文件 dict.txt。\n");
    free_hash_table(table);
    return 1;
  }

  char line[256];
  while (fgets(line, sizeof(line), file) != NULL) {
    line[strcspn(line, "\n")] = '\0';

    if (strlen(line) == 0) {
        continue;
    }
    char* word = strtok(line,delimiters);
    while(word != NULL){
      char copy[256];
      //copy[0] = '#';
      //strncpy(copy+1,word,sizeof(copy)-1);
      strncpy(copy,word,sizeof(copy));
      copy[sizeof(copy)-1] = '\0';
      to_lowercase(copy);
      const char *value =hash_table_lookup(table, copy);
      if (value != NULL) {
          // 🛡️ 防御 1：跳过翻译内容可能自带的行首空格，防止变成双空格
          const char *clean_val = value;
          while (*clean_val == ' ' || *clean_val == '\t') {
              clean_val++;
          }
          
          // 🛡️ 防御 2：砍掉可能残留在尾巴上的换行符/回车符 (\r \n)
          char safe_val[1024];
          strncpy(safe_val, clean_val, sizeof(safe_val) - 1);
          safe_val[sizeof(safe_val) - 1] = '\0';
          safe_val[strcspn(safe_val, "\r\n")] = '\0'; 
          
          // 打印干干净净的字符串
          printf("原文: %s\t翻译: %s\n", copy, safe_val);
      } else {
          // 没查到的情况，你原来写得已经非常完美了
          printf("原文: %s\t未找到该单词的翻译。\n", copy);
      }
      word = strtok(NULL,delimiters);
    }
  }

  free_hash_table(table);
  return 0;
}
