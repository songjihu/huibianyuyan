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
char firstSET[200][100]; //������ʽ���Ҳ�FIRST��
char followSET[200][100];//������ʽ���Ҳ�FOLLOW�� 

int g_num = 0, t_num = 0, nt_num = 0;//3��Ҫ����ĸ���,���һ���������ظ�
int gg_num;//�Ƶ�ʽ����
int zh = 50;//��firstSET[50]��ʼ��¼
int zh1 = 100;//��firstSET[100]��ʼ��¼

int M[200][200];         //������ 


int information(FILE*fp);//��ʼ�����벢��ջ��
int pro_first();//��first�� 
int pro_first1(int a[10],int length);//����ս����ϵ�first��
int pro_first2(int a[10],int length);//���ϵ�first�����ս�������ַ�ascii�����ս������id��
int pro_follow();//��follow��
int isT(int x);//�Ƿ�Ϊ�ս��
int isN(int x);//�Ƿ�Ϊ���ս��
int build_f();//����������M[200][200]

int isT(int x)
{
	int i;
	for (i = 0; i < t_num; i++)
	{
		if (x == terSymbol[i])
		{
			return i;
		}
	}
	return -1;
}

int isN(int x)
{
	int i;
	for (i = 0; i < nt_num; i++)
	{
		if (x == non_ter[i])
		{
			return i;
		}
	}
	return -1;
}
//��ӡ
void pp()
{
	int o, p;
	for (o = 0; o < nt_num; o++)
	{
		printf("%c:", non_ter[o]);
		for (p = 0; p < followSET[o][20]; p++)
		{
			printf("%c", followSET[o][p]);
		}
		printf("   %d\n", followSET[o][20]);
	}
}


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
	pro_follow();
	build_f();
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
			//printf("���\n");
			break;
		}

	}
	//printf("====%d %d %d====\n", flag, i,gg_num);
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
				//printf("%d id=%d,%d\n",i, gramOldSet[i + 100].formula[22], firstSET[gramOldSet[i + 100].formula[22]][20]);
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

int pro_first1(int a[10],int length)
{
	//a[i]Ϊ���ս��id
	zh++;
	//����ʼֵ
	firstSET[zh][20] = 0;
	int i,j,t;
	int b[12];//��¼a[i]�Ƿ�ȡ�õ�����ѷ
	for (i = 0; i < 10; i++)
	{
		b[i] = 0;
	}
	for (i=0;i<length;i++)
	{
		//����a[i]��first��
		for (j = 0; j < firstSET[a[i]][20]; j++)
		{
			//��ѯ�Ƿ�����Ƴ�����ѷ
			
			if (firstSET[a[i]][j] != '@')
			{
				for (t = 0; t < firstSET[zh][20]; t++)
				{
					if (firstSET[a[i]][j] == firstSET[zh][t])
					{
						break;
					}
				}
				if (t == firstSET[zh][20])
				{
					firstSET[zh][firstSET[zh][20]] = firstSET[a[i]][j];
					firstSET[zh][20]++;
				}
			}
			//��������ѷ
			else
			{
				b[i] = 1;
				;
			}
			
		}
		//��ǰһ�����ս�������Ƴ�����ѷ���򲻱�������Ƶ�
		if (b[i] == 0)
		{
			i++;
			break;
		}
	}
	//printf("\n%d\n", i);
	//��ȫ�������Ƴ�����ѷ�����������ѷ
	
	if (b[i-1] == 1)
	{
		
		firstSET[zh][firstSET[zh][20]] = '@';
		firstSET[zh][20]++;
	}
	//��ӡ
	for (i = 0; i < length; i++)
	{
		//printf("%c", non_ter[a[i]]);
	}
	//printf(":");
	for (i = 0; i < firstSET[zh][20]; i++)
	{
		//printf("%c", firstSET[zh][i]);
	}
	//printf(" %d\n", firstSET[zh][20]);
	return zh;
}

