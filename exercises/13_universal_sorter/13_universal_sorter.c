#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TYPE_INT 1
#define TYPE_FLOAT 2
#define TYPE_STRING 3


typedef int (*CompareFunc)(const void *, const void *);

int compareInt(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

int compareFloat(const void *a, const void *b) {
    float diff = (*(float*)a - *(float*)b);
    return (diff > 0) ? 1 : ((diff < 0) ? -1 : 0);
}

int compareString(const void *a, const void *b) {
    return strcmp(*(char**)a, *(char**)b);
}

void sort(void *array, size_t n, size_t size, CompareFunc compare) {
    qsort(array, n, size, compare);
}

void processFile(const char *filename) {
    FILE *fin = fopen(filename, "r");
    if (!fin) {
        printf("错误: 无法打开文件 %s\n", filename);
        return;
    }

    int choice, n;
    if (fscanf(fin, "%d", &choice) != 1 || fscanf(fin, "%d", &n) != 1) {
        printf("错误: 文件 %s 格式不正确\n", filename);
        fclose(fin);
        return;
    }

    if (n > 20) n = 20; 

    printf("=== 处理数据来自: %s ===\n", filename);

    switch (choice) {
        case TYPE_INT:{
            int contain[20];
            for(int i =0;i<n;i++){
                fscanf(fin,"%d",&contain[i]);
            }
            sort(contain,n,sizeof(int),compareInt);
            for(int i =0;i<n;i++){
                printf("%d ",contain[i]);
            }
            break;
        }
        case TYPE_FLOAT:{
            float contain[20];
            for(int i =0;i<n;i++){
                fscanf(fin,"%f",&contain[i]);
            }
            sort(contain,n,sizeof(float),compareFloat);
            for(int i =0;i<n;i++){
                printf("%f",contain[i]);
            }
            break;
        }
        case TYPE_STRING:{
            char contain[20][50];
            char* arr[20];
            for(int i =0;i<n;i++){
                fscanf(fin,"%49s",contain[i]);
                arr[i] = contain[i];
            }
            sort(arr,n,sizeof(char*),compareString);
            for(int i= 0;i<n;i++){
                printf("%19s",arr[i]);
            }
            break;
        }
    }

    fclose(fin);
}

int main() {
    processFile("int_sort.txt");
    processFile("float_sort.txt");

    return 0;
}