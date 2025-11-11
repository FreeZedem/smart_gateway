#include "app_common.h"
#include "pthread.h"
#include "mqueue.h"

mqd_t mq;
void *thread1 (void *){
    while (1)
    {
        char buf[1024];
        ssize_t size =  mq_receive(mq,buf,sizeof(buf),NULL);
        if (size == -1)
        {
            log_error("mq_receive error");
            return NULL;
        }
        log_info("mq_receive success %.*s",size,buf);
    }
}
int main(int argc, char const *argv[])
{
    pthread_t id;
    pthread_create(&id,NULL,thread1,NULL);
    struct mq_attr attr;
    attr.mq_flags =0;//阻塞模式
    attr.mq_maxmsg = 10;//最大消息数
    attr.mq_msgsize = 1024;//每条消息最大的大小
    //创建消息队列
    mq = mq_open("/mq_test",O_CREAT|O_RDWR,0666,&attr);
    if (mq == -1)
    {
        log_error("mq_open error");
    }
    log_info("mq_open success");

    mq_send(mq,"hello word 1",strlen("hello word 1"),0);
    mq_send(mq,"hello word 2",strlen("hello word 2"),0);
    
    pthread_join(id,NULL);
    mq_close(mq);
    mq_unlink("/mq_test");

    return 0;
}