#include "app_mqtt.h"
#include <openssl/ssl.h> // 必须包含
#include <openssl/err.h> // 必须包含
#define ADDRESS "tcp://192.168.56.30:1883"
#define CLIENTID "smart_gateway_client"
#define TOPIC_PULL "pull"
#define TOPIC_PUSH "push"
#define QOS 0

typedef struct
{
    MQTTClient client_handle;
    void (*recv_callback)(void *data, int len);

} app_mqtt_t;
app_mqtt_t app_mqtt;

volatile MQTTClient_deliveryToken deliveredtoken;

void conn_lost_cb(void *context, char *cause);
int msg_arrvd_cb(void *context, char *topicName, int topicLen, MQTTClient_message *message);
void delivered_cb(void *context, MQTTClient_deliveryToken dt);

GATE_STATE_T app_mqtt_init(void)
{
    // 创建mqtt客户端
    if (MQTTClient_create(&app_mqtt.client_handle, ADDRESS, CLIENTID,
                          MQTTCLIENT_PERSISTENCE_NONE, NULL) != MQTTCLIENT_SUCCESS)
    {
        log_error("mqtt init failed");
        return GATE_ERRO;
    }
    log_info("mqtt init success");

    // 设置回调函数
    if (MQTTClient_setCallbacks(app_mqtt.client_handle, NULL, conn_lost_cb, msg_arrvd_cb, delivered_cb) != MQTTCLIENT_SUCCESS)
    {
        log_error("mqtt set callback failed");
        return GATE_ERRO;
    }
    log_info("mqtt set callback success");

    // 连接订阅
    MQTTClient_connectOptions app_mqtt_conn_opts = MQTTClient_connectOptions_initializer;
    if ((MQTTClient_connect(app_mqtt.client_handle, &app_mqtt_conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        log_error("mqtt connect failed");
        return GATE_ERRO;
    }
    log_info("mqtt connect success");

    // 订阅主题
    if (MQTTClient_subscribe(app_mqtt.client_handle, TOPIC_PULL, QOS) != MQTTCLIENT_SUCCESS)
    {
        log_error("mqtt subscribe failed");
        return GATE_ERRO;
    }
    log_info("mqtt subscribe success :%s", TOPIC_PULL);

    return GATE_OK;
}

void conn_lost_cb(void *context, char *cause)
{
    log_info("mqtt connect lost:%s", cause);
    // 连接失败，进行重新连接
    MQTTClient_connectOptions app_mqtt_conn_opts = MQTTClient_connectOptions_initializer;
    while (1)
    {
        if ((MQTTClient_connect(app_mqtt.client_handle, &app_mqtt_conn_opts)) != MQTTCLIENT_SUCCESS)
        {
            log_error("mqtt connect failed");
            continue;
        }
        else
        {
            log_info("mqtt connect success");
            break;
        }
    }

    // 重新订阅主题
    while (1)
    {
        if (MQTTClient_subscribe(app_mqtt.client_handle, TOPIC_PULL, QOS) != MQTTCLIENT_SUCCESS)
        {
            log_error("mqtt subscribe failed");
            continue;
        }else{
            log_info("mqtt subscribe success :%s", TOPIC_PULL);
            break;
        }
    }
    
}

int msg_arrvd_cb(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
    // log_info("mqtt message arrived:%.*s", message->payloadlen, message->payload);
    if (app_mqtt.recv_callback)
    {
        app_mqtt.recv_callback(message->payload, message->payloadlen);
    }

    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}

void delivered_cb(void *context, MQTTClient_deliveryToken dt)
{
    printf("Message with token value %d delivery confirmed\n", dt);
    deliveredtoken = dt;
}

GATE_STATE_T app_mqtt_send(void *data, int len)
{
    if (MQTTClient_publish(app_mqtt.client_handle, TOPIC_PUSH, len, data, QOS, 0, NULL) != MQTTCLIENT_SUCCESS)
    {
        log_error("mqtt publish failed");
        return GATE_ERRO;
    }
    log_info("mqtt publish success:%s", TOPIC_PUSH);
    return GATE_OK;
}

void app_mqtt_register_callback(void (*recv_callback)(void *data, int len))
{
    app_mqtt.recv_callback = recv_callback;
}

// 把mqtt客户端反初始化
void app_mqtt_deinit(void)
{
    // 取消主题订阅
    MQTTClient_unsubscribe(app_mqtt.client_handle, TOPIC_PULL);
    log_info("mqtt unsubscribe success:%s", TOPIC_PULL);
    // 断开连接
    MQTTClient_disconnect(app_mqtt.client_handle, 10000);
    log_info("mqtt disconnect success");
    // 销毁mqtt客户端
    MQTTClient_destroy(&app_mqtt.client_handle);
}
