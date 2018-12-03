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

int g_num = 0, t_num = 0, nt_num = 0;//3种要存入的个数,最后一个内容有重复

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
int pro_first(int i);//求first集 
int pro_follow();//求follow集
int cap(int t,int j);//求右部符号是否可以推出空字

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
	if (fp == NULL) 
	{
		printf("无法打开!\n");
		return 0;
	}
	else printf("成功打开!\n");

	//2.扫描输入文法 
	fact = information(fp);//读取文件信息，并用字符数组对文件信息进行储存。
	system("pause");
	return 0;
}

int information(FILE*fp)//初始化。 
{
	//printf("hou s = %d\n",s);
	int flag = 0;
	int i = 0, j = 0, fact = 0, t = 0, m = 0, n = 0;//m为非终结符循环控制参数
	
	char *temp = 0;
	char over;
	temp = malloc(10000);
	fseek(fp, 0, SEEK_SET);//定位 
	fread(temp, sizeof(char), 10000, fp);
	//1.处理产生式
	for (i = 0; i<1000; i++)
	{
		//跳出 
		if (temp[i-1] == 10)
		{
			break;
		}
		//未结束存入grammarElement gramOldSet[200];char terSymbol[200];char non_ter[200];
		else
		{
			//printf("**");
			for (t = 0; t < 10; t++) 
			{
				if (temp[i] == 32|| temp[i] == 10)
				{
					gramOldSet[g_num].formula[20] = t;//保存产生式长度在20的位置
					g_num++;
				
					break;
				}
				//转换
				if (temp[i+1] == '\'')
				{
					//带'的大写换小写，用e代表E'
					gramOldSet[g_num].formula[t] = temp[i];
					gramOldSet[g_num].formula[t] = gramOldSet[g_num].formula[t] + 32;
					i++;
				}
				else
				{
					//替换 易普逊 为 @
					if (temp[i] == -90)
					{
						gramOldSet[g_num].formula[t] = 64;
						i++;
					}
					else
					{
						//替换 箭头 为 > 
						if (temp[i] == -95)
						{
							gramOldSet[g_num].formula[t] = '>';
							i++;
						}
						else
						{
							//处理'|'分裂为2个产生式
							if (temp[i] == '|')
							{
								//保存头
								gramOldSet[g_num].formula[20] = t;
								//printf("**\n");
								//printf("**");
								gramOldSet[g_num + 1].formula[0] = gramOldSet[g_num].formula[0];
								gramOldSet[g_num + 1].formula[1] = gramOldSet[g_num].formula[1];
								//printf("%c", gramOldSet[g_num + 1].formula[0]);
								//printf("%c", gramOldSet[g_num + 1].formula[1]);
								i++;
								t = 2;
								g_num++;
							}
							if (temp[i] == -90)
							{
								gramOldSet[g_num].formula[t] = 64;
								i++;
							}
							else
							{
								gramOldSet[g_num].formula[t] = temp[i];
							}
							
						}
						
					}
					
				}
				//printf("%c", gramOldSet[g_num].formula[t]);
				i++;

			}
			//printf("**\n");
		}
		
			//未结束存入grammarElement gramOldSet[200];char terSymbol[200];char non_ter[200];
		
	}

	//2.处理非终结符
	for (t = 0; t < g_num; t++)
	{
		
		//查重
		flag = 0;
		for (m = 0; m < nt_num; m++)
		{
			if (gramOldSet[t].formula[0] == non_ter[m])
			{
				flag = 1;
				break;
			}
		}
		if (flag == 1)
		{
			;
		}
		else
		{
			non_ter[nt_num] = gramOldSet[t].formula[0];
			nt_num++;
			//printf("++");
			//printf("%c", non_ter[nt_num-1]);
			//printf("++\n");
		}
		

	}

	for (t = 0; t < g_num; t++)
	{
		printf("***");
		for (m = 0; m < gramOldSet[t].formula[20]; m++)
		{
			printf("%c", gramOldSet[t].formula[m]);
		}
		printf("***\n");
	}

	for (t = 0; t < nt_num; t++)
	{
		printf("+++");
	    printf("%c", non_ter[t]);
		printf("+++\n");
	}

	//3.处理终结符
	t_num = 0;
	for (t = 0; t < g_num; t++)
	{
		//取箭头后面的字符
		for (i = 0; i < gramOldSet[t].formula[20]; i++)
		{
			if (gramOldSet[t].formula[i] == '>')
			{
				i=i+1;
				//printf("hhah");
				break;
			}
		}
		//得到非终结符
		for (; i < gramOldSet[t].formula[20]; i++)
		{
			//若是终结符
			flag = 0;
			for (j = 0; j < nt_num; j++)
			{
				if (gramOldSet[t].formula[i] == non_ter[j])
				{
					flag = 1;
					
					break;
				}
			}
			if (flag == 1)
			{
				continue;
			}
			//若不是则存入
			//查重
			flag = 0;
			for (m = 0; m < t_num; m++)
			{
				if (gramOldSet[t].formula[i] == terSymbol[m])
				{
					flag = 1;
					//printf("HH\n");
					break;
				}
			}
			if (flag == 1)
			{
				;
			}
			else
			{
				terSymbol[t_num] = gramOldSet[t].formula[i];
				printf("&&");
				printf("%c", terSymbol[t_num]);
				printf("&&\n");
				t_num++;
			}
		}
	}
	terSymbol[t_num] = '#';
	t_num++;
	printf("&&");
	printf("%c", terSymbol[t_num-1]);
	printf("&&\n");

	printf("%d,%d,%d\n", g_num,nt_num,t_num);
	return fact;
}

