// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include"sjh2.h" 
#include"stdio.h"
#include"stdlib.h"
#include"string.h"

/**********************************************

ʵ��2��	1.���������ķ�
2.�����ķ���FIRST����FOLLOW��
3.����Ԥ�������
4.�������봮
5.�������Ϸ�������ʾ���̼�������и��ֱ���

**********************************************/

grammarElement gramOldSet[200];//ԭʼ�ķ��Ĳ���ʽ�� 

char terSymbol[200];     //�ս���� 
char non_ter[200];       //���ս��
char allSymbol[400];     //���з���
char firstSET[100][100]; //������ʽ���Ҳ�FIRST��
char followSET[100][100];//������ʽ���Ҳ�FOLLOW�� 

int M[200][200];         //������ 

void pp(Stack *s, Stack *s1)
{
	while (s->count != 0)
	{
		Node *test_node;
		test_node = pop(s);
		push(test_node->data, s1);
		//printf("%d  %d\n",s->count,test_node->data);
	}
}

int information(FILE*fp);//��ʼ�����벢��ջ��
int process(Stack *s);//�ָ����� 
int add_string(Stack *s, char start);//ƴ���������ַ�
int add_num(Stack *s, char start);//ƴ���������ַ�
int add_delimiters(Stack *s, char start);
int delimiters_search(char *string);
//�ؼ��ֳ�ֵ����(32��) 
char *rwtab[33] = { "auto"," short","int","long","float","double","char","struct",
"union","enum","typedef","const","unsigned","signed","extern",
"register","static","volatile","void","if","else","switch","case",
"for","do","while","goto","continue","break","default","sizeof","return" };
//���(37��)
char *delimiters[50] = { "*","/",":",":=","<","<>","<=",">",">=","=",";","(",")","#","-","+","++","--",
"[","]","{","}","&","\"","'","==","%",".","||","&&","->","\,","!=","+=","-=","*","\\" };

//NUM��ID
char *ni[3] = { "1","a" };
//�����������

int main()
{
	int fact, i;
	errno_t err;
	Stack s;
	stack_init(&s);
	FILE*fp;
	err = fopen_s(&fp,"input.txt","r");

	//1.����ȷ�Ͽ��Դ�input.txt;
	if (fp == NULL) {
		printf("�޷���!\n");
		return 0;
	}
	else printf("�ɹ���!\n");

	//2.ɨ�������ķ� 
	fact = information(fp);//��ȡ�ļ���Ϣ�������ַ�������ļ���Ϣ���д��档
	system("pause");
	return 0;
}

int information(FILE*fp)//��ʼ�����벢��ջ�� 
{
	//printf("hou s = %d\n",s);
	int flag = 0;
	int i = 0, j = 0, fact = 0, t = 0, m = 0;
	int g_num = 0,t_num = 0,nt_num = 0;//3��Ҫ����ĸ���
	char *temp = 0;
	char over;
	temp = malloc(10000);
	fseek(fp, 0, SEEK_SET);//��λ 
	fread(temp, sizeof(char), 10000, fp);
	//�������ʽ
	for (i = 0; i<1000; i++){
		//���� 
		if (temp[i] == '10'){
			break;
		}
		//δ��������grammarElement gramOldSet[200];char terSymbol[200];char non_ter[200];
		else{
			for (t = 0; t < 10; t++) {



			}
			printf("%c==%d  ", temp[i], temp[i]);
			j++;

		}
	}
	fact = j;

	printf("%d\n", j);
	return fact;
}

