// mytrans.c
#include "myhash.h"
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void trim(char *str) {
    // 保持你原有的 trim 实现不变
    if(!str) return;
    char* end = str + strlen(str)-1;
    char* start = str;
    while(end>=str&&isspace((unsigned char)*end)){
      *end = '\0';
      end--;
    }
    while(start<end&&isspace((unsigned char)*start)){
      start ++;
    }
    if(start!= str){
      memmove(str,start,strlen(start)+1);
    }
}

// （可以完全废除原本基于 > 127 的 is_translation 判断）

int load_dictionary(const char *filename, HashTable *table,
                    uint64_t *dict_count) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    perror("无法打开词典文件");
    return -1;
  }

  char line[1024];
  char current_word[100] = {0};
  char current_translation[1024] = {0};
  
  while(fgets(line,sizeof(line),file)){
    trim(line);
    if(strlen(line) == 0) continue;
    
    // 严谨的协议解析逻辑：靠特定前缀识别身份，并剔除前缀
    if (line[0] == '#') {
      // 它是单词，截取 '#' 后面的干净字符串
      strncpy(current_word, line + 1, sizeof(current_word) - 1);
      current_word[sizeof(current_word) - 1]= '\0';
      to_lowercase(current_word);
      
    } else if (strncmp(line, "Trans:", 6) == 0) {
      // 它是翻译，截取 'Trans:' 后面的干净字符串
      strncpy(current_translation, line + 6, sizeof(current_translation) - 1);
      current_translation[sizeof(current_translation) - 1]= '\0';
      
      // 将绝对干净的键值对插入哈希表
      hash_table_insert(table, current_word, current_translation);
      if(dict_count) (*dict_count)++;
    }
  }

  fclose(file);
  return 0;
}