#include <stdio.h>
#include<stdbool.h>

#define MAX_ROW 5
#define MAX_COL 5
#define MAX_MATIC (MAX_ROW * MAX_COL)

int maze[MAX_ROW][MAX_COL] = {
	0, 1, 0, 0, 0,
	0, 1, 0, 1, 0,
	0, 0, 0, 0, 0,
	0, 1, 1, 1, 0,
	0, 0, 0, 1, 0,
};
typedef struct {
	int r;
	int c;
}Point;

int dr[] = {0,1,-1,0};
int dc[] = {1,0,0,-1};

typedef struct{
	Point data[MAX_MATIC];
	int head;
	int rear;
}Queue;

void init_queue(Queue* q){
	q-> head = 0;
	q -> rear= 0;
}

bool push_queue(Queue* q, Point p){
	if(q->rear >= MAX_MATIC){
		return false;
	}
	q->data[q->rear]= p;
	q->rear++;
	return true;
}
bool isEmpty(Queue* q){
	return q->head==q->rear;
}
Point pop_queue(Queue* q){
	if(q->rear>= MAX_MATIC|| isEmpty(q)){
		Point temp = {-1,-1};
		return temp;
	}
	Point p = q->data[q->head];
	q->head++;
	return p;
}
bool isValid(int r,int c,bool visit[MAX_ROW][MAX_COL]){
	if(r>=0&&r<=MAX_ROW-1&&c>=0&&c<=MAX_COL-1&& !visit[r][c]&&maze[r][c]==0){
		return true;
	}
	return false;
}
void bfs(Point start,Point end){
	Queue q;
	init_queue(&q);
	bool visit[MAX_ROW][MAX_COL]= {false};
	push_queue(&q,start);
	visit[start.r][start.c] = true;
	Point parents[MAX_ROW][MAX_COL];
	while(!isEmpty(&q)){
		Point cur = pop_queue(&q);
		if(cur.r == end.r&&cur.c==end.c){
			Point p = end;
			while(!(start.c==p.c&&start.r==p.r)){
				printf("(%d, %d)\n",p.r,p.c);
				p =parents[p.r][p.c];
			}
			printf("(%d, %d)\n", start.r, start.c);
			return;
		}
		for(int i=0;i<4;i++){
			int tr = cur.r+dr[i];
			int tc = cur.c+dc[i];
			if(isValid(tr,tc,visit)){
		       Point newq = {tr,tc};
			   push_queue(&q,newq);
			   visit[tr][tc]= true;
			   parents[tr][tc]=cur;
			}
		}
	}

}

int main(void)
{
	Point start = {0,0};
	Point end = {4,4};
	bfs(start,end);
	return 0;
}