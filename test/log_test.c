#include "log/log.h"
int main(int argc, char const *argv[])
{
    log_set_level(LOG_INFO);
    log_trace("hello world");
    log_debug("hello world");
    log_info("hello world");
    log_warn("hello world");
    log_error("hello world");
    log_fatal("hello world");

    return 0;
}
/*
日志级别:
    0: trace   verbose
    1: debug
    2: info
    3: warn
    4: error
    5: fatal


*/
