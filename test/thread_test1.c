#include "app_common.h"
#include "pthread.h"
//线程创建
//pthread_create
//线程取消
//线程执行完
//线程自杀：pthread_exit
//线程被杀：pthread_cancel
void *fun1(void *arg)
{
    int a = 0;
    while (1)
    {
        log_info("thread1 is running");
        a++;
        if(a > 10){
            //pthread_exit(NULL);
            break;
        }
        sleep(1);
    }
}

int main(int argc, char const *argv[])
{
    pthread_t t1;
    // 参数1:线程id 参数2:线程属性(传入null,表示使用默认)
    // 返回0表示创建成功, 返回非0失败
    if (pthread_create(&t1, NULL, fun1, NULL) != 0)
    {
        log_error("创建线程失败");
        return 0;
    }
    log_info("创建线程成功");

    sleep(10);
    //pthread_cancel(t1);  // 取消指定的线程

    sleep(100000);

    return 0;
}
