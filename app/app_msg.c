#include "app_msg.h"
#include "string.h"

GATE_STATE_T json_2_msg(char *json, msg_t *msg)
{
    if (json == NULL || msg == NULL)
    {
        log_error("json_2_msg: json or msg is null");
        return GATE_ERRO;
    }

    log_info("json_2_msg: json: %s", json);

    cJSON *root = cJSON_Parse(json);
    if (root == NULL)
    {
        log_error("json_2_msg: cJSON_Parse error");
        return GATE_ERRO;
    }

    GATE_STATE_T result = GATE_OK;

    // 关键：初始化结构体内的指针，以防部分成功后发生错误
    msg->connetType = NULL;
    msg->action = NULL;
    msg->status = NULL;

    // 提取和验证必需字段
    cJSON *connetType_item = cJSON_GetObjectItem(root, "connetType");
    cJSON *action_item = cJSON_GetObjectItem(root, "action");
    cJSON *motorId_item = cJSON_GetObjectItem(root, "motorId");

    if (!cJSON_IsString(connetType_item) || !cJSON_IsString(action_item) || !cJSON_IsNumber(motorId_item))
    {
        log_error("json_2_msg: Missing or invalid type for required fields (connetType, action, motorId)");
        result = GATE_ERRO;
    }
    else
    {
        // 只有在必需字段都有效时才继续
        msg->connetType = strdup(connetType_item->valuestring);
        msg->action = strdup(action_item->valuestring);
        msg->motorId = motorId_item->valueint;

        if (msg->connetType == NULL || msg->action == NULL)
        {
            log_error("json_2_msg: Failed to allocate memory for string fields");
            result = GATE_ERRO;
        }
        else
        {
            // 处理可选字段
            cJSON *motorSpeed_item = cJSON_GetObjectItem(root, "motorSpeed");
            if (cJSON_IsNumber(motorSpeed_item))
            {
                msg->motorSpeed = motorSpeed_item->valueint;
            }
            else
            {
                msg->motorSpeed = 0; // 提供一个默认值
            }

            cJSON *status_item = cJSON_GetObjectItem(root, "status");
            if (cJSON_IsString(status_item))
            {
                msg->status = strdup(status_item->valuestring);
                if (msg->status == NULL)
                {
                    log_error("json_2_msg: Failed to allocate memory for status");
                    result = GATE_ERRO;
                }
            }
        }
    }

    // 统一的清理逻辑
    if (result != GATE_OK)
    {
        // 如果过程中任何一步失败，释放所有已分配的内存
        // free(NULL) 是安全无操作的，所以无需检查指针是否为NULL
        free(msg->connetType);
        free(msg->action);
        free(msg->status);
        // 将指针置为NULL，防止调用者使用悬挂指针
        msg->connetType = NULL;
        msg->action = NULL;
        msg->status = NULL;
    }

    cJSON_Delete(root);
    return result;
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
