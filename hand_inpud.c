// PDU_date_generate.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "string.h"
#include "stdio.h"


char msgid_stroe[1000][8]={"0000000"};
char write_msgid_txt[1000][12]={"00 00 00 00"};
char write_payload_txt[1000][24]={"00 00 00 00 00 00 00 00"};

const char length[]=" 00 00 00 08 ";
int signal_len,start_bit,msg_value;


char msg_id[8] = {'0'};
int id_len = 0;
char msg_id_temp[8]={'0'};
int id_len_temp = 0;

int value_store[1000][16]={0};
int value[16]={0};
char char_value[16]={'0'};  //payload without formate
char write_value[24]={' '};  // payload with formate
int value_top = 0;



int push(int x)
{
	value[value_top++] += x%16;	
	return x/16;
}

char pop(int i)  //转化过程，int to char ；不想在这一步打印就去掉 printf();
{	
	switch (i){
	case 0: return '0';
	case 1: return '1';
	case 2: return '2';
	case 3: return '3';
	case 4: return '4';
	case 5: return '5';
	case 6: return '6';
	case 7: return '7';
	case 8: return '8';
	case 9: return '9';
	case 10: return 'A';
	case 11: return 'B';
	case 12: return 'C';
	case 13: return 'D';
	case 14: return 'E';
	case 15: return 'F';
	default : return 0;
	}
}


int change_value()  //将value转化成对应的十六进制数据；并存放在char_value[16]中
{
	while(value_top)
	{
		value_top--;
		char_value[15-value_top] = pop(value[value_top]);
	}
	return 0;
}

int make_payload()////////////////////////   不带格式的char_value[16]转化成带格式(payload)的write_value[24] ；需要把它放在 write_payload_txt[1000][24]中再调用fwrite()
{
	int x = 0;
	for(int i=0;i<24;i++)
	{
		
		if(i==23)
			write_value[i] = '\n';
		else if(i%3==2)
			write_value[i] = ' ';	
		else
			write_value[i]=char_value[x++];	
	}
	return 0;
}

int msgid_in_store(char *msg_id)  //判断新输入的msg_id是否在store中；
{
	char str[]="0000000";
	int x = 0;
	for(int i=0;i<1000;i++)  //判断新输入的msg_id是否在store中；
	{
		if(!strcmp(msg_id,msgid_stroe[i])) // in
		{
			for(int ii=0;ii<16;ii++)   
			{
				if(ii>=start_bit/4&&ii<=(start_bit+signal_len)/4)
					msg_value= push(msg_value+value_store[i][ii]);
			}
			return i;
		}

		if(!strcmp(msgid_stroe[i],str)) // not in;write 
		{
			if(!strcmp(msg_id,msgid_stroe[i])) 
			{
				for(int ii=0;ii<16;ii++)   
				{				
					if(ii>=start_bit/4&&ii<=(start_bit+signal_len)/4)
					{
						value_store[i][ii]=msg_value%16;
						msg_value= push(msg_value);	
					}
				}
			
			}
			for(int j=0;j<12;j++)
			{
				if(j%3==2)
					write_msgid_txt[i][j]=' ';
				else
				{
					write_msgid_txt[i][j] = msg_id[x++];
					return i;
				}
			}
			
		}
	}
	return 0;
}

int main_payload()
{
	printf("Input:msg_id signal_bit start_bit msg_value:\n");

//////////////////////////////////////////////////msg_id set
	while(1)
	{
		char c = getchar();
		if(c==' '||c==','||c=='\n')
			break;
		else
			msg_id_temp[id_len_temp++] = c;
	}
	for(int i = 0;i<8;i++)
	{
		if(i<=7-id_len_temp)
			msg_id[i] = 0;
		if(i>7-id_len_temp)
			msg_id[i] = msg_id_temp[id_len++];
	}
///////////////////////////////////////////////////       msg_id   (char []) set ok
	scanf("%d %d %d",&signal_len,&start_bit,&msg_value);//signal_bit start_bit msg_value  (int) set ok;
	msgid_in_store(msg_id);
	change_value();
	make_payload();
	return 0;
}

void main_list_payload()
{
	int x= msgid_in_store(msg_id);
	for(int j=0;j<=x;j++)
	{
		for(int i=0;i<12;i++)
			printf("%c",write_msgid_txt[j][i]);
		printf(" 00 00 00 08 ");
		for(i=0;i<24;i++)
			printf("%c",write_payload_txt[j][i]);		
	}
}

void main_write_payload()
{
	
}




int main()
{
	while(1)
	{
		main_payload();
		main_list_payload();

		
	}



	return 0;
}
