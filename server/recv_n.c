#include"func_linux.h"
void recv_n(int sfd,char *buf,int len){
	int sum=0;
	int ret;
	while(sum<len){
		ret=recv(sfd,buf+sum,len-sum,0);
		sum+=ret;
	}
}
