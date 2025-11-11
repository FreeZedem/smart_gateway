#include "app_common.h"
#include "pthread.h"
//线程竟态问题：需要加锁
//pthread_mutex_lock和pthread_mutex_unlock进行原子化操作

int num = 0;
pthread_mutex_t mutext = PTHREAD_MUTEX_INITIALIZER;
void *fun1(void *arg)
{
    for (size_t i = 0; i < 1000; i++)
    {
        pthread_mutex_lock(&mutext);
        num++;  // num++不是原子操作
        pthread_mutex_unlock(&mutext);
        // cpu从内存读
        // +1
        // cpu写回内存
    }
}

int main(int argc, char const *argv[])
{
    pthread_t tid[100];

    for (size_t i = 0; i < 100; i++)
    {
        pthread_create(&tid[i], NULL, fun1, NULL);
    }

    // 主线程等待子线程
    for (size_t i = 0; i < 100; i++)
    {
        // 等待指定的线程结束
        pthread_join(tid[i], NULL);
    }
    log_info("所有线程都结束了.... %d", num);
    return 0;
}
