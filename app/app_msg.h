#ifndef __APP_MSG_H__
#define __APP_MSG_H__

#include "app_common.h"
//用来解析Json数据
typedef struct {
    char *connetType;//Json字符串的连接类型
    int motorId;//Json字符串的电机ID
    char *action;//Json字符串的行为
    int16_t motorSpeed;//Json字符串的电机速度
    char *status;//Json字符串的状态
} msg_t;

GATE_STATE_T json_2_msg(char *json, msg_t *msg);

GATE_STATE_T msg_2_json(msg_t *msg, char json[]);

#endif /* __APP_MSG_H__ */