#include "func_linux.h"

int quary(pAct cli)
{
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	char* server="localhost";
	char* user="root";
	char* password="JonathanK2012!";
	char* database="ACCOUNTS";
	char* query_1="select password,path from Person where username='";
	char query[200];
	int t,r;
	int flag=0;
	conn=mysql_init(NULL);
	if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0))
	{
		printf("Error connecting to database:%s\n",mysql_error(conn));
	}else{
		printf("Connected...\n");
	}
//	printf("username=%s",cli->username);
//	sleep(5);
	sprintf(query,"%s%s%s",query_1,cli->username,"'");
//	printf("query=%s\n",query);
	t=mysql_query(conn,query);
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(conn));
		return -1;
	}else{
		printf("Query made...\n");
		res=mysql_use_result(conn);
		if(res)
		{
			while((row=mysql_fetch_row(res))!=NULL)
			{
				flag=1;
				strcpy(cli->scr,row[0]);
				strcpy(cli->path,row[1]);
			}
//			while((row=mysql_fetch_row(res))!=NULL)
//			{	
				//printf("num=%d\n",mysql_num_fields(res));//列数
//				for(t=0;t<mysql_num_fields(res);t++){
//					printf("%8s\n",row[t]);
//				}
//				printf("\n");
//			}
		}
		mysql_free_result(res);
	}
	mysql_close(conn);
	if(1==flag){
		return 0;
	}
	else{
	return -1;
	}
}
