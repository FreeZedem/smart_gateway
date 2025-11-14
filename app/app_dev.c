#include "app_dev.h"
#include "app_msg.h"
#include "app_modbus.h"

typedef struct
{
    bool is_running;

    buffer_handle_t down_buffer; // 下行缓冲区
    buffer_handle_t up_buffer;   // 上行缓冲区
} device_t;

static device_t device; // 私用静态变量

void recv_callback(void *data, int len);
void app_up_task(void *args);
void app_modbus_task(void *args);
GATE_STATE_T app_dev_init(void)
{
    // 初始化mqtt
    if (app_mqtt_init() != GATE_OK)
    {
        log_fatal("mqtt init error");
        return GATE_ERRO;
    }
    app_mqtt_register_callback(recv_callback);
    log_info("mqtt init ok");
    // 初始化线程池
    if (app_pool_init(10) != GATE_OK)
    {
        log_fatal("pool init error");
        app_mqtt_deinit(); // 释放mqtt
        return GATE_ERRO;
    }
    log_info("pool init ok");
    // 初始化缓冲区
    device.down_buffer = app_buffer_init(1024);
    if (device.down_buffer == NULL)
    {
        log_fatal("down buffer init error");
        app_pool_deinit();
        app_mqtt_deinit();
        return GATE_ERRO;
    }
    device.up_buffer = app_buffer_init(1024);
    if (device.up_buffer == NULL)
    {
        log_fatal("up buffer init error");
        app_buffer_deinit(device.down_buffer);
        app_pool_deinit();
        app_mqtt_deinit();
        return GATE_ERRO;
    }
    log_info("buffer init ok");

    // modbus初始化
    GATE_STATE_T state = app_modbus_init();
        if (state != GATE_OK)
    {
        log_fatal("modubs init error");
        app_buffer_deinit(device.down_buffer);
        app_buffer_deinit(device.up_buffer);
        app_pool_deinit();
        app_mqtt_deinit();
        return GATE_ERRO;
    }
    log_info("modbus init ok");

    log_info("dev init ok");
    return GATE_OK;
}

void app_dev_start(void)
{
    // 添加上行任务
    app_pool_add_task(app_up_task, NULL);
    app_pool_add_task(app_up_task, NULL);
    // 添加下行任务
    app_pool_add_task(app_modbus_task, NULL);
    app_pool_add_task(app_modbus_task, NULL);

    //控制任务是否启动
    app_dev_set_is_running(true);
}

bool app_dev_get_is_running(void)
{
    return device.is_running;
}
void app_dev_stop(void){
    app_dev_set_is_running(false);
}
void app_dev_set_is_running(bool is_running){
    device.is_running = is_running;
}

void app_dev_deinit(void){
    app_mqtt_deinit();
    app_pool_deinit();
    app_buffer_deinit(device.down_buffer); 
    app_buffer_deinit(device.up_buffer);
}


void app_up_task(void *args)
{
    char data[255];
    while (1)
    {
        int real_len = app_buffer_read(device.up_buffer, data, sizeof(data));
        if (real_len > 0)
        {
            data[real_len] = '\0';
            app_mqtt_send(data, real_len);
        }
    }
}

void app_modbus_task(void *args)
{
    char data[255] = {0};
    while (1)
    {
        memset(data, 0, sizeof(data));
        int real_len = app_buffer_read(device.down_buffer, data, sizeof(data));
        if (real_len == 0)
            continue;

        msg_t msg;
        json_2_msg(data, &msg);

        // 打印msg的每个成员
        log_info("action:%s,connetType:%s,motorId:%d,motorSpeed:%d,status:%s", msg.action, msg.connetType, msg.motorId, msg.motorSpeed, msg.status);

        if (strcmp(msg.action, "set") == 0)
        {
            //设置电机速度
            app_modbus_write_hold_register(msg.motorId, (uint16_t)msg.motorSpeed);
        }

        if (strcmp(msg.action, "get") == 0)
        {
            //获取电机状态，给MODBUS发数据
            GATE_STATE_T state = app_modbus_read_input_register(msg.motorId, (uint16_t*)&msg.motorSpeed);
            if (state == GATE_OK)
            {
                msg.status = "ok";
            }
            else
            {
                msg.status = "error";
                msg.motorSpeed = 0;
            }
            memset(data, 0, sizeof(data));
            msg_2_json(&msg, data);
            app_buffer_write(device.up_buffer, data, (uint8_t)strlen(data));
        }
    }
}
void recv_callback(void *data, int len)
{
    app_buffer_write(device.down_buffer, data, len);
}