#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENTS 100
#define NAME_LEN 50

typedef struct {
    char name[NAME_LEN];
    int score;
} Student;

Student students[MAX_STUDENTS];
Student temp[MAX_STUDENTS];

void merge_sort(int left, int right,Student* temp) {
    if(left<right){
        int mid = left + (right - left)/2;
        merge_sort(left,mid,temp);
        merge_sort(mid+1,right,temp);
        int i = left;
        int j = mid+1;
        int k = left;
        while(i<= mid&& j<= right){
          if(students[i].score>=students[j].score){
            temp[k]= students[i];
            i++;
           }else{
             temp[k] = students[j];
             j++;
            }
              k++;
        }
        while(i<=mid){
            temp[k]= students[i];
            i++;
            k++;
        }
        while(j<=right){
            temp[k]= students[j];
            j++;
            k++;
        }
        for(int p = 0;p<= right;p++){
            students[p] = temp[p];
        }

    }
}

int main(void) {
    FILE *file = fopen("02_students.txt", "r");
    if (!file) {
        printf("错误：无法打开文件 02_students.txt\n");
        return 1;
    }

    int n;
    fscanf(file, "%d", &n);

    if (n <= 0 || n > MAX_STUDENTS) {
        printf("学生人数无效：%d\n", n);
        fclose(file);
        return 1;
    }

    for (int i = 0; i < n; i++) {
        fscanf(file, "%s %d", students[i].name, &students[i].score);
    }
    fclose(file);

    Student* temp = malloc(sizeof(Student)*MAX_STUDENTS);
    merge_sort(0, n - 1,temp);

    printf("\n归并排序后按成绩从高到低排序的学生名单：\n");
    for (int i = 0; i < n; i++) {
        printf("%s %d\n", students[i].name, students[i].score);
    }

    return 0;
}