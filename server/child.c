#include"func_linux.h"
void init_ftp(char *path){
	char newpath[128];
	sprintf(newpath,"%s%c%s",path,'/',"picture");
	mkdir(newpath,0777);
	memset(newpath,0,sizeof(newpath));
	sprintf(newpath,"%s%c%s",path,'/',"document");
	mkdir(newpath,0777);
	memset(newpath,0,sizeof(newpath));
	sprintf(newpath,"%s%c%s",path,'/',"vedio");
	mkdir(newpath,0777);
	memset(newpath,0,sizeof(newpath));
	sprintf(newpath,"%s%c%s",path,'/',"download");
	mkdir(newpath,0777);
}
char select_cmd(char *cmd){
	if(!strcmp(cmd,"cd")){
		return 1;
	}
	if(!strcmp(cmd,"ls")){
		return 2;
	}
	if(!strcmp(cmd,"puts")){
		return 3;
	}
	if(!strcmp(cmd,"gets")){
		return 4;
	}
	if(!strcmp(cmd,"rm")){
		return 5;
	}
	if(!strcmp(cmd,"pwd")){
		return 6;
	}
	return 7;
}
void child_handle(int sfd){
//	signal(SIGPIPE,SIG_IGN);
	int new_fd;
	char flag=0;
	char cmd[128];
	int ret;
	char ff='D';
	char *p;
		//char *path_1="/Users/OnePiece/ftp";
		char *path_1=getcwd(NULL,0);
	//	printf("path_1=%s\n",path_1);
		char buf[128];
		file_info pfs[128];
				DIR *dir;
				char buf_tem[128];
						int cnt;
						int i;
				char buf_rm[128];
				char *pwd;
					char buf_file[1024];

		Account usr;

		char path[128];
		
		char sct;
		char c;
		char scr_tem[32];
	while(1){
		memset(path,0,sizeof(path));
		recv_fd(sfd,&new_fd);
label:		
		recv(new_fd,&sct,1,0);
		printf("sct=%c",sct);

		memset(&usr,0,sizeof(usr));
		recv(new_fd,&usr,sizeof(Account),0);
		printf("username:%s\n",usr.username);
		printf("pwd:%s\n",usr.scr);
		if('1'==sct){
			ret=quary(&usr);
			if(0==ret){
				c=0;
				send(new_fd,&c,1,0);
				printf("the user is exit\n");
			}
			else{
				c=1;
				sprintf(path,"%s%c%s",path_1,'/',usr.username);
				printf("path=%s\n",path);
	//		while(1);
				strcpy(usr.path,path);
				mkdir(path,0777);
				init_ftp(path);				
			
				insert(&usr);       //insert mysql
				send(new_fd,&c,1,0);
			}
			goto label;
		}
		else{
			printf("hahaha\n");
			ret=quary(&usr);
			printf("ret=%d\n",ret);
			if(-1==ret){
				c=0;
				send(new_fd,&c,1,0);
				printf("the user is not exit!\n");
				goto label;
			}
			else{
				c=1;
				send(new_fd,&c,1,0);
				send(new_fd,usr.scr,2,0);
				memset(scr_tem,0,sizeof(scr_tem));
				recv(new_fd,scr_tem,sizeof(scr_tem),0);
				if(!strcmp(scr_tem,usr.scr)){
					c=1;
					printf("login sucess\n");
					send(new_fd,&c,1,0);
				//	break;
				}
				else{
					c=0;
					printf("login fail\n");
					send(new_fd,&c,1,0);
					goto label;
				}
			}
	}


	
		memset(buf,0,sizeof(buf));
		strcpy(buf,usr.path);
		memset(buf_tem,0,sizeof(buf_tem));
		memset(pfs,0,sizeof(pfs));
	while(1){
		memset(cmd,0,sizeof(cmd));
		ret=recv(new_fd,cmd,sizeof(cmd),0);
		if(ret==0){
			break;
		}
		if(-1==ret){
			perror("recv");
			return;
		}
		p=strtok(cmd," ");
		flag=select_cmd(p);
		switch(flag){
			case 1:
//				printf("cd\n");
				p=strtok(NULL," ");
//				printf("%c",p[strlen(p)]);
				//printf("%s\n",p);
			//	while(1);
				cnt=0;
				if(NULL==p){
		//			printf("no input file\n");
					strcpy(buf_tem,path);
				}
	//			p[strlen(p)]=0;
				else if(!strcmp(p,"..")){
					if(!strcmp(buf,usr.path)){
						printf("no back\n");
		//				break;
					}	
					else{
						for(i=0;buf[i]!=0;i++){
							if(buf[i]=='/'){
								cnt++;
							}
						}
						for(i=0;cnt!=0;i++){
							if(buf[i]=='/'){
								cnt--;
							}
						}	
						buf[i-1]=0;
						strcpy(buf_tem,buf);		
					}
				}
				else{
//				memset(buf_tem,0,sizeof(buf_tem));
				sprintf(buf_tem,"%s%c%s",buf,'/',p);
				}
				dir=opendir(buf_tem);
				if(NULL==dir){
					perror("opendir");
					continue;
				}
				memset(buf,0,sizeof(buf));
				strcpy(buf,buf_tem);
	//			while(1);
//				printf("%s\n",buf);	
			break;
	
			case 2:
//				printf("ls\n");
//				printf("%s\n",buf);
				memset(pfs,0,sizeof(pfs));
   				ret=list_file(buf,pfs);
				if(-1==ret){
					printf("list error\n");
					return ;
				}
				send(new_fd,&flag,1,0);
				send(new_fd,&ret,sizeof(ret),0);
				send(new_fd,pfs,ret*sizeof(file_info),0);
				break;

			case 3:
//				printf("puts\n");
				p=strtok(NULL," ");
				send(new_fd,&flag,1,0);
				printf("p=%s\n",p);
				send(new_fd,p,strlen(p),0);
				int len;			
				recv(new_fd,&len,sizeof(len),0);
				memset(buf_file,0,sizeof(buf_file));
				recv(new_fd,buf_file,len,0);
				int fd;
				sprintf(buf_tem,"%s%c%s",buf,'/',buf_file);
				fd=open(buf_tem,O_RDWR|O_CREAT,0666);
				if(-1==fd){
					perror("open");
					return ;
				}
				while(1){
					recv_n(new_fd,(char*)&len,sizeof(len));
					if(len>0){
						memset(buf_file,0,sizeof(buf_file));
						recv_n(new_fd,buf_file,len);
						write(fd,buf_file,len);
					}
					else{
						break;
					}
				}
				char suc_info=1;
				send(new_fd,&suc_info,1,0);
				printf("puts sucess!\n");
				break;

			case 4:
				printf("gets\n");
				send(new_fd,&flag,1,0);
				p=strtok(NULL," ");
				sprintf(buf_tem,"%s%c%s",buf,'/',p);
				send_file(new_fd,buf_tem,p);
				break;

			case 5:
//				printf("rm\n");
				p=strtok(NULL," ");
				memset(buf_rm,0,sizeof(buf_rm));
				sprintf(buf_rm,"%s%c%s",buf,'/',p);
//				printf("%s\n",buf_rm);
				remove(buf_rm);
				break;

			case 6:
//				printf("pwd\n");
				chdir(buf);
				
				pwd=getcwd(NULL,0);
//				printf("pwd is %s\n",pwd);
				send(new_fd,&flag,1,0);
				send(new_fd,pwd,strlen(pwd),0);
				break;

			case 7:
				printf("error\n");break;
		
	}
//				p=strtok(NULL," ");
//				printf("membef p=%s\n",p);
				if(p!=NULL){
				memset(p,0,strlen(p));
				}
//				printf("memaft p=%s\n",p);

//	flag=flag+'0';
}
	write(sfd,&ff,1);
	close(new_fd);
	}
//	close(new_fd);
}
void make_child(pdata p,int num){
	int fds[2];
	pid_t pid;
	int i;
	for(i=0;i<num;i++){
		socketpair(AF_LOCAL,SOCK_STREAM,0,fds);
		pid=fork();
		if(pid==0){
			close(fds[1]);
			child_handle(fds[0]);
		}
		close(fds[0]);
		p[i].pid=pid;
		p[i].sfd=fds[1];	
		p[i].busy=0;
	}
}
