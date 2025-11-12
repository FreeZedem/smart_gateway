# 定义变量
cc := gcc

log := thirdparty/log/log.c thirdparty/log/log.h
cjson := thirdparty/cjson/cJSON.c thirdparty/cjson/cJSON.h
app_mqtt := app/app_mqtt.c app/app_mqtt.h
app_pool := app/app_pool.c app/app_pool.h
app_buffer:= app/app_buffer.c app/app_buffer.h

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

app_buffer_test: test/app_buffer_test.c $(log) $(app_buffer)
	@$(cc) $^ -o $@ -Ithirdparty -Iapp
	@./$@
	@rm -rf $@
#伪目标
.PHONY: clean
clean:
	@rm -rf main *.o main

