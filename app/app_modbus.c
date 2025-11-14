#include "app_modbus.h"

modbus_t *app_modbus;

GATE_STATE_T app_modbus_init(void)
{
    // 初始化modbus
    // 在Linux当中一切都是文件，包括串口。
    app_modbus = modbus_new_rtu("/dev/ttyS1", 115200,'N', 8, 1);
    if (app_modbus == NULL)
    {
        printf("modbus_new_rtu error\n");
        return GATE_ERRO;
    }

    // 启用调试模式，看到更多的细节
    modbus_set_debug(app_modbus, true);
    log_info("modbus_new_rtu ok");

    // 连接modbus 起始就是使能。开启modbus
    int result = modbus_connect(app_modbus);
    if (result == -1)
    {
        printf("modbus_connect error\n");
        modbus_close(app_modbus);
        modbus_free(app_modbus);
        return GATE_ERRO;
    }
    log_info("modbus_connect ok");
    return GATE_OK;
}

// 传递到保持寄存器肯定是int16,但是这里的函数是16位所以使用uint16
GATE_STATE_T app_modbus_write_hold_register(uint8_t address, uint16_t speed)
{
    // 设置从机地址
    modbus_set_slave(app_modbus, address);
    // 写死寄存器地址为0,当让这是可以传递参数的，但是我们这里是测试一个电机外设。所以就写0
    int result = modbus_write_register(app_modbus, 0, speed);
    if (result == -1)
    {
        printf("modbus_write_register error\n");
        return GATE_ERRO;
    }
    log_info("modbus_write_register ok");
    return GATE_OK;
}

GATE_STATE_T app_modbus_read_input_register(uint8_t address, uint16_t *speed)
{
    // 设置从机的地址
    modbus_set_slave(app_modbus, address);
    // 只读取了一个数据
    int result = modbus_read_input_registers(app_modbus, 0, 1, speed);
    if (result == -1)
    {
        printf("modbus_read_input_registers error\n");
        return GATE_ERRO;
    }
    log_info("modbus_read_input_registers ok");
    return GATE_OK;
}
