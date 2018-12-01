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

void pp(Stack *s,Stack *s1)
{
	while(s->count!=0)
	{
	    Node *test_node;
	    test_node = pop(s);
	    push(test_node->data,s1);
	    //printf("%d  %d\n",s->count,test_node->data);
    }
}

int information(Stack *s,FILE*fp);//初始化代码并入栈。
int process(Stack *s);//分隔单词 
int add_string(Stack *s,char start);//拼接连续的字符
int add_num(Stack *s,char start);//拼接连续的字符
int add_delimiters(Stack *s,char start);
int delimiters_search(char *string);
//关键字初值定义(32个) 
char *rwtab[33] = {"auto"," short","int","long","float","double","char","struct",
                   "union","enum","typedef","const","unsigned","signed","extern",
                   "register","static","volatile","void","if","else","switch","case", 
                   "for","do","while","goto","continue","break","default","sizeof","return"};
//界符(37个)
char *delimiters[50] = {"*","/",":",":=","<","<>","<=",">",">=","=",";","(",")","#","-","+","++","--", 
	                    "[","]","{","}","&","\"","'","==","%",".","||","&&","->","\,","!=","+=","-=","*","\\"};

//NUM和ID
char *ni[3] = {"1","a"}; 
//引号里的内容

int main()
{
	int fact,i;
    //扫描代码 
	Stack s;
	stack_init(&s);
	Stack s1;
	stack_init(&s1);
	FILE*fp;
	fp=fopen("test.txt","rb");
	if(fp==NULL)
	{
		printf("无法打开!\n");
		return 0;
	}
  	else printf("成功打开!\n");//首先确认可以打开 。
//    printf("%d\n",&s);
  	fact=information(&s,fp);//读取文件信息，并用字符数组对文件信息进行储存。
//	printf("hou1 s = %d\n",&s); 
  	fclose(fp);
  	//printf("s.count = %d\n",s.count);
  	pp(&s,&s1);
    fact=process(&s1);//分隔单词 
  	//pp(&s);
  	system("pause");
  	return 0;
}

int information(Stack *s,FILE*fp)//初始化代码并入栈。 
{
	//printf("hou s = %d\n",s);
	Node node;//用于入栈 
	Node *test_node; //用于判断字符 
  	int i=0,j=0,fact=0,t=0,m=0; 
  	char *temp=0;
  	char over;
  	temp=malloc(10000);
  	fseek(fp,0,SEEK_SET);//定位 
  	fread(temp,sizeof(char),10000,fp);
  	for(i=0;i<10000;i++)//将字符串存入栈s中 
	{
		if(temp[i]==0)//结束则跳出 
		{
			break;
		}
		else//未结束入栈 
		{
			if(temp[i]=='/'&&temp[i+1]=='/')//去注释(//) 
			{
				t=0;
				for(m=i;temp[m]!=13;m++)
				{
					//printf("%c",test[m]);
					t++;//计算位移量 
				}
				//printf("%d",t) ; 
				i=i+t+1;
				continue;
			} 
			if(temp[i]=='/'&&temp[i+1]=='*')//去注释(/**/) 
			{
				t=0;
				for(m=i;temp[m]!='*'||temp[m+1]!='/';m++)
				{
					//printf("%c",temp[m]);
					t++;//计算位移量 
				}
				//printf("%d",t) ; 
				i=i+t+1;
				continue;
			}
			if(temp[i]==13||temp[i]==10||temp[i]==9)//去回车，换行，tab 
			{
				 continue; 
			}
			if(temp[i]==32)//合并多个空格为一个 
			{
				test_node = get_top_stack(s);//获取栈顶 
				if(test_node->data==32) 
				{
					continue;
				}
				else
				{
					//test[j]=32;
					//node.data = 32;
					push(32,s);//入栈 
					test_node = get_top_stack(s);//获取栈顶 
				    //printf("%c",test_node->data);
					j++;
					continue; 
				} 
			}
			
			//node.data = temp[i];
			//push(&node,s);//入栈 
			push(temp[i],s);
            test_node = get_top_stack(s);//获取栈顶 
			//printf("%c",test_node->data);
			j++; 
			
		}
	}                       
	fact = j;
	
	printf("s.count = %d\n",s->count);
    return fact;
}

int process(Stack *s)//分隔单词 
{
	Node node;//用于入栈 
	Node *test_node; //用于判断字符 
	char *string; string = malloc(20);
	char *num; num = malloc(20); 
	char *lim; lim = malloc(5);
	int k=0;
	int i=0,j=0,fact,m;
	while(1)
	{
		if(s->count==0)
		{
			printf("\n分隔完成");
			break;
		}
		//printf("process s->hand=%d,count = %d\n",s->head,s->count);
		//printf("pop s->head = %d\n",s->head);
	    //printf("pop s->head->next = %d\n",s->head->next);
		test_node = pop(s);
		//printf("%c",test_node->data);
		if(test_node->data==32)//跳过空格 
		{
			continue;
		}
		
		if((test_node->data>64&&test_node->data<91)||(test_node->data>96&&test_node->data<123))//大小写字母 
		{
			add_string(s,test_node->data);
		}
		else
		{
			if(test_node->data>47&&test_node->data<58)//数字 
			{
				add_num(s,test_node->data);
			} 
			else//其他字符 
			{
				add_delimiters(s,test_node->data);
			}
		}
	}
	return fact;
} 

