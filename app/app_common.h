#ifndef __APP_COMMON_H__
#define __APP_COMMON_H__

#include "log/log.h"
#include "cjson/cJSON.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "stdbool.h"
#include "unistd.h"

typedef enum
{
    GATE_OK,
    GATE_ERRO,
    GATE_TIMEOUT,
    GATE_OTHER,
}GATE_STATE_T;


#endif /* __APP_COMMON_H__ */