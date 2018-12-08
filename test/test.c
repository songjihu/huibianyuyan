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
char firstSET[200][100]; //各产生式的右部FIRST集
char followSET[200][100];//各产生式的右部FOLLOW集 

int g_num = 0, t_num = 0, nt_num = 0;//3种要存入的个数,最后一个内容有重复
int gg_num;//推导式个数
int zh = 50;//从firstSET[50]开始记录
int zh1 = 100;//从firstSET[100]开始记录

int M[200][200];         //分析表 


int information(FILE*fp);//初始化代码并入栈。
int pro_first();//求first集 
int pro_first1(int a[10],int length);//求非终结符组合的first集
int pro_first2(int a[10],int length);//求混合的first集（终结符输入字符ascii，非终结符输入id）
int pro_follow();//求follow集
int isT(int x);//是否为终结符
int isN(int x);//是否为非终结符
int build_f();//构建分析表M[200][200]

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
//打印
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
	pro_follow();
	build_f();
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
			//printf("完成\n");
			break;
		}

	}
	//printf("====%d %d %d====\n", flag, i,gg_num);
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
				//printf("%d id=%d,%d\n",i, gramOldSet[i + 100].formula[22], firstSET[gramOldSet[i + 100].formula[22]][20]);
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

int pro_first1(int a[10],int length)
{
	//a[i]为非终结符id
	zh++;
	//赋初始值
	firstSET[zh][20] = 0;
	int i,j,t;
	int b[12];//记录a[i]是否取得到易普逊
	for (i = 0; i < 10; i++)
	{
		b[i] = 0;
	}
	for (i=0;i<length;i++)
	{
		//加入a[i]的first集
		for (j = 0; j < firstSET[a[i]][20]; j++)
		{
			//查询是否可以推出易普逊
			
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
			//跳过易普逊
			else
			{
				b[i] = 1;
				;
			}
			
		}
		//若前一个非终结符不能推出易普逊，则不必再向后推导
		if (b[i] == 0)
		{
			i++;
			break;
		}
	}
	//printf("\n%d\n", i);
	//若全部可以推出易普逊，则加入易普逊
	
	if (b[i-1] == 1)
	{
		
		firstSET[zh][firstSET[zh][20]] = '@';
		firstSET[zh][20]++;
	}
	//打印
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
	int k;//组合的标志
	int flag = 0;//标志位表示是否有变化
	int a[11];//存储组合，在10的位置存长度
	//初始化存储空间
	for (i = 0; i < 10; i++)
	{
		a[i] = 0;
	}
	a[10] = 0;
	//初始化长度
	for (i = 0; i < nt_num; i++)
	{
		followSET[i][20] = 0;
	}
	followSET[0][0] = '#';
	followSET[0][20] = 1;
	//遍历产生式
	flag = 0;
	for (i = 0;  ; i++)
	{
		
		//初始化a[]
		for (j = 0; j < 11; j++)
		{
			a[j] = 0;
		}
		//从左到右遍历右部寻找作为开头的非终结符
		for (j = 2; j < gramOldSet[i].formula[20]; j++)
		{

			//若为非终结符，则继续向右查找非终结符
			if (isN(gramOldSet[i].formula[j])!=-1)
			{
				//向右遍历并计算这个非终结符的follow集
				for (t = j + 1; t < gramOldSet[i].formula[20]; t++)
				{
					//若是终结符，加入follow集
					if (isT(gramOldSet[i].formula[t])!=-1)
					{
						//如果第一个就是终结符
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
								//打印
								//pp();
							}

							//右部坐标后移到终结符之后
							j = t + 1;

						}
						//否则求之前组合的first集并加入
						else
						{
							k = pro_first1(a, a[10]);
							//加入
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
									//打印
									//pp();
								}

							}
							//若组合可求得易普逊，则加入后一终结符
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
									//打印
									//pp();
								}
							}
							//加入后清零a[]
							for (m = 0; m < 11; m++)
							{
								a[m] = 0;
							}
							//继续向右遍历	
						}
						//继续寻找下一非终结符开头
						j = t;
						break;
					}
					//若不是则加入a[]
					else
					{
						a[a[10]] = isN(gramOldSet[i].formula[t]);
						a[10]++;
						
					}
					//若已经全部加入a[]都没有遇到终结符
					if (t == (gramOldSet[i].formula[20] - 1))
					{
						k = pro_first1(a, a[10]);
						//加入
						for (m = 0; m < firstSET[k][20]; m++)
						{
							
							//先查重
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
								//打印
								//pp();
							}

						}
						
						//加入后清零a[]
						for (m = 0; m < 11; m++)
						{
							a[m] = 0;
						}
						//继续寻找下产生式
						j = t;
						break;
					}

				}

			}
			//否则继续向右
		}
		//初始化a[]
		for (j = 0; j < 11; j++)
		{
			a[j] = 0;
		}
		//从右到左遍历右部验证是否可以将左部的follow集加入follow集（error）
		for (j = gramOldSet[i].formula[20] - 1; j > 1; j--)
		{
			//printf("_____i=%d,j=%d\n",i, j);
			//如果是非终结符，计算是否可以推出易普逊
			if (isN(gramOldSet[i].formula[j])!=-1)
			{
				//printf("//%d:是非终结符%c，计算是否可以推出易普逊\n",i,non_ter[isN(gramOldSet[i].formula[j])]);
				//查询所有推导
				for (t = 0; t < gg_num; t++)
				{
					if (gramOldSet[t + 100].formula[0] == non_ter[isN(gramOldSet[i].formula[j])] && gramOldSet[t + 100].formula[2] == '@')
					{
						t = 1000;
						break;
					}
				}
				//若不能推出易普逊，赋予follow集后则终止
				if (t != 1000)
				{
					//printf("//%d:非终结符%c，不可以推出易普逊\n",i,non_ter[isN(gramOldSet[i].formula[j])]);
					for (m = 0; m < followSET[isN(gramOldSet[i].formula[0])][20]; m++)
					{
						//先查重
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
							//打印
							//pp();
						}

					}
					break;
					
				}
				//若能则赋予follow集，继续向左
				else
				{
					//printf("//%d:非终结符%c，可以推出易普逊\n",i, non_ter[isN(gramOldSet[i].formula[j])]);
					for (m = 0; m < followSET[isN(gramOldSet[i].formula[0])][20]; m++)
					{
						//先查重
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
							//打印
							//pp();
						}

					}
						
					
				}
			}
			//遇到终结符则跳出
			else
			{
				break;
			}

		}
		//跳出条件
		if (i == (g_num - 1)&&flag==0)
		{
			break;
		}
		
		//不能跳出则继续重复循环
		if (i == (g_num - 1))
		{
			i = -1;
			flag = 0;
		}
		//("\n%d\n", i);
		//pp();
		
	}
	//打印
	pp();


}

