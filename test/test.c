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
int pro_first(int i);//��first�� 
int pro_follow();//��follow��
int cap(int t,int j);//���Ҳ������Ƿ�����Ƴ�����

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

int pro_first(int i)
{
	firstSET[i][21] = 0;//��֮ǰ����Ϊ0
	firstSET[i][20] = 1;//��20λ�ü�¼�Ƿ��Ѿ����first��
	//i���ѭ����t����ʽѭ����j����ʽ������ѭ����flag�Ƿ�Ϊ�ս����־��m�ս��ѭ��,nΪfirst���еڼ���,xΪ���ս�����
	int t=0,j=0,flag=0,m=0,n=0,x=0;
	//p���Ƽ���,flag1���Ʋ���ʽ�Ҳ�ʱ�����꣬flag2���Ʒ��ս���Ƿ񶼿����Ƴ�����ѷ
	int p = 0,flag1=0,flag2=0;
	//�����в���ʽ�в���non_ter[i]���ڵĲ���ʽ
	for (t = 0; t < g_num; t++)
	{
		//���ҵ�
		j = 0;
		if (non_ter[i] == gramOldSet[t].formula[j])
		{
			//�ƶ�����ͷ�ҷ�
			while (gramOldSet[t].formula[j] != '>')
			{
				j++;
			}
			j++;
			//�������ʽ�Ҳ���һ���ַ�Ϊ�ս����Ϊ��
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
			//���� �ս�� �� ����ѷ
			if (flag == 1)
			{
				firstSET[i][n] = terSymbol[m];
				n++;
				firstSET[i][21]++;//��21��λ�ü�¼����
			}
			else//����Ϊ���ս��
			{
				flag1 = 0;
				flag2 = 0;
				while (flag1 != 1)//���������Ҷ˵ķ��ս����first����ֱ���ս�����߽���
				{
					//���ҵ�ǰ���ս�����x
					for (x = 0; x < nt_num; x++)
					{
						if (gramOldSet[t].formula[j] == non_ter[x])
						{
							break;
						}
					}
					if (x == nt_num)//��ѭ�����ս����������
					{
						break;
					}
					//����ǰ���ս����û������first��
					if (firstSET[x][20] != 1)
					{
						//��������first������ʶ�˷���������first��
						pro_first(x);
						//���������x��first��
						for (p = 0; p < firstSET[x][21]; p++)
						{
							if (firstSET[x][p] == '@')
							{
								flag2 = 1;//��ʾ�����Ƶ��ռ�����Ҫ����һ
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

