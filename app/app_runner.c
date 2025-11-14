#include "app_runner.h"
#include "app_dev.h"
#include "signal.h"

void signle_handler(int signum)
{
    log_info("收到信号...");
    app_dev_set_is_running(false);
}

int app_run(void)
{
    signal(SIGINT,signle_handler);
    signal(SIGTERM, signle_handler);


    app_dev_init();
    app_dev_start();

    while (app_dev_get_is_running())
    {
        log_info("设备正在运行....");
        sleep(1);
    }

    app_dev_deinit();
    log_info("设备已停止运行...");
    log_info("程序已退出...");
    return 0;
}
