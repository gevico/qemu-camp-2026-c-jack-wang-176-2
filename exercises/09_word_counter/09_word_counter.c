#include <stdio.h>
#include<stdbool.h>

int count_word(const char* str){
    int count =0;
    bool is_word = false;
    while(*str != '\0'){
        if(*str == '\n'||*str =='\t'||*str==' '){
            is_word = false;

        }else if(!is_word){
            is_word = true;
            count ++;
        }
        str++;
    }
    return count;
}


int main() {
    char str[]="Don't ask what your country can do for you, but ask what you can do for your country.";
    int wordCount = 0;
    
    wordCount = count_word(str);
    printf("单词数量: %d\n", wordCount);
    
    return 0;
}