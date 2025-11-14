#include "app_runner.h"
#include "app_common.h"

int main(int argc, char const *argv[])
{
    if (argc == 1)
    {
        log_info("请输入参数：app|ota|daemon");
    }else if (strcmp(argv[1], "app") == 0)
    {
        app_run();
    }
    else if (strcmp(argv[1], "ota") == 0)
    {
        //TODO: 启动OTA服务
    }else if (strcmp(argv[1], "daemon") == 0)
    {
        //TODO: 启动守护进程
    }else{
        log_info("请输入参数:app|ota|daemon");
    }
    
    return 0;
}