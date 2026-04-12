#include <stdio.h>
#include<stdbool.h>

#define MAX_ROW 5
#define MAX_COL 5
#define MAX_MATRIC (MAX_ROW * MAX_COL)

int maze[MAX_ROW][MAX_COL] = {
	0, 1, 0, 0, 0,
	0, 1, 0, 1, 0,
	0, 0, 0, 0, 0,
	0, 1, 1, 1, 0,
	0, 0, 0, 1, 0,
};

typedef struct 
{
	int r;
	int c;
}Point;

int dr[] = {0,1,-1,0};
int dc[] = {1,0,0,-1};

typedef struct 
{
	Point data[MAX_MATRIC];
	int top;
}Stack;

void init_stack(Stack *s){
	s->top = -1;
}

bool empty_stack(Stack *s){
	return s->top == -1;
}
bool push_stack(Stack *s,Point p){
	if(s->top >= MAX_MATRIC-1){
		return false;
	}
	s->top++;
	s->data[s->top] = p;
	return true;
}
Point pop_stack(Stack *s){
	if(s->top> MAX_MATRIC-1||empty_stack(s)){
		Point temp = {-1,-1};
		return temp;
	}
	Point p = s->data[s->top];
	s->top--;
	return p;
}
bool isvalid(int r,int c,bool visit[MAX_ROW][MAX_COL]){
	if(r<MAX_ROW&&c<MAX_COL&&!visit[r][c]&&maze[r][c]== 0&&r>=0&&c>=0){
		return true;
	}
	return false;
}
bool dfs(Point start, Point end){
    bool visit[MAX_ROW][MAX_COL] = {false};
    
    Point parent[MAX_ROW][MAX_COL]; 
    
    Stack s;
    init_stack(&s);
    push_stack(&s, start);
    visit[start.r][start.c] = true; 
    
    while(!empty_stack(&s)){
        Point cur = pop_stack(&s);
        
       
        if(cur.c == end.c && cur.r == end.r){
            Point p = end;
            while (!(p.r == start.r && p.c == start.c)) {
                printf("(%d, %d)\n", p.r, p.c); 
                p = parent[p.r][p.c]; 
            }
            printf("(%d, %d)\n", start.r, start.c); 
            return true;
        }
        
        for(int i = 0; i < 4; i++){
            int next_r = cur.r + dr[i];
            int next_c = cur.c + dc[i]; 
            
            if(isvalid(next_r, next_c, visit)){
                visit[next_r][next_c] = true; 
                
                Point next = {next_r, next_c};
                parent[next_r][next_c] = cur; 
                
                push_stack(&s, next);
            }
        }
    }
    return false;
}



int main(void)
{
	Point start = {0,0};
	Point end = {4,4};
	if(dfs(start,end)){
		printf("");
	}
	return 0;
}