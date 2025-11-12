#ifndef __APP_DEV_H__
#define __APP_DEV_H__

#include "app_common.h"
#include "app_buffer.h"
#include "app_mqtt.h"
#include "app_pool.h"

GATE_STATE_T app_dev_init(void);
void app_dev_deinit(void);
void app_dev_start(void);
bool app_dev_get_is_running(void);
void app_dev_stop(void);
void app_dev_set_is_running(bool is_running);



#endif /* __APP_DEV_H__ */