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

int g_num = 0, t_num = 0, nt_num = 0;//3��Ҫ����ĸ���,���һ���������ظ�

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
int pro_first();//��first�� 
int pro_follow();//��follow��

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
	if (fp == NULL) 
	{
		printf("�޷���!\n");
		return 0;
	}
	else printf("�ɹ���!\n");

	//2.ɨ�������ķ� 
	fact = information(fp);//��ȡ�ļ���Ϣ�������ַ�������ļ���Ϣ���д��档
	pro_first();
	system("pause");
	return 0;
}

int information(FILE*fp)//��ʼ���� 
{
	//printf("hou s = %d\n",s);
	int flag = 0;
	int i = 0, j = 0, fact = 0, t = 0, m = 0, n = 0;//mΪ���ս��ѭ�����Ʋ���
	
	char *temp = 0;
	char over;
	temp = malloc(10000);
	fseek(fp, 0, SEEK_SET);//��λ 
	fread(temp, sizeof(char), 10000, fp);
	//1.�������ʽ
	for (i = 0; i<1000; i++)
	{
		//���� 
		if (temp[i-1] == 10)
		{
			break;
		}
		//δ��������grammarElement gramOldSet[200];char terSymbol[200];char non_ter[200];
		else
		{
			//printf("**");
			for (t = 0; t < 10; t++) 
			{
				if (temp[i] == 32|| temp[i] == 10)
				{
					gramOldSet[g_num].formula[20] = t;//�������ʽ������20��λ��
					g_num++;
				
					break;
				}
				//ת��
				if (temp[i+1] == '\'')
				{
					//��'�Ĵ�д��Сд����e����E'
					gramOldSet[g_num].formula[t] = temp[i];
					gramOldSet[g_num].formula[t] = gramOldSet[g_num].formula[t] + 32;
					i++;
				}
				else
				{
					//�滻 ����ѷ Ϊ @
					if (temp[i] == -90)
					{
						gramOldSet[g_num].formula[t] = 64;
						i++;
					}
					else
					{
						//�滻 ��ͷ Ϊ > 
						if (temp[i] == -95)
						{
							gramOldSet[g_num].formula[t] = '>';
							i++;
						}
						else
						{
							//����'|'����Ϊ2������ʽ
							if (temp[i] == '|')
							{
								//����ͷ
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
		
			//δ��������grammarElement gramOldSet[200];char terSymbol[200];char non_ter[200];
		
	}

	//2.������ս��
	for (t = 0; t < g_num; t++)
	{
		
		//����
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

	//3.�����ս��
	t_num = 0;
	for (t = 0; t < g_num; t++)
	{
		//ȡ��ͷ������ַ�
		for (i = 0; i < gramOldSet[t].formula[20]; i++)
		{
			if (gramOldSet[t].formula[i] == '>')
			{
				i=i+1;
				//printf("hhah");
				break;
			}
		}
		//�õ����ս��
		for (; i < gramOldSet[t].formula[20]; i++)
		{
			//�����ս��
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
			//�����������
			//����
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
	int gg_num;//�Ƶ�ʽ����
	int k = 0;//k��¼�Ƶ�ʽ�ӵĸ���
	//��ʼ��ÿһ�����ս��first���ĳ���
	for (i = 0; i < nt_num; i++)
	{
		firstSET[i][20] = 0;
	}
	//����ǰ��λ���浽100��ʼ��λ�ã�
	for (i = 0; i < g_num; i++)
	{
		k++;
		for (j = 0; j < 3; j++)
		{
			gramOldSet[i + 100].formula[j] = gramOldSet[i].formula[j];

		}
		//20��¼����ʽ����
		gramOldSet[i + 100].formula[20] = 3;
		//21��¼�Ƿ�Ϊ�ս����ͷ
		for (j = 0; j < t_num; j++)
		{
			if (gramOldSet[i + 100].formula[2] == terSymbol[j])
			{
				gramOldSet[i + 100].formula[21] = 1;
				//����first��
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
		//22��¼����ʽ��˵ķ���id
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
	//ѭ�����������Ƶ�ʽ
	for (i = 0; i < gg_num; i++)
	{
		flag = 0;
		//21��¼�Ƿ�Ϊ�ս����ͷ������������
		if (gramOldSet[i + 100].formula[21] == 1)
		{
			;
		}
		else
		{
			//��ѯ�����Ƶ�ʽ������
			for (j = 0; j < gg_num; j++)
			{
				//��ѯ�����ҿɼ��������
				if (gramOldSet[i + 100].formula[2] == gramOldSet[j + 100].formula[0]
					&&(gramOldSet[gg_num + 100].formula[0] != gramOldSet[i + 100].formula[0]
						&& gramOldSet[gg_num + 100].formula[2] != gramOldSet[j + 100].formula[2]))
				{
					gramOldSet[gg_num + 100].formula[0] = gramOldSet[i + 100].formula[0];
					gramOldSet[gg_num + 100].formula[1] = gramOldSet[i + 100].formula[1];
					gramOldSet[gg_num + 100].formula[2] = gramOldSet[j + 100].formula[2];
					gramOldSet[gg_num + 100].formula[21] = 0;
					//����21
					for (t = 0; t < t_num; t++)
					{
						if (gramOldSet[gg_num + 100].formula[2] == terSymbol[t])
						{
							gramOldSet[gg_num + 100].formula[21] = 1;
							break;
						}
					}
					//����22
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
			printf("���");
			break;
		}

	}
	printf("====%d %d %d====", flag, i,gg_num);
	//��ӡ
	for (i = 0; i < gg_num; i++)
	{
		for (j = 0; j < 3; j++) 
		{
			printf("%c", gramOldSet[i + 100].formula[j]);
		}
		printf("\n");
	
	}
	//����first��
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
	//��ӡ
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