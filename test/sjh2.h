#include"stdio.h"
#include"malloc.h"
/*****************定义**********************/

typedef struct
{
	char formula[200];//产生式 
}grammarElement; 

typedef struct Node
{
	struct Node *next;
	struct Node *pre;
	int data;
}Node;

typedef struct List
{
	struct Node *head;
	struct Node *tail;
	int count;
}List;

typedef List Stack;
typedef List Queue;


/****************表操作*********************/ 

void list_init(struct List *root)//初始化
{
	//printf("%d\n",root);
	root->count = 0;
	root->head = NULL;
	root->tail = NULL;
}

void push_back(Node *node, List *list)//出入到队列尾部 
{
	if(list->head==NULL)//如果头结点为空 
	{
		list->tail = node;
		list->head = list->tail;
		node->pre = NULL;
		node->next = node->pre;
		list->count++;
		return;
	}
	node->next = NULL;
	node->pre = list->tail;
	list->tail->next = node;
	list->tail = node;
	list->count++;
}

void push_front(int a, List *list)//插入到队列头部 
{
	Node *node;
	node = malloc(10);
	node->data = a;
	if(list->head == NULL)
	{
		list->tail = node;
		list->head = list->tail;
		node->pre = NULL;
		node->next = node->pre;
		list->count++;
		return;
	} 
	node->pre = NULL;
	node->next = list->head;
	list->head->pre = node;
	list->head = node;
	list->count++;
	//printf("list->head=%d\n",list->head);
	//printf("list->tail=%d\n",list->tail);
} 

int insert(Node *node, List *list, int locate)//插入到位置locate 
{
	//异常处理
	if(locate>list->count||locate<0)
	{
		return -1;
	} 
	
	if(locate==0)
	{
		push_front(node,list);
		return 1;
	}
	
	if(locate==list->count)
	{
		push_back(node,list);
		return 1;
	}
	
	//其他情况
	int curValue = -1;
	Node *cur = list->head;
	while(++curValue<locate)
	{
		cur = cur->next;
	} 
	node->next = cur;
	node->pre = cur->pre;
	cur->pre->next = node;
	cur->pre = node;
	list->count++;
	return 1;
}

Node* search(Node *node, List *list, int *locate)//返回相应的头结点，并记录到locate中 
{
	//*locate=0;
	int t=0;
	Node *cur = list->head;
	while(cur!=NULL&&cur->data!=node->data)
	{
		cur = cur->next;
		t++;
	} 
	if(cur==NULL)
	{
		t = -1;
	}
	*locate = t;
	return cur;
}

int rremove(List *list,int locate)//删除
{
	if(locate>=list->count||locate<0)
	{
		return -1;
	}
	
	if(locate==0)//头部 
	{
		Node *p = list->head->next;
		//free(list->head);
		p->pre = NULL;
		list->head = p;
	}
	else if(locate==(list->count-1))//尾部 
	     {
	     	Node *p = list->tail->pre;
			//free(list->tail);
			p->next = NULL;
			list->tail = p; 
		 }
		 else//其他 
		 {
		 	//查找并保存到cur中
			int curValue = -1;
			Node *cur = list->head;
			while(++curValue<locate)
			{
				cur = cur->next;
			} 
			cur->pre->next = cur->next;
			cur->next->pre = cur->pre;
			//free(cur);
		 }
	list->count--;
	return 1;
} 


/***************堆栈操作********************/ 

void stack_init(Stack *s)//初始化 
{
	list_init((List*)s);
}

void push(int a, Stack *s)//压入栈 
{
	push_front(a,(List*)s); 
} 

Node* pop(Stack *s)//出栈
{
	//printf("pop s->head = %d\n",s->head);
	//printf("pop s->head->next = %d\n",s->head->next);
	Node *node = s->head;
	if(s->head!=NULL&&s->head->next!=NULL)
	{
		//Node *node = s->head;
		s->head = s->head->next;
		s->count--;
	}
	else if(s->head!=NULL)
	     {
	     	//Node *node = s->head;
		    s->count = 0;
		    s->head = NULL;
		    s->tail = NULL;
	     }
	     else
	     {
	     	printf("**pop前已经为空**\n");
	     	return NULL;
		 }
	
	return node;
}

Node* get_top_stack(Stack *s)//获取栈顶 
{
	return s->head; 
}

 
/***************队列操作********************/

void queue_init(Queue *q)//初始化 
{
	list_init((List*)q);
} 

void in_queue(struct Node *node, Queue *q)//入队 
{
	push_back(node,(List*)q);
}

Node* out_queue(Queue *q)//出队
{
	Node *node = q->head;
	
	if(q->head!=NULL&&q->head->next!=NULL)
	{
		//Node *node = s->head;
		q->head = q->head->next;
		q->count--;
	}
	else if(q->head!=NULL)
	     {
	     	//Node *node = s->head;
		    q->count = 0;
		    q->head = NULL;
		    q->tail = NULL;
	     }
	     else
	     {
	     	printf("**out前已经为空**\n");
	     	return NULL;
		 }
	
	return node;
} 

Node* get_first_queue(Queue *q)//获取队列头 
{
	return q->head;
}

 
