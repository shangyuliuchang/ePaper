#ifndef __NETWORK_H__
#define __NETWORK_H__
#include"includes.h"
void* refreshNetData(void* data);
void *recvPic(void *args);
extern char refreshTemp;
extern char refreshText;
extern int refreshType,refminx,refminy,refwid,refhei;
size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream);
#endif
