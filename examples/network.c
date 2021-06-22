#include"includes.h"

char refreshTemp,refreshText=1;
int refreshType,refminx,refminy,refwid,refhei;

void handle(){}
void* refreshNetData(void* data){
    int sockfd, ret, i,cnt;
	time_t last,now;
    struct sockaddr_in servaddr;
    FILE *fp = NULL;
    char send[200], recv[4096],*search;//,tmpe,*tmp;
	cnt=250*5;
	while(!finishFlag){
		sleep(1);
		time(&now);
		if(last!=now){
			last=now;
			if(++cnt>5*240){
				cnt=0;
    			sockfd = socket(AF_INET, SOCK_STREAM, 0);
    			if (sockfd >= 0)
    			    printf("creat success\n");
    			else
    			    printf("creat failed\n");
    			memset(&servaddr, 0, sizeof(servaddr));
    			servaddr.sin_family = AF_INET;
    			servaddr.sin_port = htons(80);
    			if (inet_pton(AF_INET, "120.27.2.251", &servaddr.sin_addr) > 0)
    			    printf("create connection success\n");
    			else
    			    printf("create connection failed\n");
    			if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) >= 0)
    			    printf("connect success\n");
    			else
    			    printf("connect failed\n");
    			strcpy(send, "GET /api?version=v6&appid=89759873&appsecret=2IDiUz0x&cityid=101020200 HTTP/1.1\r\nHost: tianqiapi.com\r\n\r\n");
				cnt=0;
    			ret = write(sockfd, send, strlen(send));
    			printf("send length: %d", ret);



				memset(recv,0,sizeof(recv));
    			i = read(sockfd, recv, 4096);
    			printf("%d bytes of data have been received\n", i);
    			if (i > 0)
    			{
    			    fp = fopen("recv.txt", "w");
					search=strstr(recv,"\"tem\":")+7;
    			    fprintf(fp, "%s", recv);
					sscanf(search,"%d\"",&temperature);
					printf("temperature: %d\n",temperature);
					search=strstr(recv,"\"humidity\":\"")+12;
					sscanf(search,"%d",&humidity);
					search=strstr(recv,"\"wea\"")+7;
					if(search[0]=='\\' && search[1]=='u' && search[2]=='9') weather=yin;
					else if(search[0]=='\\' && search[1]=='u' && search[2]=='5'){
						if(search[3]=='9'){
							if(search[4]=='1')weather=duoyun;
							else if(search[4]=='2')weather=dayu;
						}
						else if(search[3]=='c') weather=xiaoyu;
					}
					else if(search[0]=='\\' && search[1]=='u' && search[2]=='4')weather=zhongyu;
					else if(search[0]=='\\' && search[1]=='u' && search[2]=='6') weather=qing;
    			    fclose(fp);
					refreshTemp=1;
    			}
				close(sockfd);
			}

		//	signal(SIGALRM,handle);
		//	alarm(2); 
		//	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
		//	if (sockfd < 0)
		//		goto skip;
	    //	memset(&servaddr, 0, sizeof(servaddr));
	    //	servaddr.sin_family = AF_INET;
	    //	servaddr.sin_port = htons(1500);
	    //	if (inet_pton(AF_INET, "192.168.1.100", &servaddr.sin_addr) <= 0)
		//		goto skip;
	    //	if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
		//		goto skip;
	    //	strcpy(send, "want data");
		//	cnt=0;
	    //	ret = write(sockfd, send, strlen(send));
	
	
	
		//	memset(recv,0,sizeof(recv));
	    //	i = read(sockfd, recv, 4096);
	    //	if (i > 0)
	    //	{
		//		tmpe=recv[6];
		//		recv[6]=0;
		//		if(strcmp(recv,"point:")==0){
		//			refreshTrace=1;
		//			recv[6]=tmpe;
		//			for(tmp=recv;tmp-recv<strlen(recv);tmp++){
		//				if(*tmp=='p'){
		//					sscanf(tmp,"point:%d,%d\n",&newpointX,&newpointY);
		//					printf("point:%d,%d\n",newpointX,newpointY);
		//					pointList[pointNum][0]=newpointX;
		//					pointList[pointNum][1]=newpointY;
		//					pointNum++;
		//				}
		//			}
		//		}else{
		//			recv[6]=tmpe;
	    //	   		 fp = fopen("recvFromHost.txt", "w");
		//	   		 refreshText=1;
	    //	   		 fprintf(fp, "%s", recv);
	    //	   		 fclose(fp);
		//		}
	    //	}
//skip:
		//	signal(SIGALRM,SIG_IGN);
		//	close(sockfd);

		}
    }
	return NULL;
}
void *recvPic(void* arg){
	int listenfd, connfd;
	struct sockaddr_in servaddr;
	char buff[4096];
	FILE *fp;
	int n;
	if((listenfd=socket(AF_INET,SOCK_STREAM,0))==-1){
		printf("\n\n\nCREATE SOCKET FAILED!!!\n\n\n\n");
		return NULL;
	}
	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	servaddr.sin_port=htons(1500);
	if(bind(listenfd,(struct sockaddr*)&servaddr,sizeof(servaddr))==-1){
		printf("\n\n\nBIND FAILED!!!\n\n\n");
		return NULL;
	}
	if(listen(listenfd,10)==-1){
		printf("\n\n\nLISTEN FAILED!!!\n\n\n");
		return NULL;
	}
	struct sockaddr_in clientaddr;
	socklen_t size=sizeof(clientaddr);
	while(!finishFlag){
		sleep(1);
		if(refreshText) continue;
		if((connfd=accept(listenfd,(struct sockaddr*)&clientaddr,&size))==-1){
			printf("\n\n\nACCEPT FAILED\n\n\n");
			return NULL;
		}
		n=read(connfd,buff,100);
		if(n<100){
			if(buff[0]=='t' && buff[1]=='o' && buff[2]=='t'){
				refreshType=0;
			}else if(buff[0]=='s' && buff[1]=='e' && buff[2]=='g'){
				refreshType=1;
				sscanf(buff,"seg:%d,%d,%d,%d",&refminx,&refminy,&refwid,&refhei);
			}
		}
		send(connfd,"begin",5,0);

		if(refreshType==0){
			if((fp=fopen("/home/pi/Documents/project/ePaper/myPro/pic/text_back.bmp","wb"))==NULL){
				printf("\n\n\nOPEN FILE FAILED\n\n\n");
				return NULL;
			}
		}else{
			if((fp=fopen("/home/pi/Documents/project/ePaper/myPro/pic/text_back_seg.bmp","wb"))==NULL){
				printf("\n\n\nOPEN FILE FAILED\n\n\n");
				return NULL;
			}
		}
		while(1){
			n=read(connfd,buff,4096);
			if(n<=0) break;
			fwrite(buff,1,n,fp);
		}
		close(connfd);
		fclose(fp);
		refreshText=1;
		printf("\n\n\nSUCCESS\n\n\n");
	}
	return NULL;
}