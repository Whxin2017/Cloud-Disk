#include"func_linux.h"
int list_file(char *path,file_info pfs[]){
	char str[16];
	memset(str,0,sizeof(str));		
	DIR *dir;
	struct dirent *p;
	struct stat buf;
	memset(&buf,0,sizeof(buf));
	dir=opendir(path);
	if(NULL==dir){
		perror("opendir\n");
		return -1;
	}
	int j=0;
	while((p=readdir(dir))!=NULL){
		if(!strcmp(p->d_name,".") || !strcmp(p->d_name,"..")){
			continue;
		}
		memset(&buf,0,sizeof(buf));
		stat(p->d_name,&buf);
 // printf("mode=%x,link=%hu,uid=%s,gid=%s,size=%lld,time=%s,%s\n",buf.st_mode,buf.st_nlink,getpwuid(buf.st_uid)->pw_name,getgrgid(buf.st_gid)->gr_name,buf.st_size,ctime(&buf.st_mtime),p->d_name);   
//	pfs[j]=(pf_info)calloc(1,sizeof(file_info));
	strcpy(pfs[j].filename,p->d_name);
	pfs[j].size=(int)buf.st_size;	
//	printf("\t-%-8s\t%-4lld",p->d_name,buf.st_size);
//	printf("\t\t");
	int i;
	for(i=0;i<12;i++){
		pfs[j].filedate[i]=*(ctime(&buf.st_mtime)+4+i);
//		printf("%c",*(ctime(&buf.st_mtime)+4+i));
	}
	j++;
//	printf(" ");
//	printf("\n");	    	
}	  
	return j;
}
