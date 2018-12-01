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

int information(Stack *s,FILE*fp);//��ʼ�����벢��ջ��
int process(Stack *s);//�ָ����� 
int add_string(Stack *s,char start);//ƴ���������ַ�
int add_num(Stack *s,char start);//ƴ���������ַ�
int add_delimiters(Stack *s,char start);
int delimiters_search(char *string);
//�ؼ��ֳ�ֵ����(32��) 
char *rwtab[33] = {"auto"," short","int","long","float","double","char","struct",
                   "union","enum","typedef","const","unsigned","signed","extern",
                   "register","static","volatile","void","if","else","switch","case", 
                   "for","do","while","goto","continue","break","default","sizeof","return"};
//���(37��)
char *delimiters[50] = {"*","/",":",":=","<","<>","<=",">",">=","=",";","(",")","#","-","+","++","--", 
	                    "[","]","{","}","&","\"","'","==","%",".","||","&&","->","\,","!=","+=","-=","*","\\"};

//NUM��ID
char *ni[3] = {"1","a"}; 
//�����������

int main()
{
	int fact,i;
    //ɨ����� 
	Stack s;
	stack_init(&s);
	Stack s1;
	stack_init(&s1);
	FILE*fp;
	fp=fopen("test.txt","rb");
	if(fp==NULL)
	{
		printf("�޷���!\n");
		return 0;
	}
  	else printf("�ɹ���!\n");//����ȷ�Ͽ��Դ� ��
//    printf("%d\n",&s);
  	fact=information(&s,fp);//��ȡ�ļ���Ϣ�������ַ�������ļ���Ϣ���д��档
//	printf("hou1 s = %d\n",&s); 
  	fclose(fp);
  	//printf("s.count = %d\n",s.count);
  	pp(&s,&s1);
    fact=process(&s1);//�ָ����� 
  	//pp(&s);
  	system("pause");
  	return 0;
}

int information(Stack *s,FILE*fp)//��ʼ�����벢��ջ�� 
{
	//printf("hou s = %d\n",s);
	Node node;//������ջ 
	Node *test_node; //�����ж��ַ� 
  	int i=0,j=0,fact=0,t=0,m=0; 
  	char *temp=0;
  	char over;
  	temp=malloc(10000);
  	fseek(fp,0,SEEK_SET);//��λ 
  	fread(temp,sizeof(char),10000,fp);
  	for(i=0;i<10000;i++)//���ַ�������ջs�� 
	{
		if(temp[i]==0)//���������� 
		{
			break;
		}
		else//δ������ջ 
		{
			if(temp[i]=='/'&&temp[i+1]=='/')//ȥע��(//) 
			{
				t=0;
				for(m=i;temp[m]!=13;m++)
				{
					//printf("%c",test[m]);
					t++;//����λ���� 
				}
				//printf("%d",t) ; 
				i=i+t+1;
				continue;
			} 
			if(temp[i]=='/'&&temp[i+1]=='*')//ȥע��(/**/) 
			{
				t=0;
				for(m=i;temp[m]!='*'||temp[m+1]!='/';m++)
				{
					//printf("%c",temp[m]);
					t++;//����λ���� 
				}
				//printf("%d",t) ; 
				i=i+t+1;
				continue;
			}
			if(temp[i]==13||temp[i]==10||temp[i]==9)//ȥ�س������У�tab 
			{
				 continue; 
			}
			if(temp[i]==32)//�ϲ�����ո�Ϊһ�� 
			{
				test_node = get_top_stack(s);//��ȡջ�� 
				if(test_node->data==32) 
				{
					continue;
				}
				else
				{
					//test[j]=32;
					//node.data = 32;
					push(32,s);//��ջ 
					test_node = get_top_stack(s);//��ȡջ�� 
				    //printf("%c",test_node->data);
					j++;
					continue; 
				} 
			}
			
			//node.data = temp[i];
			//push(&node,s);//��ջ 
			push(temp[i],s);
            test_node = get_top_stack(s);//��ȡջ�� 
			//printf("%c",test_node->data);
			j++; 
			
		}
	}                       
	fact = j;
	
	printf("s.count = %d\n",s->count);
    return fact;
}

