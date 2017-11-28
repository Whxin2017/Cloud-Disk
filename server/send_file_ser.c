#include"func_linux.h"
void send_file(int new_fd,char *file_path,char *file_name){
	int fd;
	char *p;
//	char ifmmp;
	struct stat buf1;
	stat(file_path,&buf1);
	fd=open(file_path,O_RDONLY);
	if(-1==fd){
		perror("open");
		return;
	}
//	if(buf1.st_size>100*(1<<10)){
//		printf("mmp!\n");
//		ifmmp=0;
//		p=(char*)mmap(NULL,buf1.st_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
//		send(new_fd,&ifmmp,1,0);
//		send(new_fd,&buf1.st_size,sizeof(buf1.st_size));
//		send_n(new_fd,p,buf1.st_size);
//	}
//	printf("size=%lld\n",buf1.st_size);
//	sleep(10);
//	else{
//	ifmmp=1;
//	send(new_fd,&ifmmp,1,0);
	train t;
	memset(&t,0,sizeof(t));
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
//	}
	close(fd);
}
