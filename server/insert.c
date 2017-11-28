#include "func_linux.h"
int insert(pAct cli)
{
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	char* server="localhost";
	char* user="root";
	char* password="JonathanK2012!";
	char* database="ACCOUNTS";
	char query[200];
	int t,r;
//	FILE *fp;
//	if(argc!=3)
//	{
//		printf("error args\n");
//		return -1;
//	}
//	fp=fopen(argv[1],"r");
//	if(NULL==fp)
//	{
//		perror("fopen");
//		return -1;
//	}
//	char name[30];
	conn=mysql_init(NULL);
	if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0))
	{
		printf("Error connecting to database:%s\n",mysql_error(conn));
		return -1;
	}else{
		printf("Connected...\n");
	}
	//	cli->username[strlen(cli->username)-1]='\0';
		memset(query,0,sizeof(query));
		sprintf(query,"%s%s%s%s%s%s%s","insert into Person(username,password,path) values('",cli->username,"','",cli->scr,"','",cli->path,"')");
		puts(query);
		t=mysql_query(conn,query);
		if(t)
		{
			printf("Error making query:%s\n",mysql_error(conn));
			return -1;
		}else{
			printf("insert success\n");
		}
//	fclose(fp);
	mysql_close(conn);
	return 0;
}
