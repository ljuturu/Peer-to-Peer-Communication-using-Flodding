#include <stdio.h>
#include <string.h>

#define P 10
int rNum, NWL[P+2][P+2]={{0}};

int main (int argc, char *argv[])
{
	int x,k,cnt;
	char line[30],*d,*n;
	FILE *file;
	if(argc>0)
		file=fopen(argv[1],"r");
	if (file!=NULL && !feof(file))
	{
		fgets(line,20,file);
		rNum=atoi(line);
		for (k=1;k<=rNum;k++)
		{
			fgets(line,20,file);
			d=strtok(line," ");
			cnt=atoi(d);
			while(cnt>0)
			{
				n=strtok(NULL," ");
				x=atoi(n);
				NWL[k][x]=1;
				NWL[x][k]=1;
				cnt--;
			}
		}
		printf("matrix values:\n");
		for(k=1;k<=rNum+1;k++)
		{
			for(x=1;x<=rNum+1;x++)
				printf("%d ",NWL[k][x]);
			printf("\n");
		}
		fclose(file);
	}
	else
	{
		printf("Cannot read the file\n");
		exit(1);
	}
}
