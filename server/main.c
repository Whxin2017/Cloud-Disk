#include"func_linux.h"
int main(int argc,char *argv[]){
	if(argc!=4){
		printf("error args\n");
		return -1;
	}
	int pro_num=atoi(argv[3]);
	pdata p=(pdata)calloc(pro_num,sizeof(data_t));
	make_child(p,pro_num);
	
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
	ret=bind(sfd,(struct sockaddr*)&ser,sizeof(struct sockaddr));
	if(-1==ret){
		perror("bind");
		return -1;
	}
	ret=listen(sfd,10);
	if(-1==ret){
		perror("listen");
		return -1;
	}
	struct sockaddr_in cli;
	memset(&cli,0,sizeof(cli));
	socklen_t addrlen=sizeof(cli);
	int kqfd=kqueue();
	struct kevent event;
	memset(&event,0,sizeof(event));
	EV_SET(&event,sfd,EVFILT_READ,EV_ADD,0,0,NULL);
	ret=kevent(kqfd,&event,1,NULL,0,NULL);
	if(-1==ret){
		perror("kevent zhuce");
		return -1;
	}

	int new_fd;
	int i;
	struct kevent *evs=(struct kevent*)calloc(pro_num+1,sizeof(struct kevent));
	for(i=0;i<pro_num;i++){
		memset(&event,0,sizeof(event));
		EV_SET(&event,p[i].sfd,EVFILT_READ,EV_ADD,0,0,NULL);
		kevent(kqfd,&event,1,NULL,0,NULL);
	}
	int j;
	char flag;
//	char buf[16];
	int ret1;
	while(1){
		memset(evs,0,(pro_num+1)*sizeof(struct kevent));
		ret=kevent(kqfd,NULL,0,evs,pro_num+1,NULL);
		for(i=0;i<ret;i++){
			if(evs[i].ident==sfd){
				new_fd=accept(sfd,NULL,NULL);
//				memset(buf,0,sizeof(buf));
//				ret1=recv(new_fd,buf,sizeof(buf),0);
				if(-1==ret1){
					perror("recv");
					return -1;
				}	
			
				for(j=0;j<pro_num;j++){
					if(p[j].busy==0){
						send_fd(p[j].sfd,new_fd);
						//ret1=send(new_fd,&flag,1,0);				
						if(-1==ret1){
							perror("send");
							return -1;
						}
						p[j].busy=1;
						printf("child is busy\n");
						break;
					}
				}
				close(new_fd);
			}
		for(j=0;j<pro_num;j++){
			if(evs[i].ident==p[j].sfd){
				read(p[j].sfd,&flag,sizeof(flag));				
				p[j].busy=0;
				printf("child is not busy\n");
				break;		
			}	
		}
		}
	}
	wait(NULL);
	return 0;	
}
