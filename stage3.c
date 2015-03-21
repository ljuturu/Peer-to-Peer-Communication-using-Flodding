#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <thread.h>
#include <unistd.h>

#define P 10

typedef struct MSG
{
	unsigned int sender;
	unsigned int TTL;
	unsigned int msg;
	struct MSG *next;
 } node,*node_ptr;
node_ptr Q_head[P+1], Q_tail[P+1];

void sendMSG(int s,int r,int m,int t);
void enterQ(node_ptr curr_node, int qID);
node_ptr grabMSG(int x);

int rNum, NWL[P+2][P+2]={{0}};
mutex_t lock[P+1];

void sendMSG(int s,int qID,int m,int t)
{
	node_ptr curr_node;
	curr_node = malloc(sizeof(node));
	if (curr_node==NULL) printf("Node creation failed/n");
	else
	{
		curr_node->sender=s;
		curr_node->TTL=t;
		curr_node->msg=m;
		enterQ(curr_node, qID);
	}
}
void enterQ(node_ptr curr_node, int qID)
{
	mutex_lock(&lock[qID]);
	if (Q_head[qID]== NULL && Q_tail[qID]==NULL)
	{
		Q_head[qID]=curr_node;
	}
	else
	{
		Q_tail[qID]->next= curr_node;
	}
	Q_tail[qID]=curr_node;
	Q_tail[qID]->next=NULL;

	mutex_unlock(&lock[qID]);
}
node_ptr grabMSG(int x)
{
	node_ptr y, tmp;
	mutex_lock(&lock[x]);
	if (Q_head[x]==NULL && Q_tail[x]==NULL)
		y=NULL;
	else if (Q_head[x]->next != NULL)
	{
		y=Q_head[x];
		tmp=Q_head[x];
		Q_head[x]=Q_head[x]->next;
		free(tmp);
	}
	else {
		y=Q_head[x];
		Q_head[x]=NULL;
		Q_tail[x]=NULL;
		free(Q_head[x]);
	}
	mutex_unlock(&lock[x]);
	return y;
}

int main (int argc, char *argv[])
{
	int x,k,cnt,i;
	node_ptr y=NULL;
	char line[30],*d,*n;
	FILE *file;
	for (i=1; i<=P+1; i++)
		mutex_init(&lock[i], USYNC_THREAD, NULL);

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
		printf("Cannot read the file");
		exit(1);
	}
	sendMSG(1, 2, 1000, 1); //sender = 1, receiver=2, msg = 1000, TTL=1
	sendMSG(1, 2, 2000, 2);
	sendMSG(1, 4, 2000, 4);
	if ((y = grabMSG(2)) != NULL)
		printf ("Queue 2->sender: %d  Queue 2->msg: %d  Queue 2->TTL: %d\n",y->sender,y->msg,y->TTL);
	else printf("Queue 2 is empty\n");
	if ((y = grabMSG(3)) != NULL)
		printf ("Queue 3->sender: %d  Queue 3->msg: %d  Queue 3->TTL: %d\n",y->sender,y->msg,y->TTL);
	else printf("Queue 3 is empty\n");
	if ((y = grabMSG(2)) != NULL)
		printf ("Queue 2->sender: %d  Queue 2->msg: %d  Queue 2->TTL: %d\n",y->sender,y->msg,y->TTL);
	else printf("Queue 2 is empty\n");

	for(i=1;i<=rNum+1;i++)
		if(Q_head[i] != NULL)
			printf("Queue %d is not null\n",i);
	for (i=1; i<=P+1; i++)
		mutex_destroy(&lock[i]);
	return 0;
}
