#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int parse_url(const char* url) {
    if (url == NULL) {
        return -EINVAL; 
    }
    int err = 0;
    
    // ==========================================
    // 终极修复：使用标准 C 的 malloc + strcpy 替代 strdup
    // ==========================================
    size_t url_len = strlen(url);
    char* temp_url = (char*)malloc(url_len + 1); // +1 是为了存放字符串末尾的 '\0'
    
    if(temp_url == NULL){
        err = -EINVAL;
        goto exit;
    }
    // 将原字符串安全拷贝到我们分配的内存中
    strcpy(temp_url, url);
    // ==========================================

    char* query_start = strchr(temp_url, '?');
    if(query_start == NULL){
        goto exit;
    }
    query_start++;
    char* pair = strtok(query_start, "&");
    while(pair != NULL){
        char* equal = strchr(pair, '=');
        if(equal != NULL){
            *equal = '\0';
            char* key = pair;
            char* value = equal + 1;
            printf("key = %s, value = %s\n", key, value);
        }
        pair = strtok(NULL, "&");
    }
exit:
    if(temp_url) {
        free(temp_url);
    }
    return err;
}

int main() {
    const char* test_url = "https://cn.bing.com/search?name=John&age=30&city=New+York";

    printf("Parsing URL: %s\n", test_url);
    printf("Parameters:\n");

    parse_url(test_url);

    return 0;
}