#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define WIDTH 1024
#define HEIGHT 768
int rdata[WIDTH][HEIGHT];
int gdata[WIDTH][HEIGHT];
int bdata[WIDTH][HEIGHT];

int main( int argc, char* argv[])
{
	FILE* input;
	FILE* output;
	int i;
	int j;
	int length=atoi(argv[2]);
//	int junk;
	int count = 0;
	long scount=1;
	char message[length];
	input = fopen(argv[1], "r");
	int rkey=atoi(argv[3]);
	int gkey=atoi(argv[4]);
	int bkey=atoi(argv[5]);
	fseek(input,54,SEEK_SET);	
	printf("header stripped\n");
	for(i=0;i<HEIGHT;i++)
	{
		for(j=0;j<WIDTH;j++)
		{
			bdata[j][i] = getc(input);  
			gdata[j][i] = getc(input); 
			rdata[j][i] = getc(input);  
		}
	}
	printf("%d",gdata[0][1]);
	fclose(input);
	printf("data got, file closed\n");
	while(count<length)
	{
		for(i=0;i<WIDTH&&count<length;i++)
		{
			for(j=0;j<HEIGHT&&count<length;j++)
			{
				if(scount%rkey==0)
				{
					message[count]=rdata[i][j];
					count++;
				}
				if(scount%gkey==0)
				{
					message[count]=gdata[i][j];
					count++;
				}
				if(scount%bkey==0)
				{
					message[count]=bdata[i][j];
					count++;
				}
				scount++;
			}
		}
	}
	output=fopen("debitcrypted.txt","w");
	fputs(message,output);
	fclose(output);
	return 0;
}
