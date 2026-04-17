#include "mywc.h"
#include <string.h>

// 创建哈希表
WordCount **wc_create_hash_table() {
  WordCount **hash_table = calloc(HASH_SIZE, sizeof(WordCount *));
  return hash_table;
}

// 简单的哈希函数
unsigned int hash(const char *word) {
  unsigned long hash = 5381;
  int c;
  while ((c = *word++))
    hash = ((hash << 5) + hash) + c; // hash * 33 + c
  return hash % HASH_SIZE;
}

// 修改参数类型为 int，并在调用 ctype 函数时强制转换为 (unsigned char)
bool is_valid_word_char(int c) { 
    return isalpha((unsigned char)c) || c == '\''; 
}

char to_lower(int c) { 
    return tolower((unsigned char)c); 
}

// 添加单词到哈希表
void add_word(WordCount **hash_table, const char *word) {
  unsigned int index = hash(word);
  WordCount *entry = hash_table[index];
  while(entry){
    if(strcmp(entry->word,word)==0){
      entry->count++;
      return;
    }
    entry = entry->next;
  }
  WordCount *new_node = (WordCount*)malloc(sizeof(WordCount));
  if(!new_node)return;
  new_node->count = 1;
  strncpy(new_node->word,word,MAX_WORD_LEN-1);
  new_node->word[MAX_WORD_LEN-1]= '\0';
  new_node->next = hash_table[index];
  hash_table[index] = new_node;

}

// 打印单词统计结果
void print_word_counts(WordCount **hash_table) {
  printf("Word Count Statistics:\n");
  printf("======================\n");
  for(int i = 0;i<HASH_SIZE;i++){
    WordCount* cur = hash_table[i];
    while(cur != NULL){
      printf("%-20s %d\n", cur->word, cur->count);
      cur = cur ->next;
    }
  }
}

// 释放哈希表内存
void wc_free_hash_table(WordCount **hash_table) {
  for (int i = 0; i < HASH_SIZE; i++) {
    WordCount *entry = hash_table[i];
    while (entry != NULL) {
      WordCount *temp = entry;
      entry = entry->next;
      free(temp);
    }
  }
  free(hash_table);
}

// 处理文件并统计单词
void process_file(const char *filename) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    perror("Error opening file");
    return;
  }

  WordCount **hash_table = wc_create_hash_table();
  char word[MAX_WORD_LEN];
  int word_pos = 0;
  int c;

  while ((c = fgetc(file)) != EOF) {
    if (is_valid_word_char(c)) {
      if (word_pos < MAX_WORD_LEN - 1) {
        word[word_pos++] = to_lower(c);
      }
    } else {
      if (word_pos > 0) {
        word[word_pos] = '\0';
        add_word(hash_table, word);
        word_pos = 0;
      }
    }
  }

  // 处理文件末尾的最后一个单词
  if (word_pos > 0) {
    word[word_pos] = '\0';
    add_word(hash_table, word);
  }

  fclose(file);
  print_word_counts(hash_table);
  wc_free_hash_table(hash_table);
}

int __cmd_mywc(const char* filename) {
  process_file(filename);
  return 0;
}