#include "simple_tree.h"

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
//#include <cstddef>

Queue* create_queue() {
    Queue *q = (Queue*)malloc(sizeof(Queue));
    if(q)q->front = q->rear = NULL;
    return q;
}

bool is_empty(Queue *q) {
    return q == NULL || q->front == NULL;
}
void enqueue(Queue *q, TreeNode *tree_node) {
    if(!q || !tree_node) return;
    QueueNode* new_node = malloc(sizeof(QueueNode));
    new_node ->tree_node = tree_node;
    new_node->next= NULL;
    if(is_empty(q)){
        q->front = new_node;
        q->rear = new_node;
    }else{
        q->rear->next = new_node;
        q->rear = new_node;
    }
}

TreeNode* dequeue(Queue *q) {
    if(is_empty(q)) return NULL;
    QueueNode* temp = q->front;
    TreeNode* out_tree = temp->tree_node;
    q->front = q->front->next;
    if(q->front == NULL){
        q->rear = NULL;
    }
    temp->next =NULL;
    free(temp);
    return out_tree;
}



void free_queue(Queue *q) {
    while (!is_empty(q)) {
        dequeue(q);
    }
    free(q);
}





TreeNode* build_tree_by_level(int *level_order, int size) {
    if(!level_order)return NULL;
    Queue* q = create_queue();
    TreeNode* root = malloc(sizeof(TreeNode));
    root->left = NULL;
    root->right = NULL;
    root->val = level_order[0];
    enqueue(q, root);
    int i = 1;
    while(i<size&&!is_empty(q)){
        TreeNode* current = dequeue(q);
        if(i < size && level_order[i] != INT_MIN){
            TreeNode* left_node = malloc(sizeof(TreeNode));
            left_node->val = level_order[i];
            left_node->left = NULL;
            left_node->right = NULL;
            current->left = left_node;
            enqueue(q, left_node);
        }
        ++i;
        if(i < size && level_order[i] != INT_MIN){
            TreeNode* right_node = malloc(sizeof(TreeNode));
            right_node->val = level_order[i];
            right_node->right = NULL;
            right_node->left = NULL;
            current->right = right_node;
            enqueue(q, right_node);
        }
        ++i;
    }
    free_queue(q);
    return root;
}

void preorder_traversal(TreeNode *root) {
    if(!root) return;
    printf("%d ",root->val);
    preorder_traversal(root->left);
    preorder_traversal(root->right);
}

typedef struct StackNode{
    TreeNode* Node;
    struct StackNode* next;
}StackNode;

void push(StackNode** top,TreeNode* node){
    StackNode* new_node = malloc(sizeof(StackNode));
    new_node->Node = node;
    new_node->next = *top;
    *top = new_node;
}
TreeNode* pop(StackNode** top){
    if(!top||!*top)return NULL;
    StackNode* temp  = *top;
    TreeNode* return_node = (*top)->Node;
    *top = (*top)->next;
    free(temp);
    return return_node;
}

void preorder_traversal_iterative(TreeNode *root) {
    if(!root) return;
    StackNode* tail = NULL;
    push(&tail,root);
    while(tail!= NULL){
        TreeNode* current = pop(&tail);
        printf("%d ",current->val);
        if(current->right)push(&tail,current->right);
        if(current->left)push(&tail,current->left);
    }
}

void free_tree(TreeNode *root) {
    if (root == NULL) {
        return;
    }
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}
