#include "func_linux.h"
void send_fd(int sfd,int fd)
{
	struct msghdr msg;
	memset(&msg,0,sizeof(msg));
	struct iovec iov[2];
	char buf[10]="ONEPIECE";
	char buf1[10]="ONEPIECE";
	iov[0].iov_base=buf;
	iov[0].iov_len=strlen(buf)-1;
	iov[1].iov_base=buf1;
	iov[1].iov_len=strlen(buf1)-1;
	msg.msg_iov=iov;
	msg.msg_iovlen=2;
	struct cmsghdr *cmsg;
	int len=CMSG_LEN(sizeof(fd));
	cmsg=(struct cmsghdr *)calloc(1,len);	
	cmsg->cmsg_len=len;
	cmsg->cmsg_level = SOL_SOCKET;
        cmsg->cmsg_type = SCM_RIGHTS;
	*(int*)CMSG_DATA(cmsg)=fd;
	msg.msg_control=cmsg;
	msg.msg_controllen=len;
	int ret=sendmsg(sfd,&msg,0);
	if(-1==ret)
	{
		perror("sendmsg");
		return;
	}	
}
void recv_fd(int sfd,int *fd)
{
	struct msghdr msg;
	memset(&msg,0,sizeof(msg));
	struct iovec iov[2];
	char buf[10]="onepiece";
	char buf1[10]="onepiece";
	iov[0].iov_base=buf;
	iov[0].iov_len=strlen(buf)-1;
	iov[1].iov_base=buf1;
	iov[1].iov_len=strlen(buf1)-1;
	msg.msg_iov=iov;
	msg.msg_iovlen=2;
	struct cmsghdr *cmsg;
	int len=CMSG_LEN(sizeof(fd));
	cmsg=(struct cmsghdr *)calloc(1,len);	
	cmsg->cmsg_len=len;
	cmsg->cmsg_level = SOL_SOCKET;
        cmsg->cmsg_type = SCM_RIGHTS;
	msg.msg_control=cmsg;
	msg.msg_controllen=len;
	int ret=recvmsg(sfd,&msg,0);
	if(-1==ret)
	{
		perror("recvmsg");
		return;
	}	
	*fd=*(int*)CMSG_DATA(cmsg);
}