int pro_follow()
{
	int i, j, t, m, n, o, p;
	int k;//��ϵı�־
	int flag = 0;//��־λ��ʾ�Ƿ��б仯
	int a[11];//�洢��ϣ���10��λ�ô泤��
	//��ʼ���洢�ռ�
	for (i = 0; i < 10; i++)
	{
		a[i] = 0;
	}
	a[10] = 0;
	//��ʼ������
	for (i = 0; i < nt_num; i++)
	{
		followSET[i][20] = 0;
	}
	followSET[0][0] = '#';
	followSET[0][20] = 1;
	//��������ʽ
	flag = 0;
	for (i = 0;  ; i++)
	{
		
		//��ʼ��a[]
		for (j = 0; j < 11; j++)
		{
			a[j] = 0;
		}
		//�����ұ����Ҳ�Ѱ����Ϊ��ͷ�ķ��ս��
		for (j = 2; j < gramOldSet[i].formula[20]; j++)
		{

			//��Ϊ���ս������������Ҳ��ҷ��ս��
			if (isN(gramOldSet[i].formula[j])!=-1)
			{
				//���ұ���������������ս����follow��
				for (t = j + 1; t < gramOldSet[i].formula[20]; t++)
				{
					//�����ս��������follow��
					if (isT(gramOldSet[i].formula[t])!=-1)
					{
						//�����һ�������ս��
						if (a[10] == 0)
						{
							for (n = 0; n < followSET[isN(gramOldSet[i].formula[j])][20]; n++)
							{
								if (followSET[isN(gramOldSet[i].formula[j])][n] == terSymbol[isT(gramOldSet[i].formula[t])]
									|| terSymbol[isT(gramOldSet[i].formula[t])] == '@')
								{
									n = 1000;
									break;
								}
							}
							if (n != 1000)
							{
								flag = 1;

								followSET[isN(gramOldSet[i].formula[j])][followSET[isN(gramOldSet[i].formula[j])][20]] = terSymbol[isT(gramOldSet[i].formula[t])];
								followSET[isN(gramOldSet[i].formula[j])][20]++;
								//��ӡ
								//pp();
							}

							//�Ҳ�������Ƶ��ս��֮��
							j = t + 1;

						}
						//������֮ǰ��ϵ�first��������
						else
						{
							k = pro_first1(a, a[10]);
							//����
							for (m = 0; m < firstSET[k][20]; m++)
							{
								for (n = 0; n < followSET[isN(gramOldSet[i].formula[j])][20]; n++)
								{
									if (followSET[isN(gramOldSet[i].formula[j])][n] == firstSET[k][m]
										|| firstSET[k][m] =='@')
									{
										n = 1000;
										break;
									}
								}
								if (n != 1000)
								{
									flag = 1;
									followSET[isN(gramOldSet[i].formula[j])][followSET[isN(gramOldSet[i].formula[j])][20]] = firstSET[k][m];
									followSET[isN(gramOldSet[i].formula[j])][20]++;
									//��ӡ
									//pp();
								}

							}
							//����Ͽ��������ѷ��������һ�ս��
							if (firstSET[k][m - 1] == '@')
							{
								for (n = 0; n < followSET[isN(gramOldSet[i].formula[j])][20]; n++)
								{
									
									if (followSET[isN(gramOldSet[i].formula[j])][n] == terSymbol[isT(gramOldSet[i].formula[t])]
										|| terSymbol[isT(gramOldSet[i].formula[t])] == '@')
									{
										n = 1000;
										break;
									}
								}
								if (n != 1000)
								{
									flag = 1;
									followSET[isN(gramOldSet[i].formula[j])][followSET[isN(gramOldSet[i].formula[j])][20]] = terSymbol[isT(gramOldSet[i].formula[t])];
									followSET[isN(gramOldSet[i].formula[j])][20]++;
									//��ӡ
									//pp();
								}
							}
							//���������a[]
							for (m = 0; m < 11; m++)
							{
								a[m] = 0;
							}
							//�������ұ���	
						}
						//����Ѱ����һ���ս����ͷ
						j = t;
						break;
					}
					//�����������a[]
					else
					{
						a[a[10]] = isN(gramOldSet[i].formula[t]);
						a[10]++;
						
					}
					//���Ѿ�ȫ������a[]��û�������ս��
					if (t == (gramOldSet[i].formula[20] - 1))
					{
						k = pro_first1(a, a[10]);
						//����
						for (m = 0; m < firstSET[k][20]; m++)
						{
							
							//�Ȳ���
							for (n = 0; n < followSET[isN(gramOldSet[i].formula[j])][20]; n++)
							{
								if (followSET[isN(gramOldSet[i].formula[j])][n] == firstSET[k][m]
									|| firstSET[k][m] == '@')
								{
									n = 1000;
									break;
								}
							}
							if (n != 1000)
							{
								flag = 1;
								followSET[isN(gramOldSet[i].formula[j])][followSET[isN(gramOldSet[i].formula[j])][20]] = firstSET[k][m];
								followSET[isN(gramOldSet[i].formula[j])][20]++;
								//��ӡ
								//pp();
							}

						}
						
						//���������a[]
						for (m = 0; m < 11; m++)
						{
							a[m] = 0;
						}
						//����Ѱ���²���ʽ
						j = t;
						break;
					}

				}

			}
			//�����������
		}
		//��ʼ��a[]
		for (j = 0; j < 11; j++)
		{
			a[j] = 0;
		}
		//���ҵ�������Ҳ���֤�Ƿ���Խ��󲿵�follow������follow����error��
		for (j = gramOldSet[i].formula[20] - 1; j > 1; j--)
		{
			//printf("_____i=%d,j=%d\n",i, j);
			//����Ƿ��ս���������Ƿ�����Ƴ�����ѷ
			if (isN(gramOldSet[i].formula[j])!=-1)
			{
				//printf("//%d:�Ƿ��ս��%c�������Ƿ�����Ƴ�����ѷ\n",i,non_ter[isN(gramOldSet[i].formula[j])]);
				//��ѯ�����Ƶ�
				for (t = 0; t < gg_num; t++)
				{
					if (gramOldSet[t + 100].formula[0] == non_ter[isN(gramOldSet[i].formula[j])] && gramOldSet[t + 100].formula[2] == '@')
					{
						t = 1000;
						break;
					}
				}
				//�������Ƴ�����ѷ������follow��������ֹ
				if (t != 1000)
				{
					//printf("//%d:���ս��%c���������Ƴ�����ѷ\n",i,non_ter[isN(gramOldSet[i].formula[j])]);
					for (m = 0; m < followSET[isN(gramOldSet[i].formula[0])][20]; m++)
					{
						//�Ȳ���
						for (n = 0; n < followSET[isN(gramOldSet[i].formula[j])][20]; n++)
						{
							if (followSET[isN(gramOldSet[i].formula[j])][n] == followSET[isN(gramOldSet[i].formula[0])][m]
								|| followSET[isN(gramOldSet[i].formula[0])][m] == '@')
							{
								n = 1000;
								break;
							}
						}
						if (n != 1000)
						{
							flag = 1;
							followSET[isN(gramOldSet[i].formula[j])][followSET[isN(gramOldSet[i].formula[j])][20]] = followSET[isN(gramOldSet[i].formula[0])][m];
							followSET[isN(gramOldSet[i].formula[j])][20]++;
							//��ӡ
							//pp();
						}

					}
					break;
					
				}
				//��������follow������������
				else
				{
					//printf("//%d:���ս��%c�������Ƴ�����ѷ\n",i, non_ter[isN(gramOldSet[i].formula[j])]);
					for (m = 0; m < followSET[isN(gramOldSet[i].formula[0])][20]; m++)
					{
						//�Ȳ���
						for (n = 0; n < followSET[isN(gramOldSet[i].formula[j])][20]; n++)
						{
							if (followSET[isN(gramOldSet[i].formula[j])][n] == followSET[isN(gramOldSet[i].formula[0])][m]
								|| followSET[isN(gramOldSet[i].formula[0])][m] == '@')
							{
								n = 1000;
								break;
							}
						}
						if (n != 1000)
						{
							flag = 1;
							followSET[isN(gramOldSet[i].formula[j])][followSET[isN(gramOldSet[i].formula[j])][20]] = followSET[isN(gramOldSet[i].formula[0])][m];
							followSET[isN(gramOldSet[i].formula[j])][20]++;
							//��ӡ
							//pp();
						}

					}
						
					
				}
			}
			//�����ս��������
			else
			{
				break;
			}

		}
		//��������
		if (i == (g_num - 1)&&flag==0)
		{
			break;
		}
		
		//��������������ظ�ѭ��
		if (i == (g_num - 1))
		{
			i = -1;
			flag = 0;
		}
		//("\n%d\n", i);
		//pp();
		
	}
	//��ӡ
	pp();


}

