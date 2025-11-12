#include "app_msg.h"
#include "string.h"

GATE_STATE_T json_2_msg(char *json, msg_t *msg)
{
    if (json == NULL || msg == NULL)
    {
        log_error("json_2_msg: json or msg is null");
        return GATE_ERRO;
    }

    cJSON *root = cJSON_Parse(json);
    if (root == NULL)
    {
        log_error("json_2_msg: cJSON_Parse error");
        return GATE_ERRO;
    }

    msg->connetType = NULL;
    msg->action = NULL;
    msg->status = NULL;

    // 进行深拷贝之前，需要先进行启用对应的宏定义。
    msg->connetType = strdup(cJSON_GetObjectItem(root, "connetType")->valuestring);
    msg->action = strdup(cJSON_GetObjectItem(root, "action")->valuestring);
    msg->motorId = cJSON_GetObjectItem(root, "motorId")->valueint;

    cJSON *motorSpeedItem = cJSON_GetObjectItem(root, "motorSpeed");
    if (motorSpeedItem != NULL)
    {
        msg->motorSpeed = motorSpeedItem->valueint;
    }

    cJSON *statusItem = cJSON_GetObjectItem(root, "status");
    if (statusItem != NULL)
    {
        msg->status = strdup(statusItem->valuestring);
    }

    cJSON_Delete(root);
}

GATE_STATE_T msg_2_json(msg_t *msg, char json[])
{
    cJSON *root = cJSON_CreateObject();
    if (root == NULL)
    {
        log_error("msg_2_json: cJSON_CreateObject error");
        return GATE_ERRO;
    }
    cJSON_AddStringToObject(root, "action", msg->action);
    cJSON_AddStringToObject(root, "connetType", msg->connetType);
    cJSON_AddNumberToObject(root, "motorId", msg->motorId);
    cJSON_AddNumberToObject(root, "motorSpeed", msg->motorSpeed);

    if (msg->status != NULL)
    {
        cJSON_AddStringToObject(root, "status", msg->status);
    }

    char *str = cJSON_PrintUnformatted(root);
    stpcpy(json, str);
    json[strlen(str)] = '\0';
    cJSON_Delete(root);

    return GATE_OK;
}
