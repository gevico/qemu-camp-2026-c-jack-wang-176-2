#include <stdio.h>
#include <stdbool.h>

#define MAX_PEOPLE 51
#define ONE_TIME 51

typedef struct {
    int id;
} People;

typedef struct {
    People data[MAX_PEOPLE];
    int head;
    int tail;
    int count;
} Queue;
void init_queue(Queue* q){
    q->head=0;
    q->tail=0;
}
bool isEmpty(Queue* q){
    return q->head==q->tail;
}
bool isFull(Queue* q){
    return (q->tail +1)%ONE_TIME==q->head;
}
bool push_queue(Queue*q ,People p){
    if(isFull(q)){
        printf("the queue is full");
        return false;
    }
    q->data[q->tail] = p;
    q->tail = (q->tail +1)%ONE_TIME;
    return true;
}
People pop_queue(Queue* q){
    if(isEmpty(q)){
        printf("the queue is empty");
        People temp = {-1};
        return temp;
    }
    People p = q->data[q->head];
    q->head = (q->head+1)%ONE_TIME;
    return p;
}

int main() {
    Queue q;
    int total_people=50;
    int report_interval=5;
    init_queue(&q);
    for(int i=1;i<=total_people;i++){
        People p = {i};
        push_queue(&q,p);
    }
    int remain_people = total_people;
    while(remain_people>1){
        for(int i=0;i<report_interval-1;i++){
            People safe = pop_queue(&q);
            push_queue(&q,safe);
        }
        People out = pop_queue(&q);
        printf("淘汰:%d\n",out.id);
        remain_people--;
    }
    People last = pop_queue(&q);
    printf("最后剩下的人是: %d\n", last.id);

    return 0;
}