#ifndef __APP_COMMON_H__
#define __APP_COMMON_H__
#define _GNU_SOURCE  // 使用posix 扩展功能 . 必须时所有依赖的第1行

#include "log/log.h"
#include "cjson/cJSON.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "stdbool.h"
#include "unistd.h"
#include "string.h"

typedef enum
{
    GATE_OK,
    GATE_ERRO,
    GATE_TIMEOUT,
    GATE_OTHER,
}GATE_STATE_T;


#endif /* __APP_COMMON_H__ */