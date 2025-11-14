#ifndef __APP_MODBUS_H__
#define __APP_MODBUS_H__
#include "modbus/modbus.h"
#include "app_common.h"

GATE_STATE_T app_modbus_init(void);
GATE_STATE_T app_modbus_write_hold_register(uint8_t address, uint16_t speed);
GATE_STATE_T app_modbus_read_input_register(uint8_t address, uint16_t *speed);

#endif /* __APP_MODBUS_H__ */