int pro_first2(int a[10], int length)
{
	//初始化存储位置
	zh1++;
	//赋初始值
	firstSET[zh1][20] = 0;
	int i,j;
	for (i = 0; i < length; i++)
	{
		//若检测到终结符
		if (isT(a[i]) != -1)
		{
			//若之前可以推出易普逊则加入终结符
			if (firstSET[zh1][20] > 0)
			{
				if (firstSET[zh1][firstSET[zh1][20] - 1] == '@')
				{
					
					//检测加入first集
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
				//若第一个为终结符
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
			//调用first1函数求first集
			zh1 = pro_first1(a, (i + 1));
		}
	}
}

int build_f()
{
	int i, j, m;
	int x, y;
	int a[12], length, w=0;
	int fir;// first集return值
	int fol;//follow集return值
	/*      x   终结符0    终结符1   终结符2   终结符3  ... （不含易普逊）
	    y
	非终结符0   产生式
	非终结符1   产生式
	非终结符2   报错
	非终结符3   产生式
	...
	*/
	//初始化M[][]
	for (i = 0; i < 100; i++)
	{
		for (j = 0; j < 100; j++)
		{
			M[i][j] = -1;
		}
	}
	//遍历规则
	for (i = 0; i < g_num; i++)
	{
		//确定y位置
		y = isN(gramOldSet[i].formula[0]);
		//若右端不为易普逊，求右端first集
		if (gramOldSet[i].formula[2] != '@')
		{
			//初始化
			length = 0;
			fir = 0;
			for (j = 0; j < 11; j++)
			{
				a[10] = 0;
			}
			//加入右端到a[]
			for (j = 2; j < gramOldSet[i].formula[20]; j++)
			{
				//若是终结符
				if (isT(gramOldSet[i].formula[j]) != -1)
				{
					a[j - 2] = gramOldSet[i].formula[j];
					length++;
				}
				//若是非终结符
				else
				{
					a[j - 2] = isN(gramOldSet[i].formula[j]);
					length++;
				}
			}
			//求first集
			fir = pro_first2(a, length);
			/*for (j = 0; j < firstSET[fir][20]; j++)
			{
				printf("%c", firstSET[fir][j]);
			}
			printf("   %d\n", firstSET[fir][20]);*/
			//加入到M表
			for (j = 0; j < firstSET[fir][20]; j++)
			{
				//求的x坐标
				x = isT(firstSET[fir][j]);
				//以x，y位置加入产生式id
				M[x][y] = i;
				/*printf("加入[%d][%d]", x, y);
				for (m = 0; m < gramOldSet[i].formula[20]; m++)
				{
					printf("%c", gramOldSet[i].formula[m]);
				}
				printf("\n");*/
			}
		}
		//若为易普逊，求左端follow集
		else
		{
			//加入到M表
			for (j = 0; j < followSET[isN(gramOldSet[i].formula[0])][20]; j++)
			{
				//求的x坐标
				x = isT(followSET[isN(gramOldSet[i].formula[0])][j]);
				//以x，y位置加入产生式id
				M[x][y] = i;
				/*printf("加入[%d][%d]", x, y);
				for (m = 0; m < gramOldSet[i].formula[20]; m++)
				{
					printf("%c", gramOldSet[i].formula[m]);
				}
				printf("\n");*/
			}
		}
		//完成后继续循环
		
	}
	//打印
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