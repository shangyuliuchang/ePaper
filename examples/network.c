#include"includes.h"

char refreshTemp,refreshText=1;
int refreshType,refminx,refminy,refwid,refhei;

size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream){
	int written = fwrite(ptr, size, nmemb, (FILE *)stream);
	return written;
}
void handle(){}
void* refreshNetData(void* data){
    int sockfd, ret, i,cnt;
	time_t last,now;
    struct sockaddr_in servaddr;
    FILE *fp = NULL;
    char Send[200], recv[4096],*search;//,tmpe,*tmp;
	cnt=250*5;
	FILE *recvHead, *recvBody;
	CURL *curlHandle;
    struct tm *p;
	while(!finishFlag){
		sleep(1);
		time(&now);
		p=localtime(&now);
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
    			if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) >= 0){
    			    printf("connect success\n");
					strcpy(Send, "GET /api?version=v6&appid=89759873&appsecret=2IDiUz0x&cityid=101020200 HTTP/1.1\r\nHost: tianqiapi.com\r\n\r\n");
					cnt=0;
					ret = write(sockfd, Send, strlen(Send));
					printf("send length: %d\n", ret);


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
						printf("weather receive done!\n");
					}
					close(sockfd);
				}
    			else
    			    printf("connect failed\n");
			}
			if(now%60==0 && !refreshSentence){
				curlHandle=curl_easy_init();
				curl_easy_setopt(curlHandle, CURLOPT_URL, "https://v1.hitokoto.cn/?encode=text&charset=utf-8");
				curl_easy_setopt(curlHandle, CURLOPT_NOPROGRESS, 1L);
				curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, write_data);
				recvHead=fopen("header.txt","wb");
				recvBody=fopen("body.txt","wb");
				if(recvHead>0 && recvBody>0){
					curl_easy_setopt(curlHandle, CURLOPT_WRITEHEADER, recvHead);
					curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, recvBody);
					curl_easy_perform(curlHandle);
					fclose(recvHead);
					fclose(recvBody);
					curl_easy_cleanup(curlHandle);
					refreshSentence=1;
				}else{
					curl_easy_cleanup(curlHandle);
					fclose(recvHead);
					fclose(recvBody);
				}
			}




				if(!refreshFig && p->tm_sec && 0){
					curlHandle=curl_easy_init();
					curl_easy_setopt(curlHandle, CURLOPT_URL, "https://picsum.photos/1920/1080.jpg");
					//curl_easy_setopt(curlHandle, CURLOPT_URL, "https://img.xjh.me/random_img.php?type=bg&ctype=acg");
					curl_easy_setopt(curlHandle, CURLOPT_NOPROGRESS, 1L);
					curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, write_data);
					recvHead=fopen("header.txt","wb");
					recvBody=fopen("./pic/random.txt","wb");
					if(recvHead>0 && recvBody>0){
						curl_easy_setopt(curlHandle, CURLOPT_WRITEHEADER, recvHead);
						curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, recvBody);
						curl_easy_perform(curlHandle);
						fclose(recvHead);
						fclose(recvBody);
						curl_easy_cleanup(curlHandle);
					}else{
						curl_easy_cleanup(curlHandle);
						fclose(recvHead);
						fclose(recvBody);
					}


					fp=fopen("header.txt","rb");
					while(!feof(fp)){
						fgets(recv, 1000, fp);
						if(strstr(recv, "location: ") == recv){
							search = recv + strlen("location: ");
							search[strlen(search) - 2] = 0;
							printf("url:%s\n",search);
							curlHandle=curl_easy_init();
							curl_easy_setopt(curlHandle, CURLOPT_URL, search);
							curl_easy_setopt(curlHandle, CURLOPT_NOPROGRESS, 10L);
							curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, write_data);
							recvHead=fopen("header2.txt","wb");
							recvBody=fopen("./pic/random.jpg","wb");
							if(recvHead>0 && recvBody>0){
								curl_easy_setopt(curlHandle, CURLOPT_WRITEHEADER, recvHead);
								curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, recvBody);
								curl_easy_perform(curlHandle);
								fclose(recvHead);
								fclose(recvBody);
								curl_easy_cleanup(curlHandle);
								printf("receive figure done!\n");
							}else{
								curl_easy_cleanup(curlHandle);
								fclose(recvHead);
								fclose(recvBody);
							}
							break;
						}
					}
					fclose(fp);
					printf("begin converting\n");
					JpegToBmp("./pic/random.jpg","./pic/random.bmp");
					printf("converting done\n");



