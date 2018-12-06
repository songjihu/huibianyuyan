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
int pro_first();//求first集 
int pro_follow();//求follow集

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
	pro_first();
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

int pro_first()
{
	int i, j, t;
	int gg_num;//推导式个数
	int k = 0;//k记录推导式子的个数
	//初始化每一个非终结符first集的长度
	for (i = 0; i < nt_num; i++)
	{
		firstSET[i][20] = 0;
	}
	//保存前三位（存到100开始的位置）
	for (i = 0; i < g_num; i++)
	{
		k++;
		for (j = 0; j < 3; j++)
		{
			gramOldSet[i + 100].formula[j] = gramOldSet[i].formula[j];

		}
		//20记录产生式长度
		gramOldSet[i + 100].formula[20] = 3;
		//21记录是否为终结符开头
		for (j = 0; j < t_num; j++)
		{
			if (gramOldSet[i + 100].formula[2] == terSymbol[j])
			{
				gramOldSet[i + 100].formula[21] = 1;
				//存入first集
				/*for (t = 0; t < nt_num; t++)
				{
					if (gramOldSet[i + 100].formula[0] == non_ter[t])
					{
						firstSET[t][firstSET[t][20]] = terSymbol[j];
						firstSET[t][20]++;
						break;
					}
				}*/
				break;
			}
			else
			{
				gramOldSet[i + 100].formula[21] = 0;
			}
		}
		//22记录产生式左端的符号id
		for (j = 0; j < nt_num; j++)
		{
			if (gramOldSet[i + 100].formula[0] == non_ter[j])
			{
				gramOldSet[i + 100].formula[22] = j;
				break;
			}
		}


	}
	gg_num = k;
	int flag;
	//循环产生所有推导式
	for (i = 0; i < gg_num; i++)
	{
		flag = 0;
		//21记录是否为终结符开头，若是则跳过
		if (gramOldSet[i + 100].formula[21] == 1)
		{
			;
		}
		else
		{
			//查询所有推导式并加入
			for (j = 0; j < gg_num; j++)
			{
				//查询到并且可加入则加入
				if (gramOldSet[i + 100].formula[2] == gramOldSet[j + 100].formula[0]
					&&(gramOldSet[gg_num + 100].formula[0] != gramOldSet[i + 100].formula[0]
						&& gramOldSet[gg_num + 100].formula[2] != gramOldSet[j + 100].formula[2]))
				{
					gramOldSet[gg_num + 100].formula[0] = gramOldSet[i + 100].formula[0];
					gramOldSet[gg_num + 100].formula[1] = gramOldSet[i + 100].formula[1];
					gramOldSet[gg_num + 100].formula[2] = gramOldSet[j + 100].formula[2];
					gramOldSet[gg_num + 100].formula[21] = 0;
					//计算21
					for (t = 0; t < t_num; t++)
					{
						if (gramOldSet[gg_num + 100].formula[2] == terSymbol[t])
						{
							gramOldSet[gg_num + 100].formula[21] = 1;
							break;
						}
					}
					//计算22
					for (t = 0; t < nt_num; t++)
					{
						if (gramOldSet[gg_num + 100].formula[0] == non_ter[t])
						{
							gramOldSet[gg_num + 100].formula[22] = t;
							break;
						}
					}
					gg_num++;
					flag = 1;
				}
			}

		}
		if (flag == 0 && i == gg_num - 1)
		{
			printf("完成");
			break;
		}

	}
	printf("====%d %d %d====", flag, i,gg_num);
	//打印
	for (i = 0; i < gg_num; i++)
	{
		for (j = 0; j < 3; j++) 
		{
			printf("%c", gramOldSet[i + 100].formula[j]);
		}
		printf("\n");
	
	}
	//加入first集
	for (i = 0; i < gg_num; i++)
	{
		for (j = 0; j < t_num; j++)
		{
			if (gramOldSet[i + 100].formula[2] == terSymbol[j])
			{
				printf("%d id=%d,%d\n",i, gramOldSet[i + 100].formula[22], firstSET[gramOldSet[i + 100].formula[22]][20]);
				firstSET[gramOldSet[i + 100].formula[22]][firstSET[gramOldSet[i + 100].formula[22]][20]]= terSymbol[j];
				firstSET[gramOldSet[i + 100].formula[22]][20]++;

				break;
			}
		}
	}
	//打印
	for (i = 0; i < nt_num; i++)
	{
		printf("%c:", non_ter[i]);
		for (j = 0; j < firstSET[i][20]; j++)
		{
			printf("%c",firstSET[i][j]);
		}
		printf("   %d\n", firstSET[i][20]);
	}
}