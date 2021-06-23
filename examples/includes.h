#ifndef __INCLUDE_H__
#define __INCLUDE_H__

#define yin (0)
#define duoyun (1)
#define qing (2)
#define xiaoyu (3)
#define zhongyu (4)
#define dayu (5)

#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <curl/curl.h>

#include "../lib/e-Paper/EPD_IT8951.h"
#include "../lib/GUI/GUI_Paint.h"
#include "../lib/GUI/GUI_BMPfile.h"
#include "../lib/Config/Debug.h"

#include "../lib/Config/DEV_Config.h"

#include <math.h>

#include <stdlib.h> //exit()
#include <signal.h> //signal()
#include"../examples/network.h"
#include"../examples/main.h"
#include"../examples/display.h"
#include <iconv.h>



#endif
