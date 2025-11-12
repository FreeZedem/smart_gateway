#ifndef __APP_BUFFER_H__
#define __APP_BUFFER_H__
#include "app_common.h"

typedef void* buffer_handle_t;

buffer_handle_t *app_buffer_init(int capacity);
void app_buffer_deinit(buffer_handle_t *buffer_handle);
GATE_STATE_T app_buffer_write(buffer_handle_t *buffer_handle, char *data, uint8_t len);
int app_buffer_read(buffer_handle_t *buffer_handle, char *data, int data_capacity);

#endif /* __APP_BUFFER_H__ */