int pro_first2(int a[10], int length)
{
	//��ʼ���洢λ��
	zh1++;
	//����ʼֵ
	firstSET[zh1][20] = 0;
	int i,j;
	for (i = 0; i < length; i++)
	{
		//����⵽�ս��
		if (isT(a[i]) != -1)
		{
			//��֮ǰ�����Ƴ�����ѷ������ս��
			if (firstSET[zh1][20] > 0)
			{
				if (firstSET[zh1][firstSET[zh1][20] - 1] == '@')
				{
					
					//������first��
					for (j = 0; j < firstSET[zh1][20]; j++)
					{
						if (firstSET[zh1][j] == terSymbol[isT(a[i])])
						{
							j = 1000;
							break;
						}
					}

					if (j != 1000)
					{
						firstSET[zh1][firstSET[zh1][20]-1] = terSymbol[isT(a[i])];
						
					}
					//end
					return zh1;
				}
			}
			else
			{
				//����һ��Ϊ�ս��
				if (firstSET[zh1][20] == 0)
				{
					firstSET[zh1][0] = terSymbol[isT(a[i])];
					firstSET[zh1][20]++;
				}
				return zh1;
			}
			
		}
		else
		{
			//����first1������first��
			zh1 = pro_first1(a, (i + 1));
		}
	}
}

