#define GRAM_SIZE 8456
#include "tool.h"
#define PAGE_COUNT GRAM_SIZE/2048+1
typedef  void (*iapfun)(void);				//定义一个函数类型的参数.
#define  FLASH_APP_ADDR		0x08006000  	//第一个应用程序起始地址(存放在FLASH)

void iap_load_app(u32 appxaddr);

