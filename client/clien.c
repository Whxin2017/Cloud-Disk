#include"func_linux.h"
int client(int sfd){
	char flag;
	fd_set rdset;
	char buf[128];
	int ret1;
	int ret;
	//				int len,i;
					file_info pfs[128];
					char buf_2[128];
					char buf_file[1024];
	int len;
	long long biglen;
	char ifmmp;
	while(1){
		FD_ZERO(&rdset);
		FD_SET(0,&rdset);
		FD_SET(sfd,&rdset);
		ret1=select(sfd+1,&rdset,NULL,NULL,NULL);
		memset(buf_2,0,sizeof(buf_2));
	if(ret1>0){
		if(FD_ISSET(0,&rdset)){
			memset(buf,0,sizeof(buf));
//			 setbuf(stdin, NULL);
			
			read(0,buf,sizeof(buf));
		//	printf("sendbuf=%s\n",buf);
         	ret=send(sfd,buf,strlen(buf)-1,0);
         	if(-1==ret){
         		perror("send");
         		return -1;
         	}
			if(ret<=0){
				printf("bye-bye\n");
				return -1;
			}
		system("clear\n");
		}
		if(FD_ISSET(sfd,&rdset)){
		//	memset(buf,0,sizeof(buf));
        flag=0;
		ret=recv(sfd,&flag,1,0);
        	if(-1==ret){
        		perror("recv");
        		return -1;
        	}
			if(ret>0){
        // 		printf("%c\n",flag+'0');
				switch(flag){

					case 1:
	//				printf("cd\n");
					break;

					case 2:
					//printf("\n");
					memset(pfs,0,sizeof(pfs));
					
					recv(sfd,&len,sizeof(len),0);
		//			printf("%d\n",len);
					recv(sfd,pfs,len*sizeof(file_info),0);
					int i;	
					for(i=0;i<len;i++){
					printf("\t-%-8s\t%-8d\t%s\n",pfs[i].filename,pfs[i].size,pfs[i].filedate);
					}
                    break;

					case 3:
				//	printf("puts\n");
					recv(sfd,buf_2,sizeof(buf_2),0);
				//	printf("buf_2=%s\n",buf_2);
					send_file(sfd,buf_2);
					char info;
					recv(sfd,&info,1,0);
					if(1==info) printf("upload sucess!\n");
					break;

					case 4:
					printf("gets\n");
			//		recv(sfd,&ifmmp,1,0);
			//		if(0==ifmmp){
			//			recv(sfd,&biglen,sizeof(biglen),0);
			//			if(biglen>0){
			//				memset(buf_file,0,sizeof)
			//			}	
			//		}
				
					recv(sfd,&len,sizeof(len),0);
					memset(buf_file,0,sizeof(buf_file));
					recv(sfd,buf_file,len,0);
					char *pwd=getcwd(NULL,0);
					sprintf(buf_2,"%s%c%s%c%s",pwd,'/',DOWN_DIR,'/',buf_file);
					int fd=open(buf_2,O_RDWR|O_CREAT,0666);
					if(-1==fd){
						perror("open");
						return -1;
					}
					while(1){
						recv_n(sfd,(char*)&len,sizeof(len));
						if(len>0){
							memset(buf_file,0,sizeof(buf_file));
							recv_n(sfd,buf_file,len);
							write(fd,buf_file,len);
						}
						else{
							break;
						}
					}
					printf("download sucess!\n");
					break;

					case 5:printf("remove\n");break;

					case 6:
				//	printf("pwd\n");
					recv(sfd,buf_2,sizeof(buf_2),0);
					printf("%s\n",buf_2);
					break;

					case 7:printf("error");break;
				}
			}
			else if(ret==0){
				printf("game over!\n");
				return -1;
			}
     	}
	}
}

}


void get_rand_str(char s[]){
 //定义随机生成字符串表
	char *str = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz./";
 	int i,lstr;
 	lstr = strlen(str);//计算字符串长度
 	srand((unsigned int)time((time_t *)NULL));//使用系统时间来初始化随机数发生器
 	for(i = 0; i < 2; i++){//按指定大小返回相应的字符串
  		s[i]=str[(rand()%lstr)];//rand()%lstr 可随机返回0-71之间的整数, str[0-71]可随机得到其中的字符
 	}
//	s[i]='$';
}


int main(int argc,char *argv[]){


	

	if(argc!=3){
		printf("error args\n");
		return -1;
	}
	int sfd;
	sfd=socket(AF_INET,SOCK_STREAM,0);
	if(-1==sfd){
		perror("socket");
		return -1;

	}
	struct sockaddr_in ser;
	memset(&ser,0,sizeof(ser));
	ser.sin_family=AF_INET;
	ser.sin_port=htons(atoi(argv[2]));
	ser.sin_addr.s_addr=inet_addr(argv[1]);
	int ret;
	ret=connect(sfd,(struct sockaddr *)&ser,sizeof(ser));
	if(-1==ret){
		perror("connect");
		return -1;
	}
	int jet;
	char c;
	char sct;
	char salt[3];
	char *password;
//	char *scr;
//注册登陆
	Account usr;
	system("clear");
	while(1){

	printf("1.Register\n");
	printf("2.Login\n");
	
	printf("please select:");
	while(1){
		scanf("%c",&sct);
		if(sct=='1' || sct=='2') break;
	}
		memset(&usr,0,sizeof(usr));
//		memset(password,0,sizeof(password));
		memset(salt,0,sizeof(salt));
//	system("clear");
		if('1'==sct){
			printf("***Register***\n");
		}
		else{
			printf("***Login***\n");
		}
		printf("username:");
		scanf("%s",usr.username);
	//	send(sfd,usr.username,strlen(usr.username),0);
		printf("password:");
		password=getpass("");
		printf("\n");
//		usr.salt[0]='$';usr.salt[1]='6';usr.salt[2]='$';
//		printf("\n");
	if('1'==sct){
		get_rand_str(salt);
		printf("salt=%s\n",salt);
		strcpy(usr.scr,crypt(password,salt));	
		printf("scr=%s\n",usr.scr);
		send(sfd,&sct,1,0);
		send(sfd,&usr,sizeof(Account),0);
	
		recv(sfd,&c,1,0);
		if(c==1) {
			printf("Register Sucess!\n");
		}	
		else{
			printf("the user is exit!\n");
		}
		continue;
	}
	
	else{
//		printf("c=%c\n",c);
		send(sfd,&c,1,0);
		send(sfd,&usr,sizeof(Account),0);
		recv(sfd,&c,1,0);
		if(0==c){
			printf("the user is not exit!\n");
			continue;
		}
		else{
	//		send(sfd,usr.username,sizeof(usr.username),0);
			recv(sfd,salt,sizeof(salt),0);
			strcpy(usr.scr,crypt(password,salt));
			send(sfd,usr.scr,strlen(usr.scr),0);
			recv(sfd,&c,1,0);
			if(c==0){
				printf("login fail!\n");
				continue;		
			}			
			else{
				printf("login sucess!\n");
				break;
			}					
		}
	}
}


	client(sfd);
	close(sfd);
	return 0;
}