int build_f()
{
	int i, j, m;
	int x, y;
	int a[12], length, w=0;
	int fir;// first��returnֵ
	int fol;//follow��returnֵ
	/*      x   �ս��0    �ս��1   �ս��2   �ս��3  ... ����������ѷ��
	    y
	���ս��0   ����ʽ
	���ս��1   ����ʽ
	���ս��2   ����
	���ս��3   ����ʽ
	...
	*/
	//��ʼ��M[][]
	for (i = 0; i < 100; i++)
	{
		for (j = 0; j < 100; j++)
		{
			M[i][j] = -1;
		}
	}
	//��������
	for (i = 0; i < g_num; i++)
	{
		//ȷ��yλ��
		y = isN(gramOldSet[i].formula[0]);
		//���Ҷ˲�Ϊ����ѷ�����Ҷ�first��
		if (gramOldSet[i].formula[2] != '@')
		{
			//��ʼ��
			length = 0;
			fir = 0;
			for (j = 0; j < 11; j++)
			{
				a[10] = 0;
			}
			//�����Ҷ˵�a[]
			for (j = 2; j < gramOldSet[i].formula[20]; j++)
			{
				//�����ս��
				if (isT(gramOldSet[i].formula[j]) != -1)
				{
					a[j - 2] = gramOldSet[i].formula[j];
					length++;
				}
				//���Ƿ��ս��
				else
				{
					a[j - 2] = isN(gramOldSet[i].formula[j]);
					length++;
				}
			}
			//��first��
			fir = pro_first2(a, length);
			/*for (j = 0; j < firstSET[fir][20]; j++)
			{
				printf("%c", firstSET[fir][j]);
			}
			printf("   %d\n", firstSET[fir][20]);*/
			//���뵽M��
			for (j = 0; j < firstSET[fir][20]; j++)
			{
				//���x����
				x = isT(firstSET[fir][j]);
				//��x��yλ�ü������ʽid
				M[x][y] = i;
				/*printf("����[%d][%d]", x, y);
				for (m = 0; m < gramOldSet[i].formula[20]; m++)
				{
					printf("%c", gramOldSet[i].formula[m]);
				}
				printf("\n");*/
			}
		}
		//��Ϊ����ѷ�������follow��
		else
		{
			//���뵽M��
			for (j = 0; j < followSET[isN(gramOldSet[i].formula[0])][20]; j++)
			{
				//���x����
				x = isT(followSET[isN(gramOldSet[i].formula[0])][j]);
				//��x��yλ�ü������ʽid
				M[x][y] = i;
				/*printf("����[%d][%d]", x, y);
				for (m = 0; m < gramOldSet[i].formula[20]; m++)
				{
					printf("%c", gramOldSet[i].formula[m]);
				}
				printf("\n");*/
			}
		}
		//��ɺ����ѭ��
		
	}
	//��ӡ
	for (i = 0; i < t_num; i++)
	{ 
		printf("   %c   ", terSymbol[i]);
	}
	printf("\n");
	for (i = 0; i < nt_num; i++)
	{
		printf("%c  ", non_ter[i]);
		for (j = 0; j < t_num; j++)
		{
			if (M[j][i] != -1)
			{
				//printf("[%d][%d]", j, i);
				for (m = 0; m < gramOldSet[M[j][i]].formula[20]; m++)
				{
					printf("%c", gramOldSet[M[j][i]].formula[m]);
				}
			}
			else
			{
				printf("    ");
			}
			
				printf("   ");
			
			
		}
		printf("\n");
	}
}