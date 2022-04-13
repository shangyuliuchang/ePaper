#include"includes.h"
void JpegToBmp( char *JpegName,char * NewBmpName)//将读取jpeg中的数据并保存为BMP格式的文件,生成的都是24位真彩图，不需要调色板
{
       
          FILE *fNewBmp;//存储生成的Bmp文件的句柄
        	FILE *fJpeg;//目标jpeg文件的句柄
        	unsigned char *data;   //存放解压后的数据
        	unsigned char *jpgbuf;      //存放解压后一行图像数据
        	BMPFILEHEADER header;//存储文件头
        	memset(&header, 0, sizeof(header));//文件头赋初始值
        	BMPINFOHEADER infoheader;//位图信息头
        	memset(&infoheader, 0, sizeof(infoheader));//赋初始值
 
        	int row_stride;        //定义每行的字节数
        	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
        cinfo.err = jpeg_std_error(&jerr);
        jpeg_create_decompress(&cinfo);//声明并初始化解压缩对象
 
        	fJpeg=fopen(JpegName,"rb");//二进制模式读取jpeg文件
       
        if(fJpeg==NULL) //二进制模式读取
        {
              printf("error: cannot open  the file\n");
              return ;
        }//打开jpeg图片
		printf("open success\n");
 
        jpeg_stdio_src(&cinfo, fJpeg);//指定解压对象的源文件
        jpeg_read_header(&cinfo, TRUE);//读取文件信息，将图像的缺省的信息填充到cinfo结构中比便程序使用
		cinfo.out_color_space=JCS_RGB;
        jpeg_start_decompress(&cinfo);//开始接压缩
 
        data=(unsigned char *)malloc(((cinfo.output_width* cinfo.output_components-1)/4+1)*4*cinfo.output_height);//动态分配数据存储内存
        memset(data,0,((cinfo.output_width* cinfo.output_components-1)/4+1)*4*cinfo.output_height);//设置图像数据初值为0
       jpgbuf = (unsigned char *) malloc(cinfo.output_width *cinfo.output_components);//动态分配缓存内存
        memset(jpgbuf,0,cinfo.output_width* cinfo.output_components);//设置图像数据初值为0
		printf("malloc success\n");
 
        row_stride = cinfo.output_width * cinfo.output_components; //计算每行所需的空间，字节为单位
//		if(cinfo.output_components==3){
			while (cinfo.output_scanline < cinfo.output_height)
			{
				  int line=cinfo.output_scanline;//当前行数
	 
				  (void) jpeg_read_scanlines(&cinfo, &jpgbuf, 1);//执行该操作读取第line行数据，cinfo.output_scanline将加一，指向下一个要扫描的行
	 
				  for(int i=0;i< cinfo.output_width;i++)//循环将存储在jpgbuf缓存区的数据放入data中
						  {      
								  data[line*row_stride+i*cinfo.output_components+0]=jpgbuf[i*3];
								  data[line*row_stride+i*cinfo.output_components+1]=jpgbuf[i*3+1];
								  data[line*row_stride+i*cinfo.output_components+2]=jpgbuf[i*3+2];
						   }
	 
			}
//		}else{
//			while (cinfo.output_scanline < cinfo.output_height)
//			{
//				  int line=cinfo.output_scanline;//当前行数
//	 
//				  (void) jpeg_read_scanlines(&cinfo, &jpgbuf, 1);//执行该操作读取第line行数据，cinfo.output_scanline将加一，指向下一个要扫描的行
//	 
//				  for(int i=0;i< cinfo.output_width;i++)//循环将存储在jpgbuf缓存区的数据放入data中
//						  {      
//								  data[line*row_stride+i*cinfo.output_components+0]=jpgbuf[i];
//								  data[line*row_stride+i*cinfo.output_components+1]=jpgbuf[i];
//								  data[line*row_stride+i*cinfo.output_components+2]=jpgbuf[i];
//						   }
//	 
//			}
//		}
       
        free(jpgbuf);
//填充文件头信息
        	header.bType= 0x4D42;//设置为“BM”
        	header.bSize=sizeof(BMPFILEHEADER) + sizeof(BMPINFOHEADER)+((cinfo.output_width*3-1)/4+1)*4*cinfo.output_height;
        header.bOffset=sizeof(BMPFILEHEADER) + sizeof(BMPINFOHEADER);
 
 
//填充位图信息头
        	infoheader.biInfoSize=sizeof(infoheader);//位图信息头结构的长度，为40
        	infoheader.biBitCount=24;//设置为真彩图
        	infoheader.biHeight=cinfo.image_height;
        infoheader.biWidth=cinfo.image_width;
        infoheader.biCompression=0;//一般默认为BI_RGB格式表示不压缩
        	infoheader.biPlanes=1;//表示不用考虑
        	infoheader.biXPelsPerMeter=0;
        infoheader.biYPelsPerMeter=0;
        infoheader.biClrUsed=0;//在MSDN中队RGB位图定义可知默认值为0
		infoheader.bimpImageSize=((infoheader.biWidth*3-1)/4+1)*4*infoheader.biHeight;
		printf("setting success\n");
 
//将文件 头和位图信息头写入文件中
        fNewBmp=fopen(NewBmpName,"wb");//二进制写的形式打开文件
        if(!fNewBmp)
        {
              return;
        }
        fwrite(&header,sizeof(header),1,fNewBmp);//写入文件头
        	fwrite(&infoheader,sizeof(infoheader),1,fNewBmp);//写入位图信息头
			printf("info write success\n");
 
//由于BMP扫描方式为从上到下，从左到右，window扫描要满足4字节的倍数。读data数据进行修改，存储为BGR顺序
        int tmpRow=((infoheader.biWidth*3+3)>>2)<<2;//调整没行的字节数，使其是4的倍数
       
        jpgbuf = (unsigned char *) malloc(tmpRow);//动态分配缓存内存
        memset(jpgbuf,0,tmpRow);//为缓存内存设置初值
 
        for(int i=0;i<infoheader.biHeight;i++)
        {
              for(int j=0;j<infoheader.biWidth;j++)//将从下到上从左到右读取数据，并将RGB顺寻装换为BGR顺序
              {
              
                       jpgbuf[3*j+0]=data[(infoheader.biHeight-1-i)*row_stride+3*j+2];
                       jpgbuf[3*j+1]=data[(infoheader.biHeight-1-i)*row_stride+3*j+1];
                       jpgbuf[3*j+2]=data[(infoheader.biHeight-1-i)*row_stride+3*j+0];
              }
               fwrite(jpgbuf,sizeof(unsigned char),tmpRow,fNewBmp);//每行的数据写入文件中
        }
		printf("write all success\n");
        jpeg_finish_decompress(&cinfo);//完成解压过程
        jpeg_destroy_decompress(&cinfo);//释放cinfo
        fclose(fJpeg);
        free(data);
        free(jpgbuf);
        fclose(fNewBmp);
        return;
}