int pro_first(int i)
{
	firstSET[i][21] = 0;//求之前长度为0
	firstSET[i][20] = 1;//在20位置记录是否已经求过first集
	//i外层循环，t产生式循环，j产生式从左到右循环，flag是否为终结符标志，m终结符循环,n为first集中第几个,x为非终结符序号
	int t=0,j=0,flag=0,m=0,n=0,x=0;
	//p控制加入,flag1控制产生式右部时候求完，flag2控制非终结符是否都可以推出易普逊
	int p = 0,flag1=0,flag2=0;
	//在所有产生式中查找non_ter[i]所在的产生式
	for (t = 0; t < g_num; t++)
	{
		//若找到
		j = 0;
		if (non_ter[i] == gramOldSet[t].formula[j])
		{
			//移动至箭头右方
			while (gramOldSet[t].formula[j] != '>')
			{
				j++;
			}
			j++;
			//如果产生式右部第一个字符为终结符或为空
			m = 0;
			flag = 0;
			while (m < t_num)
			{
				if (gramOldSet[t].formula[j] == terSymbol[m])
				{
					flag = 1;
					break;
				}
				m++;
			}
			//加入 终结符 或 易普逊
			if (flag == 1)
			{
				firstSET[i][n] = terSymbol[m];
				n++;
				firstSET[i][21]++;//在21的位置记录长度
			}
			else//否则为非终结符
			{
				flag1 = 0;
				flag2 = 0;
				while (flag1 != 1)//查找所有右端的非终结符的first集，直到终结符或者结束
				{
					//查找当前非终结符序号x
					for (x = 0; x < nt_num; x++)
					{
						if (gramOldSet[t].formula[j] == non_ter[x])
						{
							break;
						}
					}
					if (x == nt_num)//若循环到终结符，则跳出
					{
						break;
					}
					//若当前非终结符还没有求其first集
					if (firstSET[x][20] != 1)
					{
						//查找他的first集并标识此符号已求其first集
						pro_first(x);
						//将结果加入x的first集
						for (p = 0; p < firstSET[x][21]; p++)
						{
							if (firstSET[x][p] == '@')
							{
								flag2 = 1;//表示可以推到空集，需要求下一
								firstSET[i][n] = firstSET[x][p];
								n++;
								firstSET[i][21]++;
							}
							firstSET[i][n] = firstSET[x][p];
							n++;
							firstSET[i][21]++;
						}
					}
					else
					{
						for (p = 0; p < firstSET[x][21]; p++)
						{
							firstSET[i][n] = firstSET[x][p];
							n++;
							firstSET[i][21]++;
						}
					}

				}
			}
		}
	}
	i++;
	
}

int cap(int i, int j)
{

}

