#include <mysql/mysql.h>
#include <stdio.h>
#include <string.h>
int main(int argc,char* argv[])
{
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	char* server="localhost";
	char* user="root";
	char* password="JonathanK2012!";
	char* database="OnePiece";
	char query[200];
	int t,r;
	FILE *fp;
	if(argc!=2)
	{
		printf("error args\n");
		return -1;
	}
	fp=fopen(argv[1],"r");
	if(NULL==fp)
	{
		perror("fopen");
		return -1;
	}
	char name[30];
	conn=mysql_init(NULL);
	if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0))
	{
		printf("Error connecting to database:%s\n",mysql_error(conn));
	}else{
		printf("Connected...\n");
	}
//	int personID=atoi(argv[2]);
	while(memset(name,0,sizeof(name)),fgets(name,sizeof(name),fp)!=NULL)
	{
		name[strlen(name)-1]='\0';
		memset(query,0,sizeof(query));
		sprintf(query,"%s%s%s","insert into Person(FirstName) values('",name,"')");
		puts(query);
		t=mysql_query(conn,query);
		if(t)
		{
			printf("Error making query:%s\n",mysql_error(conn));
			break;
		}else{
			printf("insert success\n");
		}
	}			
	fclose(fp);
	mysql_close(conn);
	return 0;
}
