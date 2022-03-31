#include"includes.h"


int code_convert(char *from_charset, char *to_charset, char *inbuf, int inlen, char *outbuf, int outlen){
	iconv_t cd;
	char **pin=&inbuf;
	char **pout=&outbuf;
	cd=iconv_open(to_charset, from_charset);
	if(cd==0)return -1;
	memset(outbuf, 0, outlen);
	if(iconv(cd, pin, (unsigned int *)&inlen, pout, (unsigned int *)&outlen)==-1) return -1;
	iconv_close(cd);
	return 0;
}
static void Epd_Mode(int mode)
{
    if (mode == 1)
    {
        Paint_SetRotate(ROTATE_0);
        Paint_SetMirroring(MIRROR_HORIZONTAL);
    }
    else
    {
        Paint_SetRotate(ROTATE_0);
        Paint_SetMirroring(MIRROR_NONE);
    }
}
void time2Nums(struct tm *t, char *nums)
{
    nums[0] = t->tm_hour / 10;
    nums[1] = t->tm_hour % 10;
    nums[2] = t->tm_min / 10;
    nums[3] = t->tm_min % 10;
    nums[4] = t->tm_sec / 10;
    nums[5] = t->tm_sec % 10;
}
void SSD(int x, int y, char *type,  char num)
{
    char filename[100];
    sprintf(filename, "/home/pi/project/ePaper/ePaper/pic/%s_%d.bmp",type, num);
    GUI_ReadBmp(filename, x, y);
}
UBYTE Dynamic_Refresh_Example(IT8951_Dev_Info Dev_Info, UDOUBLE Init_Target_Memory_Addr)
{
    UWORD Panel_Width = Dev_Info.Panel_W;
    UWORD Panel_Height = Dev_Info.Panel_H;

    UDOUBLE Imagesize;

    //malloc enough memory for 1bp picture first
    Imagesize = ((Panel_Width * 8 % 8 == 0) ? (Panel_Width * 8 / 8) : (Panel_Width * 8 / 8 + 1)) * Panel_Height;
    Refresh_Frame_Buf = (UBYTE *)malloc(Imagesize);
	
	FILE *fp;
	char recvText[1000],*tmp, convert[1000], shortstr[10];
    int x, y, linecnt;
	int minX,maxX,minY,maxY;
	int timImgH=300,timImgW=256;
    time_t nowTime, lastTime;
    struct tm *p;
    char lastTimeSS[6], nowTimeSS[6],refreshFlag,lastMon[2],lastDay[2],nowMon[2],nowDay[2];
    Paint_NewImage(Refresh_Frame_Buf,Panel_Width, Panel_Height, 0, BLACK);
    Paint_SelectImage(Refresh_Frame_Buf);
	Epd_Mode(epd_mode);
	Paint_SetBitsPerPixel(1);
	Paint_Clear(WHITE);
    EPD_IT8951_1bp_Refresh_WithoutDisplay(Refresh_Frame_Buf, 0, 0, Panel_Width, Panel_Height, A2_Mode, Init_Target_Memory_Addr, true);


    Paint_NewImage(Refresh_Frame_Buf,timImgW, timImgH, 0, BLACK);
    Paint_SelectImage(Refresh_Frame_Buf);
    Epd_Mode(epd_mode);
    Paint_SetBitsPerPixel(1);
    for (int i = 0; i < 4; i++)
    {
        lastTimeSS[i] = 0;
        Paint_Clear(WHITE);
        SSD(0, 0, "tim", lastTimeSS[i]);
        EPD_IT8951_1bp_Refresh_WithoutDisplay(Refresh_Frame_Buf, i * timImgW, 0, timImgW, timImgH, A2_Mode, Init_Target_Memory_Addr, true);
    }
    EPD_IT8951_1bp_Refresh_DisplayOnly(Refresh_Frame_Buf, 0,0,timImgW*4, timImgH, A2_Mode, Init_Target_Memory_Addr, true);


    Paint_NewImage(Refresh_Frame_Buf,425,300, 0, BLACK);
    Paint_SelectImage(Refresh_Frame_Buf);
    Epd_Mode(epd_mode);
    Paint_SetBitsPerPixel(1);
	GUI_ReadBmp("/home/pi/project/ePaper/ePaper/pic/mon_back.bmp",0,0);
	GUI_ReadBmp("/home/pi/project/ePaper/ePaper/pic/wea_qing.bmp",200,160);
	lastMon[0]=0;
	lastMon[1]=0;
	lastDay[0]=0;
	lastDay[1]=0;
    SSD(56, 0, "mon", lastMon[0]);
    SSD(0, 0, "mon", lastMon[1]);
    SSD(56, 200, "mon", lastDay[0]);
    SSD(0, 200, "mon", lastDay[1]);
    SSD(120+56*2, 0, "mon", 0);
    SSD(120+56*3, 0, "mon", 0);
	SSD(120+56*1, 80, "mon",0);
	SSD(120+56*2, 80, "mon",0);
	SSD(120+56*3, 80, "mon",0);
    EPD_IT8951_1bp_Refresh_WithoutDisplay(Refresh_Frame_Buf, 4*timImgW, 0, 424, 300, A2_Mode, Init_Target_Memory_Addr, true);
    EPD_IT8951_1bp_Refresh_DisplayOnly(Refresh_Frame_Buf, 4*timImgW, 0,424, 300, A2_Mode, Init_Target_Memory_Addr, true);

    while (!finishFlag)
    {

        time(&nowTime);
        p = localtime(&nowTime);
		sleep(1);

        if (nowTime != lastTime)
        {
            time2Nums(p, nowTimeSS);
			refreshFlag=0;
            for (int i = 0; i < 4; i++)
            {
                if (lastTimeSS[i] != nowTimeSS[i])
                {
					if(refreshFlag==0){
    					Paint_NewImage(Refresh_Frame_Buf,timImgW, timImgH, 0, BLACK);
    					Paint_SelectImage(Refresh_Frame_Buf);
    					Epd_Mode(epd_mode);
    					Paint_SetBitsPerPixel(1);
					}
                    Paint_Clear(WHITE);
                    x = 0;
                    y = 0;
					nowDay[0]=p->tm_mday%10;
					nowDay[1]=p->tm_mday/10;
					nowMon[0]=(p->tm_mon+1)%10;
					nowMon[1]=(p->tm_mon+1)/10;

                    SSD(x, y, "tim", nowTimeSS[i]);
        			EPD_IT8951_1bp_Refresh_WithoutDisplay(Refresh_Frame_Buf, i * timImgW, 0, timImgW, timImgH, A2_Mode, Init_Target_Memory_Addr, true);
                    lastTimeSS[i] = nowTimeSS[i];
					refreshFlag=1;
                }
            }
        	if(refreshFlag) {
				EPD_IT8951_1bp_Refresh_DisplayOnly(Refresh_Frame_Buf, 0,0,timImgW*4, timImgH, A2_Mode, Init_Target_Memory_Addr, true);
				printf("REFRESH TIME\n");
			}

			nowDay[0]=p->tm_mday%10;
			nowDay[1]=p->tm_mday/10;
			nowMon[0]=(p->tm_mon+1)%10;
			nowMon[1]=(p->tm_mon+1)/10;
			if(lastDay[0]!=nowDay[0] || lastDay[1]!=nowDay[1]||lastMon[0]!=nowMon[0]||lastMon[1]!=nowMon[1]||refreshTemp){
    			Paint_NewImage(Refresh_Frame_Buf,425,300, 0, BLACK);
    			Paint_SelectImage(Refresh_Frame_Buf);
    			Epd_Mode(epd_mode);
    			Paint_SetBitsPerPixel(1);
				GUI_ReadBmp("/home/pi/project/ePaper/ePaper/pic/mon_back.bmp",0,0);
				switch(weather){
					case yin:GUI_ReadBmp("/home/pi/project/ePaper/ePaper/pic/wea_yin.bmp",200,160);
						break;
					case duoyun:GUI_ReadBmp("/home/pi/project/ePaper/ePaper/pic/wea_duoyun.bmp",200,160);
						break;
					case qing:GUI_ReadBmp("/home/pi/project/ePaper/ePaper/pic/wea_qing.bmp",200,160);
						break;
					case xiaoyu:GUI_ReadBmp("/home/pi/project/ePaper/ePaper/pic/wea_xiaoyu.bmp",200,160);
						break;
					case zhongyu:GUI_ReadBmp("/home/pi/project/ePaper/ePaper/pic/wea_zhongyu.bmp",200,160);
						break;
					case dayu:GUI_ReadBmp("/home/pi/project/ePaper/ePaper/pic/wea_dayu.bmp",200,160);
						break;
				}
			
	
				for(int i=0;i<2;i++){
						SSD(56*(1-i), 200, "mon", nowDay[i]);
						lastDay[i]=nowDay[i];
				}
				for(int i=0;i<2;i++){
						SSD(56*(1-i), 0, "mon", nowMon[i]);
						lastMon[i]=nowMon[i];
				}
				SSD(120+56*2,0,"mon",temperature/10);
				SSD(120+56*3,0,"mon",temperature%10);
				SSD(120+56*1,80,"mon",humidity/100);
				SSD(120+56*2,80,"mon",(humidity/10)%10);
				SSD(120+56*3,80,"mon",humidity%10);
				refreshTemp=0;
	    		EPD_IT8951_1bp_Refresh(Refresh_Frame_Buf, 4*timImgW, 0, 424, 300, A2_Mode, Init_Target_Memory_Addr, true);
			}


            lastTime = nowTime;

        }
		if(refreshText){
			if(refreshType==0){
				fp=fopen("/home/pi/project/ePaper/ePaper/recvFromHost.txt","r");
				if(fp){
					Paint_NewImage(Refresh_Frame_Buf,1344,1072-timImgH, 0, WHITE);
					Paint_SelectImage(Refresh_Frame_Buf);
					Epd_Mode(epd_mode);
					Paint_SetBitsPerPixel(4);
					memset(recvText,0,sizeof(recvText));
					tmp=recvText;
					while(!feof(fp))*(tmp++)=fgetc(fp); 
					printf("%s\n",recvText);
					fclose(fp);

					GUI_ReadBmp2("/home/pi/project/ePaper/ePaper/pic/text_back.bmp", 0, 0, 1344, 772);
					Paint_DrawString_EN(0,0 , recvText, &Font20, 0x00, 0xFF);
					EPD_IT8951_4bp_Refresh(Refresh_Frame_Buf, 0, timImgH, 1344, 1072-timImgH, A2_Mode, Init_Target_Memory_Addr, true);
				}
				refreshText=0;
			}else if(refreshType==1){
				fp=fopen("/home/pi/project/ePaper/ePaper/recvFromHost.txt","r");
				if(fp){
					Paint_NewImage(Refresh_Frame_Buf,refwid,refhei, 0, WHITE);
					Paint_SelectImage(Refresh_Frame_Buf);
					Epd_Mode(epd_mode);
					Paint_SetBitsPerPixel(1);
					memset(recvText,0,sizeof(recvText));
					tmp=recvText;
					while(!feof(fp))*(tmp++)=fgetc(fp); 
					printf("%s\n",recvText);
					fclose(fp);

					GUI_ReadBmp("/home/pi/project/ePaper/ePaper/pic/text_back_seg.bmp",0,0);
					EPD_IT8951_1bp_Refresh(Refresh_Frame_Buf, refminx,refminy,refwid,refhei, A2_Mode, Init_Target_Memory_Addr, true);
				}
				refreshText=0;
			}

		}
		if(refreshTrace&&pointNum){
			refreshTrace=0;
   			Paint_NewImage(Refresh_Frame_Buf,16, 16,0, WHITE);
    		Paint_SelectImage(Refresh_Frame_Buf);
    		Epd_Mode(epd_mode);
    		Paint_SetBitsPerPixel(1);
			Paint_Clear(BLACK);
			minX=((pointList[0][0]-7)/16)*16;
			maxX=((pointList[0][0]+7)/16+1)*16-1;
			minY=pointList[0][1];
			maxY=pointList[0][1];
			for(int i=0;i<pointNum;i++){
				if(((pointList[i][0]-7)/16)*16<minX)minX=((pointList[i][0]-7)/16)*16;
				if(((pointList[i][0]+7)/16+1)*16-1>maxX)maxX=((pointList[i][0]+7)/16+1)*16-1;
				if(pointList[i][1]-7<minY)minY=pointList[i][1]-7;
				if(pointList[i][1]+7>maxY)maxY=pointList[i][1]+7;
			}
			for(int i=0;i<pointNum;i++){
		    	EPD_IT8951_1bp_Refresh_WithoutDisplay(Refresh_Frame_Buf, (pointList[i][0]/16)*16, timImgH + pointList[i][1], 16, 16, A2_Mode, Init_Target_Memory_Addr, true);
			}
	    	EPD_IT8951_1bp_Refresh_DisplayOnly(Refresh_Frame_Buf, minX, timImgH+minY, maxX-minX+1, maxY-minY+1, A2_Mode, Init_Target_Memory_Addr, true);
			pointNum=0;
		}
		if(refreshSentence){
			refreshSentence=0;
			fp=fopen("body.txt","r");
			if(fp>0){
				fgets(convert, 1000, fp);
				//fgets(recvText, 1000, fp);
				fclose(fp);
				//code_convert("utf-8", "gb2312", recvText, strlen(recvText), convert, 1000);
				printf("%s\n", convert);

				Paint_NewImage(Refresh_Frame_Buf,48 * 2, 700,0, WHITE);
				Paint_SelectImage(Refresh_Frame_Buf);
				Epd_Mode(epd_mode);
				Paint_SetBitsPerPixel(1);
				Paint_Clear(WHITE);
				linecnt=0;

				for(int i=0;i<strlen(convert);i++){
					if(convert[i]>=160){
						shortstr[0]=convert[i];
						shortstr[1]=convert[i+1];
						shortstr[2]=0;
						i++;
					}else{
						shortstr[0]=convert[i];
						shortstr[1]=0;
					}
					Paint_DrawString_CN_Try((1 - (linecnt*24*2)/650)*48,(linecnt*24*2)%650, shortstr, &Font24CN, 0x00, 0xFF);
					linecnt++;
				}
				EPD_IT8951_1bp_Refresh(Refresh_Frame_Buf, 1344, 300, 48 * 2, 700, A2_Mode, Init_Target_Memory_Addr, true);
			}
		}
			if(refreshFig && p->tm_min%2==0){
				printf("begin converting\n");
				JpegToBmp("./pic/random.jpg","./pic/random.bmp");
				printf("converting done\n");
				Paint_NewImage(Refresh_Frame_Buf,1344, 772,0, WHITE);
				Paint_SelectImage(Refresh_Frame_Buf);
				Epd_Mode(epd_mode);
				Paint_SetBitsPerPixel(4);
				Paint_Clear(WHITE);
				GUI_ReadBmp2("./pic/random.bmp",0,0, 1344, 772);
				EPD_IT8951_4bp_Refresh(Refresh_Frame_Buf, 0,300,1344, 772, GC16_Mode, Init_Target_Memory_Addr, true);
				refreshFig=0;
			}
    }
    if (Refresh_Frame_Buf != NULL)
    {
        free(Refresh_Frame_Buf);
        Refresh_Frame_Buf = NULL;
    }

    return 0;
}
