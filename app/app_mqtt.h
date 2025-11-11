#ifndef __APP_MQTT_H__
#define __APP_MQTT_H__
#include "app_common.h"//常用的c语言头文件
#include "MQTTClient.h"

GATE_STATE_T app_mqtt_init(void);
void app_mqtt_deinit(void);
GATE_STATE_T app_mqtt_send(void* data,int len);

void app_mqtt_register_callback(void (*recv_callback)(void* data,int len));
#endif 