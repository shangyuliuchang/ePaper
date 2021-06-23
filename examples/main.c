#include"includes.h"



#define Enhance false

#define USE_Factory_Test false

#define USE_Normal_Demo true

#define USE_Touch_Panel false
UBYTE *Refresh_Frame_Buf = NULL;

UBYTE *Panel_Frame_Buf = NULL;
UBYTE *Panel_Area_Frame_Buf = NULL;

bool Four_Byte_Align = false;

UWORD VCOM = 2510;

	int temperature=0;
pthread_t th,th_net;
char finishFlag = 0;
IT8951_Dev_Info Dev_Info;
UWORD Panel_Width;
UWORD Panel_Height;
UDOUBLE Init_Target_Memory_Addr;
int epd_mode = 0; //0: no rotate, no mirror
int newpointX,newpointY;
int refreshTrace;
int pointList[1000][2],pointNum;
int humidity;
int weather;
int refreshSentence=1;

void *thread(void *arg)
{
    Dynamic_Refresh_Example(Dev_Info, Init_Target_Memory_Addr);
	return NULL;
}
//1: no rotate, horizontal mirror

void Handler(int signo)
{
    int *tmp = NULL;
    finishFlag = 1;
    pthread_join(th, (void **)&tmp);
    pthread_join(th_net, (void **)&tmp);
    Debug("\r\nHandler:exit\r\n");
    if (Refresh_Frame_Buf != NULL)
    {
        free(Refresh_Frame_Buf);
        Debug("free Refresh_Frame_Buf\r\n");
        Refresh_Frame_Buf = NULL;
    }
    if (Panel_Frame_Buf != NULL)
    {
        free(Panel_Frame_Buf);
        Debug("free Panel_Frame_Buf\r\n");
        Panel_Frame_Buf = NULL;
    }
    if (Panel_Area_Frame_Buf != NULL)
    {
        free(Panel_Area_Frame_Buf);
        Debug("free Panel_Area_Frame_Buf\r\n");
        Panel_Area_Frame_Buf = NULL;
    }
    if (bmp_src_buf != NULL)
    {
        free(bmp_src_buf);
        Debug("free bmp_src_buf\r\n");
        bmp_src_buf = NULL;
    }
    if (bmp_dst_buf != NULL)
    {
        free(bmp_dst_buf);
        Debug("free bmp_dst_buf\r\n");
        bmp_dst_buf = NULL;
    }
    Debug("Going to sleep\r\n");
    EPD_IT8951_Clear_Refresh(Dev_Info, Init_Target_Memory_Addr, A2_Mode);
    EPD_IT8951_Clear_Refresh(Dev_Info, Init_Target_Memory_Addr, GC16_Mode);
    EPD_IT8951_Sleep();
    DEV_Module_Exit();
    exit(0);
}

int main(int argc, char *argv[])
{
    //Exception handling:ctrl + c
    signal(SIGINT, Handler);

    if (DEV_Module_Init() != 0)
    {
        return -1;
    }

    double temp;
    temp = -2.28;
    VCOM = (UWORD)(fabs(temp) * 1000);
    epd_mode = 0;
    Dev_Info = EPD_IT8951_Init(VCOM);

#if (Enhance)
    Debug("Attention! Enhanced driving ability, only used when the screen is blurred\r\n");
    Enhance_Driving_Capability();
#endif

    //get some important info from Dev_Info structure
    Panel_Width = Dev_Info.Panel_W;
    Panel_Height = Dev_Info.Panel_H;
    Init_Target_Memory_Addr = Dev_Info.Memory_Addr_L | (Dev_Info.Memory_Addr_H << 16);
    char *LUT_Version = (char *)Dev_Info.LUT_Version;
    if (strcmp(LUT_Version, "M641") == 0)
    {
        //6inch e-Paper HAT(800,600), 6inch HD e-Paper HAT(1448,1072), 6inch HD touch e-Paper HAT(1448,1072)
        A2_Mode = 4;
        Four_Byte_Align = true;
    }
    else if (strcmp(LUT_Version, "M841_TFAB512") == 0)
    {
        //Another firmware version for 6inch HD e-Paper HAT(1448,1072), 6inch HD touch e-Paper HAT(1448,1072)
        A2_Mode = 6;
        Four_Byte_Align = true;
    }
    else if (strcmp(LUT_Version, "M841") == 0)
    {
        //9.7inch e-Paper HAT(1200,825)
        A2_Mode = 6;
    }
    else if (strcmp(LUT_Version, "M841_TFA2812") == 0)
    {
        //7.8inch e-Paper HAT(1872,1404)
        A2_Mode = 6;
    }
    else if (strcmp(LUT_Version, "M841_TFA5210") == 0)
    {
        //10.3inch e-Paper HAT(1872,1404)
        A2_Mode = 6;
    }
    else
    {
        //default set to 6 as A2 Mode
        A2_Mode = 6;
    }
    Debug("A2 Mode:%d\r\n", A2_Mode);

    EPD_IT8951_Clear_Refresh(Dev_Info, Init_Target_Memory_Addr, INIT_Mode);

#if (USE_Factory_Test)
    Factory_Test_Only(Dev_Info, Init_Target_Memory_Addr);
#endif

#if (USE_Normal_Demo)
    pthread_create(&th, NULL, thread, NULL);
    pthread_create(&th_net, NULL, recvPic, NULL);
	refreshNetData(NULL);

    printf("start socket\n");

    //while (1)
    //{
    //    printf("hello\n");
    //    sleep(5000);
    //}
    //Dynamic_Refresh_Example(Dev_Info,Init_Target_Memory_Addr);
    //EPD_IT8951_Clear_Refresh(Dev_Info, Init_Target_Memory_Addr, A2_Mode);
    //EPD_IT8951_Clear_Refresh(Dev_Info, Init_Target_Memory_Addr, GC16_Mode);
#endif

#if (USE_Touch_Panel)
    //show a simple demo for hand-painted tablet, only support for <6inch HD touch e-Paper> at present
    EPD_IT8951_Clear_Refresh(Dev_Info, Init_Target_Memory_Addr, INIT_Mode);
    TouchPanel_ePaper_Example(Panel_Width, Panel_Height, Init_Target_Memory_Addr);
#endif

    //We recommended refresh the panel to white color before storing in the warehouse.
    EPD_IT8951_Clear_Refresh(Dev_Info, Init_Target_Memory_Addr, INIT_Mode);

    //EPD_IT8951_Standby();
    EPD_IT8951_Sleep();

    //In case RPI is transmitting image in no hold mode, which requires at most 10s
    DEV_Delay_ms(5000);

    DEV_Module_Exit();
    return 0;
}
