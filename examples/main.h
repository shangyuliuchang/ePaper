#ifndef __MAIN_H__
#define __MAIN_H__
#include "includes.h"
extern int temperature;
extern UBYTE *Refresh_Frame_Buf;
extern UBYTE *Panel_Frame_Buf;
extern UBYTE *Panel_Area_Frame_Buf;
extern bool Four_Byte_Align;
extern UWORD VCOM;
extern 	int temperature;
extern pthread_t th,th_net;
extern char finishFlag;
extern IT8951_Dev_Info Dev_Info;
extern UWORD Panel_Width;
extern UWORD Panel_Height;
extern UDOUBLE Init_Target_Memory_Addr;
extern int epd_mode; //0: no rotate, no mirror
extern int newpointX,newpointY;
extern int refreshTrace;
extern int pointList[1000][2],pointNum;
extern int humidity;
extern int weather;
#endif
