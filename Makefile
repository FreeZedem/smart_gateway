# 定义变量
cc := gcc

log := thirdparty/log/log.c thirdparty/log/log.h
cjson := thirdparty/cjson/cJSON.c thirdparty/cjson/cJSON.h
app_mqtt := app/app_mqtt.c app/app_mqtt.h


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
#伪目标
.PHONY: clean
clean:
	@rm -rf main *.o main

