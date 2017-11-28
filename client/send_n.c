#include"func_linux.h"
int send_n(int sfd,char *buf,int len){
	int sum=0;
	int ret;
	while(sum<len){
		ret=send(sfd,buf+sum,len-sum,0);
		if(ret<0){
			printf("errno=%d\n",errno);
			return -1;
		}
		else{
			sum+=ret;
		}
	}
	return 0;
}
