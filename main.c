#include "app_dev.h"
int main(int argc, char const *argv[])
{
    app_dev_init();
    app_dev_start();

    while (app_dev_get_is_running())
    {
        log_info("设备正在运行....");
        sleep(1);
    }

    app_dev_deinit();
    log_info("设备已停止运行...");

    return 0;
}
