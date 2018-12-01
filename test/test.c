// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include"sjh2.h" 
#include"stdio.h"
#include"stdlib.h"
#include"string.h"

/**********************************************

实验2：	1.读入输入文法
2.根据文法求FIRST集和FOLLOW集
3.构造预测分析表
4.读入输入串
5.根据以上分析，显示过程及结果（有各种报错）

**********************************************/

grammarElement gramOldSet[200];//原始文法的产生式集 

char terSymbol[200];     //终结符号 
char non_ter[200];       //非终结符
char allSymbol[400];     //所有符号
char firstSET[100][100]; //各产生式的右部FIRST集
char followSET[100][100];//各产生式的右部FOLLOW集 

int M[200][200];         //分析表 

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

int information(FILE*fp);//初始化代码并入栈。
int process(Stack *s);//分隔单词 
int add_string(Stack *s, char start);//拼接连续的字符
int add_num(Stack *s, char start);//拼接连续的字符
int add_delimiters(Stack *s, char start);
int delimiters_search(char *string);
//关键字初值定义(32个) 
char *rwtab[33] = { "auto"," short","int","long","float","double","char","struct",
"union","enum","typedef","const","unsigned","signed","extern",
"register","static","volatile","void","if","else","switch","case",
"for","do","while","goto","continue","break","default","sizeof","return" };
//界符(37个)
char *delimiters[50] = { "*","/",":",":=","<","<>","<=",">",">=","=",";","(",")","#","-","+","++","--",
"[","]","{","}","&","\"","'","==","%",".","||","&&","->","\,","!=","+=","-=","*","\\" };

//NUM和ID
char *ni[3] = { "1","a" };
//引号里的内容

int main()
{
	int fact, i;
	errno_t err;
	Stack s;
	stack_init(&s);
	FILE*fp;
	err = fopen_s(&fp,"input.txt","r");

	//1.首先确认可以打开input.txt;
	if (fp == NULL) {
		printf("无法打开!\n");
		return 0;
	}
	else printf("成功打开!\n");

	//2.扫描输入文法 
	fact = information(fp);//读取文件信息，并用字符数组对文件信息进行储存。
	system("pause");
	return 0;
}

int information(FILE*fp)//初始化代码并入栈。 
{
	//printf("hou s = %d\n",s);
	int flag = 0;
	int i = 0, j = 0, fact = 0, t = 0, m = 0;
	int g_num = 0,t_num = 0,nt_num = 0;//3种要存入的个数
	char *temp = 0;
	char over;
	temp = malloc(10000);
	fseek(fp, 0, SEEK_SET);//定位 
	fread(temp, sizeof(char), 10000, fp);
	//处理产生式
	for (i = 0; i<1000; i++){
		//跳出 
		if (temp[i] == '10'){
			break;
		}
		//未结束存入grammarElement gramOldSet[200];char terSymbol[200];char non_ter[200];
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

