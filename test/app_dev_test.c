#include "app_dev.h"

int main(int argc, char const *argv[])
{
    app_dev_init();
    app_dev_start();

    while (app_dev_get_is_running())
    {
        log_info("running");
        sleep(1);
    }
    app_dev_deinit();
    log_info("app_dev_deinit");
    return 0;
}
