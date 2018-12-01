#include"stdio.h"
#include"malloc.h"
/*****************����**********************/

typedef struct
{
	char formula[200];//����ʽ 
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


/****************�����*********************/ 

void list_init(struct List *root)//��ʼ��
{
	//printf("%d\n",root);
	root->count = 0;
	root->head = NULL;
	root->tail = NULL;
}

void push_back(Node *node, List *list)//���뵽����β�� 
{
	if(list->head==NULL)//���ͷ���Ϊ�� 
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

void push_front(int a, List *list)//���뵽����ͷ�� 
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

int insert(Node *node, List *list, int locate)//���뵽λ��locate 
{
	//�쳣����
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
	
	//�������
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

Node* search(Node *node, List *list, int *locate)//������Ӧ��ͷ��㣬����¼��locate�� 
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

int rremove(List *list,int locate)//ɾ��
{
	if(locate>=list->count||locate<0)
	{
		return -1;
	}
	
	if(locate==0)//ͷ�� 
	{
		Node *p = list->head->next;
		//free(list->head);
		p->pre = NULL;
		list->head = p;
	}
	else if(locate==(list->count-1))//β�� 
	     {
	     	Node *p = list->tail->pre;
			//free(list->tail);
			p->next = NULL;
			list->tail = p; 
		 }
		 else//���� 
		 {
		 	//���Ҳ����浽cur��
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


/***************��ջ����********************/ 

void stack_init(Stack *s)//��ʼ�� 
{
	list_init((List*)s);
}

void push(int a, Stack *s)//ѹ��ջ 
{
	push_front(a,(List*)s); 
} 

Node* pop(Stack *s)//��ջ
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
	     	printf("**popǰ�Ѿ�Ϊ��**\n");
	     	return NULL;
		 }
	
	return node;
}

Node* get_top_stack(Stack *s)//��ȡջ�� 
{
	return s->head; 
}

 
/***************���в���********************/

void queue_init(Queue *q)//��ʼ�� 
{
	list_init((List*)q);
} 

void in_queue(struct Node *node, Queue *q)//��� 
{
	push_back(node,(List*)q);
}

Node* out_queue(Queue *q)//����
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
	     	printf("**outǰ�Ѿ�Ϊ��**\n");
	     	return NULL;
		 }
	
	return node;
} 

Node* get_first_queue(Queue *q)//��ȡ����ͷ 
{
	return q->head;
}

 
