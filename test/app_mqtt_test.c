#include "app_mqtt.h"
#include "MQTTClient.h" 

void recv_callback(void* data,int len){
    log_info("recv_callback:%.*s",len,data);
}
int main(int argc, char const *argv[])
{
    app_mqtt_init();
    app_mqtt_register_callback(recv_callback);
    while (1)
    {
        sleep(1);
    }
    return 0;
}
