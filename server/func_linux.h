#include<stdio.h>
#include<dirent.h>
#include<stdlib.h>
#include<strings.h>
#include<time.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<pwd.h>
#include<grp.h>
#include<fcntl.h>
#include<sys/select.h>
#include<sys/time.h>
#include<sys/mman.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<sys/shm.h>
#include<sys/wait.h>
#include<signal.h>
#include<pthread.h>
#include<errno.h>

#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<sys/event.h>
#include<sys/types.h>

#include<fcntl.h>
#include<sys/uio.h>

#include<mysql/mysql.h>
#define FILENAME "file"
typedef struct data{
	pid_t pid;
	short busy;
	int sfd;//socket 管道
}data_t,*pdata;
typedef struct{
	int len;
	char buf[1024];
}train,*ptrain;
typedef struct {
	char filename[16];
	int size;
	char filedate[16];
}file_info,*pf_info;
void make_child(pdata,int);
void send_file(int,char *,char *);
void send_fd(int,int);
void recv_fd(int,int*);
int send_n(int,char *,int);
int list_file(char *,pf_info);
void recv_n(int,char *,int); 
typedef struct{
	char username[16];
	char scr[32];
	char path[128];
}Account,*pAct;
int insert(pAct);
int quary(pAct);
