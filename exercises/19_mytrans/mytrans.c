// mytrans.c
#include "myhash.h"
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void trim(char *str) {
    if(!str) return;
    char* end = str + strlen(str)-1;
    char* start = str;
    while(end>str&&isspace(*end)){
      *end = '\0';
      end--;
    }
    while(start<end&&isspace(*start)){
      start ++;
    }
    if(start!= str){
      memmove(str,start,strlen(start)+1);
    }
}

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
  int in_entry = 0;

  while(fgets(line,sizeof(line),file)){
    trim(line);
    if(strlen(line) == 0)continue;
    if(in_entry ==0){
      strncpy(current_word,line,sizeof(current_word)-1);
      current_word[sizeof(current_word)-1]= '\0';
      to_lowercase(current_word);
      in_entry = 1;
    }else if(in_entry == 1){
      strncpy(current_translation,line,sizeof(current_translation)-1);
      current_translation[sizeof(current_translation)-1]= '\0';
      hash_table_insert(table,current_word, current_translation);
      if(dict_count)(*dict_count)++;
      in_entry = 0;
    }
  }

  fclose(file);
  return 0;
}
