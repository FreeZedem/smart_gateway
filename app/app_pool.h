#ifndef __APP_POOL_H__
#define __APP_POOL_H__

#include "app_common.h"
#include "pthread.h"
#include "mqueue.h"

GATE_STATE_T app_pool_init(int pool_size);

void app_pool_deinit(void);

GATE_STATE_T app_pool_add_task(void (*task)(void *), void *data);

#endif /* __APP_POOL_H__ */
