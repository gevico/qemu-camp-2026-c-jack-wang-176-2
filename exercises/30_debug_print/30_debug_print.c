#include <stdio.h>
#include <stdlib.h>
#include <execinfo.h>

#ifdef DEBUG_LEVEL
    #if DEBUG_LEVEL == 1
        #define DEBUG_PRINT(fmt, ...) \
            printf("DEBUG: func=%s, line=%d\n", __func__, __LINE__)
    #elif DEBUG_LEVEL == 2
        #define DEBUG_PRINT(fmt, ...) \
            printf("DEBUG: func=%s, line=%d, " fmt "\n", __func__, __LINE__, ##__VA_ARGS__)
    #elif DEBUG_LEVEL == 3
        #define DEBUG_PRINT(fmt, ...) \
            do { \
                printf("DEBUG: func=%s, line=%d, " fmt "\n", __func__, __LINE__, ##__VA_ARGS__); \
                void *buffer[10]; \
                int nptrs = backtrace(buffer, 10); \
                char** strings = backtrace_symbols(buffer, nptrs); \
                if(strings) { \
                    for(int i = 0; i < nptrs; i++) { \
                        printf("  %s\n", strings[i]); \
                    } \
                } \
                free(strings); \
            } while(0)
    #endif
#else
    #define DEBUG_PRINT(fmt, ...) do {} while (0)
#endif
















void test() {
    int x = 42;
    DEBUG_PRINT("x=%d", x);
}

int main() {
    test();
    return 0;
}
