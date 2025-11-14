# 定义变量
cc := gcc

log := thirdparty/log/log.c thirdparty/log/log.h
cjson := thirdparty/cjson/cJSON.c thirdparty/cjson/cJSON.h
app_mqtt := app/app_mqtt.c app/app_mqtt.h
app_pool := app/app_pool.c app/app_pool.h
app_buffer:= app/app_buffer.c app/app_buffer.h
app_dev := app/app_dev.c app/app_dev.h
app_msg := app/app_msg.c app/app_msg.h

log_test: test/log_test.c $(log)
	@$(cc) $^ -o $@ -Ithirdparty
	@./$@
	@rm -rf $@


cjson_test: test/cjson_test.c $(cjson) $(log)
	@$(cc) $^ -o $@ -Ithirdparty
	@./$@
	@rm -rf $@

app_mqtt_test: test/app_mqtt_test.c $(app_mqtt) $(cjson) $(log)
	@$(cc) $^ -o $@ -Ithirdparty -Iapp -lpaho-mqtt3c
	@./$@   
	@rm -rf $@

thread_test1: test/thread_test1.c $(log)
	@$(cc) $^ -o $@ -Ithirdparty -Iapp
	@./$@
	@rm -rf $@


thread_test2: test/thread_test2.c $(log)
	@$(cc) $^ -o $@ -Ithirdparty -Iapp
	@./$@
	@rm -rf $@

thread_test3: test/thread_test3.c $(log)
	@$(cc) $^ -o $@ -Ithirdparty -Iapp
	@./$@
	@rm -rf $@

app_pool_test: test/app_pool_test.c $(log) $(app_pool)
	@$(cc) $^ -o $@ -Ithirdparty -Iapp
	@./$@
	@rm -rf $@

app_buffer_test: test/app_buffer_test.c $(log) $(app_buffer) $()
	@$(cc) $^ -o $@ -Ithirdparty -Iapp
	@./$@
	@rm -rf $@
app_dev_test: test/app_dev_test.c $(log) $(app_buffer) $(app_dev) $(app_msg) $(app_mqtt) $(cjson) $(app_pool)
	@$(cc) $^ -o $@ -Ithirdparty -Iapp -lpaho-mqtt3c
	@./$@
	@rm -rf $@
#伪目标
.PHONY: clean
clean:
	@rm -rf main *.o main





CC:=$(CROSS_COMPILE)gcc

BOARD_DIR := $(shell pwd)/
#改为自己开发板的ip地址
PEER := root@192.168.56.78

CFLAGS += -Wall -Wextra

CFLAGS += -I.
CFLAGS += -Ithirdparty
CFLAGS += -Iapp
CFLAGS += -Idaemon
CFLAGS += -Iota

ifdef SYSROOT
	CFLAGS += --sysroot=$(SYSROOT)
endif

LDLIBS += -lpaho-mqtt3c
#LDLIBS += -lcurl
#LDLIBS += -lcrypto
LDLIBS += -lmodbus
#modbus文件路径,toolchain前面的路径需要改为自己的工程路径
#LDLIBS += -L/home/shtos/桌面/01_Projects/485_gateway/toolchain/arm-linux-gnueabihf/lib

SRC += $(shell find app -name "*.c" -type f)
#SRC += $(shell find daemon -name "*.c"  -type f)
#SRC += $(shell find ota -name "*.c" -type f)
SRC += $(shell find thirdparty -name "*.c"  -type f)

OBJ := $(SRC:.c=.o)

TARGET := gateway

.PHONY: all, clean

all: $(TARGET)

clean:
	@-rm -f $(TARGET) $(OBJ) main.o
$(TARGET): main.o $(OBJ)
	@-$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

cross-compile:
	@CROSS_COMPILE=$(BOARD_DIR)/toolchain/bin/arm-linux-gnueabihf- \
	 SYSROOT=$(BOARD_DIR)/sysroot \
	 make -j16 all
	@scp -O $(TARGET) $(PEER):/usr/bin/$(TARGET)

cross-init:
	@scp -O init/S99gateway $(PEER):/etc/init.d/S99gateway

%.o: %.c
	@-$(CC) $(CFLAGS) -c $^ -o $@

%_test: test/%_test.o $(OBJ)
	@-$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)
	@-./$@
	@-rm $@ $^

#./表示让系统在当前目录(GATEWAY_SHT)下查找这个程序
# $^表示所有依赖文件，即所有的.o文件
# $@表示目标(target)文件，即可执行文件
# -I表示在编译时添加头文件搜索路径