#include "app_pool.h"

typedef struct
{
    void (*task)(void *); // 传递的程序
    void *data;           // 传递的参数
} app_pool_task_t;

typedef struct
{
    pthread_t *thread;
    mqd_t mq;
    char *mq_name;
    int pool_size;
} app_pool_t;
app_pool_t app_pool;

void *app_pool_thread(void *arg);


GATE_STATE_T app_pool_init(int pool_size)
{

    app_pool.mq_name = "/pool";
    app_pool.pool_size = pool_size;

    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = pool_size;
    attr.mq_msgsize = sizeof(app_pool_task_t); // 消息大小和线程数有关，线程数越多，消息大小越大，线程数越少，消息大小越小
    app_pool.mq = mq_open(app_pool.mq_name, O_CREAT | O_RDWR, 0666, &attr);

    // 分配好内存，并存储线程的id。所建立了一堆线程池，去执行同一个任务
    app_pool.thread = malloc(pool_size * sizeof(pthread_t));
    // 以此启动线程id
    for (size_t i = 0; i < pool_size; i++)
    {
        int result = pthread_create(&app_pool.thread[i], NULL, app_pool_thread, NULL);
        if (result == -1)
        {
            log_error("create app pool thread failed");
            return GATE_ERRO;
        }
    }
    log_info("app pool init success");
    return GATE_OK;
}

void *app_pool_thread(void *arg)
{
    while (1)
    {
        app_pool_task_t task;
        ssize_t size = mq_receive(app_pool.mq, (char *)&task, sizeof(app_pool_task_t), NULL);
        if (size > 0 && task.task)
        {
            task.task(task.data);
        }
    }
}

void app_pool_deinit(void)
{
    for (size_t i = 0; i < app_pool.pool_size; i++)
    {
        pthread_cancel(app_pool.thread[i]);
    }
    free(app_pool.thread);

    mq_close(app_pool.mq);
    mq_unlink(app_pool.mq_name);

    log_info("app pool deinit success");
}

GATE_STATE_T app_pool_add_task(void (*task)(void *), void *data)
{
    if (task == NULL)
    {
        log_error("task don't NUll");
        return GATE_ERRO;
    }

    app_pool_task_t task_info = {
        .task = task,
        .data = data,
    };

    int result = mq_send(app_pool.mq, (char *)&task_info, sizeof(app_pool_task_t), 0);
    if (result == -1)
    {
        log_error("add task to app pool failed");
        return GATE_ERRO;
    }
    log_info("add task to app pool success");
    return GATE_OK;
}