//					fp=fopen("./pic/random.txt","r");
//					fgets(recv,1000,fp);
//					fclose(fp);
//					search=strstr(recv,"src=");
//					if(search){
//						search--;
//						search[0]='h';
//						search[1]='t';
//						search[2]='t';
//						search[3]='p';
//						search[4]='s';
//						search[5]=':';
//						//search[4]=':';
//						for(int i=0;i<strlen(search);i++)
//						  if(search[i]=='\"'){
//							  search[i]=0;
//							  break;
//						  }
//
//
//						printf("url:%s\n",search);
//						curlHandle=curl_easy_init();
//						curl_easy_setopt(curlHandle, CURLOPT_URL, search);
//						curl_easy_setopt(curlHandle, CURLOPT_NOPROGRESS, 1L);
//						curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, write_data);
//						recvHead=fopen("header2.txt","wb");
//						recvBody=fopen("./pic/random.jpg","wb");
//						if(recvHead>0 && recvBody>0){
//							curl_easy_setopt(curlHandle, CURLOPT_WRITEHEADER, recvHead);
//							curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, recvBody);
//							curl_easy_perform(curlHandle);
//							fclose(recvHead);
//							fclose(recvBody);
//							curl_easy_cleanup(curlHandle);
//							printf("receive figure done!\n");
//						}else{
//							curl_easy_cleanup(curlHandle);
//							fclose(recvHead);
//							fclose(recvBody);
//						}
//					}
					refreshFig=1;
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
	char buff[1000000];
	FILE *fp;
	int n;
	CURL *curlHandle;
	if((listenfd=socket(AF_INET,SOCK_STREAM,0))==-1){
		printf("\n\n\nCREATE SOCKET FAILED!!!\n\n\n\n");
		return NULL;
	}
	struct timeval tv_out;
	tv_out.tv_sec = 5;
	tv_out.tv_usec = 0;
	setsockopt(listenfd, SOL_SOCKET, SO_RCVTIMEO, &tv_out, sizeof(tv_out));
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
		usleep(1000 * 100);
		if(refreshText) continue;
		if((connfd=accept(listenfd,(struct sockaddr*)&clientaddr,&size))==-1){
			printf("\n\n\nACCEPT FAILED\n\n\n");
			continue;
//			return NULL;
		}
		n=read(connfd,buff,100);
		if(n<100){
			if(buff[0]=='t' && buff[1]=='o' && buff[2]=='t' && buff[4] != 't'){
				refreshType=0;
			}else if(buff[0]=='s' && buff[1]=='e' && buff[2]=='g'){
				refreshType=1;
				sscanf(buff,"seg:%d,%d,%d,%d",&refminx,&refminy,&refwid,&refhei);
			}else{
				refreshType = 2;
			}
		}
		printf("%s\n", buff);
		if(dispTextNum > 1000)
			dispTextNum = 1000;
		else if(dispTextNum < 0)
			dispTextNum = 0;
		char tmpStr[20];
		sprintf(tmpStr, "begin:%d", dispTextNum);
		send(connfd,tmpStr,strlen(tmpStr),0);
		dispTextNum = 0;

		int imgType;
		imgType = 1;
		if(refreshType==0){
			if(buff[4] == 'b'){
				if((fp=fopen("/home/pi/project/ePaper/ePaper/pic/text_back.bmp","wb"))==NULL){
					printf("\n\n\nOPEN FILE FAILED\n\n\n");
					return NULL;
				}
				imgType = 1;
			}else{
				if((fp=fopen("/home/pi/project/ePaper/ePaper/pic/text_back.jpg","wb"))==NULL){
					printf("\n\n\nOPEN FILE FAILED\n\n\n");
					return NULL;
				}
				imgType = 2;
			}
		}else if(refreshType == 1){
			if((fp=fopen("/home/pi/project/ePaper/ePaper/pic/text_back_seg.bmp","wb"))==NULL){
				printf("\n\n\nOPEN FILE FAILED\n\n\n");
				return NULL;
			}
		}else{
			if((fp=fopen("/home/pi/project/ePaper/ePaper/text.txt","wb"))==NULL){
				printf("\n\n\nOPEN FILE FAILED\n\n\n");
				return NULL;
			}
		}
		while(1){
			n=read(connfd,buff,1000000);
			printf("receive length: %d\n", n);
			if(n<=0) break;
			fwrite(buff,1,n,fp);
		}
		close(connfd);
		fclose(fp);

		if(refreshType==2){
			if(buff[0] == '<' && strstr(buff, "<img src") == buff){
				for(int i=strlen("<img src=\"https");i<strlen(buff);i++)
					if(buff[i]=='\"'){
						buff[i]=0;
						break;
					}
				curlHandle=curl_easy_init();
				curl_easy_setopt(curlHandle, CURLOPT_URL, buff+strlen("<img src=\""));
				curl_easy_setopt(curlHandle, CURLOPT_NOPROGRESS, 10L);
				curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, write_data);
				FILE *recvHead=fopen("headerInsert.txt","wb");
				FILE *recvBody=fopen("./pic/insert.jpg","wb");
				if(recvHead>0 && recvBody>0){
					curl_easy_setopt(curlHandle, CURLOPT_WRITEHEADER, recvHead);
					curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, recvBody);
					curl_easy_perform(curlHandle);
					fclose(recvHead);
					fclose(recvBody);
					curl_easy_cleanup(curlHandle);
					printf("receive figure done!\n");
				}else{
					curl_easy_cleanup(curlHandle);
					fclose(recvHead);
					fclose(recvBody);
				}
				JpegToBmp("pic/insert.jpg", "pic/insert.bmp");
			}
		}

		if(imgType == 2)
			JpegToBmp("pic/text_back.jpg", "pic/text_back.bmp");
		refreshText=1;
		printf("\n\n\nSUCCESS\n\n\n");
	}
	return NULL;
}