int process(Stack *s)//�ָ����� 
{
	Node node;//������ջ 
	Node *test_node; //�����ж��ַ� 
	char *string; string = malloc(20);
	char *num; num = malloc(20); 
	char *lim; lim = malloc(5);
	int k=0;
	int i=0,j=0,fact,m;
	while(1)
	{
		if(s->count==0)
		{
			printf("\n�ָ����");
			break;
		}
		//printf("process s->hand=%d,count = %d\n",s->head,s->count);
		//printf("pop s->head = %d\n",s->head);
	    //printf("pop s->head->next = %d\n",s->head->next);
		test_node = pop(s);
		//printf("%c",test_node->data);
		if(test_node->data==32)//�����ո� 
		{
			continue;
		}
		
		if((test_node->data>64&&test_node->data<91)||(test_node->data>96&&test_node->data<123))//��Сд��ĸ 
		{
			add_string(s,test_node->data);
		}
		else
		{
			if(test_node->data>47&&test_node->data<58)//���� 
			{
				add_num(s,test_node->data);
			} 
			else//�����ַ� 
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
			printf("<%2d, - >�ؼ���%s\n",i,string);
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
	for(i=0;i<37;i++)//��ƥ�䳤��λ2�ķָ�������ƥ��1λ 
	{
		if(strncmp(string,delimiters[i],k) == 0&&k==strlen(delimiters[i]))
		{
			printf("<%2d,%s>\n",i+32,delimiters[i]);
			//printf("%s",delimiters[i]);
			if(strlen(string)>k)//������滹�� 
			{
				string = string+k;//����kλ 
				//kֵ 
				if(strlen(string)>1)//2λ������ 
				{
					k=2;
				}
				else//��ʣ��1λ 
				{
					k=1;
				}
				//printf("\n*******%s******\n",string);
				//printf("*******%d******\n",strlen(string));
				//iֵ 
				i=0;
				continue; 
			}
			return i;
			
		}
		if(i==36&&k==2)//2λƥ�䲻�ɹ��򻻳�1λ 
		{
			k=1;
			i=0;
			continue;
		}
		if(i==36&&k==1)
		{
			if(strlen(string)>1)//������滹�� 
			{
				string = string+1;//����kλ 
				//kֵ 
				if(strlen(string)>1)//2λ������ 
				{
					k=2;
				}
				else//��ʣ��1λ 
				{
					k=1;
				}
				//printf("\n*******%s******\n",string);
				//printf("*******%d******\n",strlen(string));
				//iֵ 
				i=0;
				continue; 
			}
		} 
	}
	printf("**error:%s**\n",string);
	return 0;
}

int add_string(Stack *s,char start)//ƴ���������ַ� 
{
	int j=0,i,num;
	Node *test_node; //�����ж��ַ� 
	char *string; string = malloc(20);
	char *string1; string1 = malloc(20);
	string[0] = start;
	for(j=1;j<20;j++)//ƴ���������ַ� 
	{
		
		test_node = pop(s);
	    if((test_node->data>64&&test_node->data<91)||(test_node->data>96&&test_node->data<123)||test_node->data=='_'
		||(test_node->data>47&&test_node->data<58))//����
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

int add_num(Stack *s,char start)//ƴ���������ַ� 
{
	int j=0,i,num;
	Node *test_node; //�����ж��ַ� 
	char *string; string = malloc(20);
	char *string1; string1 = malloc(20);
	string[0] = start;
	for(j=1;j<20;j++)//ƴ���������ַ� 
	{
		
		test_node = pop(s);
	    if(test_node->data>47&&test_node->data<58)//����
	    {
		    string[j] = test_node->data;
		    //printf("%c",string[j]);
	    }
	    else
	    {
	    	if((test_node->data>64&&test_node->data<91)||(test_node->data>96&&test_node->data<123))
	    	{
	    		printf("**error:�����������\n"); 
			}
	    	push(test_node->data,s);
	    	break;
		}
    }
    //printf("%s",string);
	//printf("*\n");
    num = j+1;
   
    //���� 
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

int add_delimiters(Stack *s,char start)//ƴ���������ַ� 
{
	int j=0,i,num;
	Node *test_node; //�����ж��ַ� 
	char *string; string = malloc(20);
	char *string1; string1 = malloc(20);
	string[0] = start;
	for(j=1;j<5;j++)//ƴ���������ַ� 
	{
		
		test_node = pop(s);
	    if((test_node->data>47&&test_node->data<58)||(test_node->data>64&&test_node->data<91)||
		(test_node->data>96&&test_node->data<123)||test_node->data==32)//����
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
   
    //���� 
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
 








