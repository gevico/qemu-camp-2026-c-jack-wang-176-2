#include "circular_linked_list.h"

#include <stdio.h>
#include <stdlib.h>


Node* create_circular_list(int n) {
    if(n<=0) return NULL;
    Node* head = (Node*)malloc(sizeof(Node));
    if(!head)return NULL;
    head->id = 1;
    head->next = head;
    Node* tail = head;
    for(int i = 2;i<=n;i++){
        Node* new_node = (Node*)malloc(sizeof(Node));
        if(!head)continue;
        new_node->id = i;
        new_node->next = head;
        tail->next = new_node;
        tail = new_node;
    }
    return head;
}

void free_list(Node* head) {
    if(!head)return;
    Node* current = head->next;
    while(current != head){
        Node* temp = current;
        current = current ->next;
        free(temp);
    }
    free(head);
}