int word_search(char *string)
{
	
	int i;
	//printf("%s",string);
	//printf("**\n");
	for(i=0;i<32;i++)
	{
		if(strncmp(string,rwtab[i],strlen(rwtab[i])) == 0&&strlen(string)==strlen(rwtab[i]))
		{
			printf("<%2d, - >关键字%s\n",i,string);
			//printf("%s",string);
			return i;
		}
		
	}

	printf("<88,%s>\n",string);
	//printf("%s",string);

	return 0;
}

int delimiters_search(char *string)
{
	
	int i;
	int k=2;
    //printf("\n*******%s******\n",string);
	//printf("*******%d******\n",strlen(string));
	//printf("**\n"); 
	for(i=0;i<37;i++)//先匹配长度位2的分隔符，再匹配1位 
	{
		if(strncmp(string,delimiters[i],k) == 0&&k==strlen(delimiters[i]))
		{
			printf("<%2d,%s>\n",i+32,delimiters[i]);
			//printf("%s",delimiters[i]);
			if(strlen(string)>k)//如果后面还有 
			{
				string = string+k;//后移k位 
				//k值 
				if(strlen(string)>1)//2位及以上 
				{
					k=2;
				}
				else//还剩下1位 
				{
					k=1;
				}
				//printf("\n*******%s******\n",string);
				//printf("*******%d******\n",strlen(string));
				//i值 
				i=0;
				continue; 
			}
			return i;
			
		}
		if(i==36&&k==2)//2位匹配不成功则换成1位 
		{
			k=1;
			i=0;
			continue;
		}
		if(i==36&&k==1)
		{
			if(strlen(string)>1)//如果后面还有 
			{
				string = string+1;//后移k位 
				//k值 
				if(strlen(string)>1)//2位及以上 
				{
					k=2;
				}
				else//还剩下1位 
				{
					k=1;
				}
				//printf("\n*******%s******\n",string);
				//printf("*******%d******\n",strlen(string));
				//i值 
				i=0;
				continue; 
			}
		} 
	}
	printf("**error:%s**\n",string);
	return 0;
}

int add_string(Stack *s,char start)//拼接连续的字符 
{
	int j=0,i,num;
	Node *test_node; //用于判断字符 
	char *string; string = malloc(20);
	char *string1; string1 = malloc(20);
	string[0] = start;
	for(j=1;j<20;j++)//拼接连续的字符 
	{
		
		test_node = pop(s);
	    if((test_node->data>64&&test_node->data<91)||(test_node->data>96&&test_node->data<123)||test_node->data=='_'
		||(test_node->data>47&&test_node->data<58))//数字
	    {
		    string[j] = test_node->data;
		    //printf("%c",string[j]);
	    }
	    else
	    {
	    	push(test_node->data,s);
	    	break;
		}
    }
    //printf("%s",string);
	//printf("*\n");
    word_search(string);
	return j;
}

int add_num(Stack *s,char start)//拼接连续的字符 
{
	int j=0,i,num;
	Node *test_node; //用于判断字符 
	char *string; string = malloc(20);
	char *string1; string1 = malloc(20);
	string[0] = start;
	for(j=1;j<20;j++)//拼接连续的字符 
	{
		
		test_node = pop(s);
	    if(test_node->data>47&&test_node->data<58)//数字
	    {
		    string[j] = test_node->data;
		    //printf("%c",string[j]);
	    }
	    else
	    {
	    	if((test_node->data>64&&test_node->data<91)||(test_node->data>96&&test_node->data<123))
	    	{
	    		printf("**error:变量定义错误！\n"); 
			}
	    	push(test_node->data,s);
	    	break;
		}
    }
    //printf("%s",string);
	//printf("*\n");
    num = j+1;
   
    //倒序 
/*	char t, *s1,*s2;
    int n = strlen(string);
    s1 = string;
    s2 = string+n-1;
    while(s1<s2)
    {
        t = *s1;
        *s1 = *s2;
        *s2 = t;
        s1++;
        s2--;
    }*/
    printf("<99,%s>\n",string);
    
    //printf("%s",string);
   //printf("%s",string);
	//printf("***\n");
    //word_search(string);
	return j;
}

int add_delimiters(Stack *s,char start)//拼接连续的字符 
{
	int j=0,i,num;
	Node *test_node; //用于判断字符 
	char *string; string = malloc(20);
	char *string1; string1 = malloc(20);
	string[0] = start;
	for(j=1;j<5;j++)//拼接连续的字符 
	{
		
		test_node = pop(s);
	    if((test_node->data>47&&test_node->data<58)||(test_node->data>64&&test_node->data<91)||
		(test_node->data>96&&test_node->data<123)||test_node->data==32)//数字
	    {
		    push(test_node->data,s);
	    	break;
	    }
	    else
	    {
	    	//printf("********%c",test_node->data);
	    	string[j] = test_node->data;
		}
    }
    //printf("%s ",string);
	//printf("*\n");
    num = j+1;
   
    //倒序 
/*	char t, *s1,*s2;
    int n = strlen(string);
    s1 = string;
    s2 = string+n-1;
    while(s1<s2)
    {
        t = *s1;
        *s1 =*s2;
        *s2 = t;
        s1++;
        s2--;
    }
*/    
    //printf("**%s*\n",string);
	//printf("***\n");
    delimiters_search(string);
	return j;
}
 








