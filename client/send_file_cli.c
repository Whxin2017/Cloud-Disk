#include"func_linux.h"
void send_file(int new_fd,char *file_path){
	int fd;
	fd=open(file_path,O_RDONLY);
	if(-1==fd){
		perror("open");
		return;
	}
	int i;
	int cnt=0;
	char file_name[128];
	train t;
	memset(&t,0,sizeof(t));
	for(i=0;file_path[i]!=0;i++){
		if(file_path[i]=='/'){
			cnt++;
		}
	}
	for(i=0;cnt!=0;i++){
		if(file_path[i]=='/'){
			cnt--;
		}
	}	
	//file_path[i]=0;
	strcpy(file_name,&file_path[i]);
	strcpy(t.buf,file_name);
//	printf("file_name=%s\n",file_name);
	t.len=strlen(file_name);
	send(new_fd,&t,4+t.len,0);
	while((memset(&t,0,sizeof(t)),t.len=read(fd,t.buf,sizeof(t.buf)))){
		if(-1==send_n(new_fd,(char*)&t,4+t.len)){
			close(new_fd);
			return;
		}
	}
	int flag=0;
	send_n(new_fd,(char*)&flag,sizeof(int));
//	close(new_fd);
	close(fd);
}
