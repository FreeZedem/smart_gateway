#include "app_buffer.h"
#include "pthread.h"

typedef struct
{
    uint8_t *ptr;      // 指向缓冲区的地址
    uint32_t capacity; // 缓冲区的长度
    uint32_t len;      // 缓冲区的使用长度
} sub_buffer_t;

typedef struct
{
    sub_buffer_t *sub_buffer[2]; // 双缓冲区架构，一个是读，一个是写。
    int read_index;              // 读缓冲区的索引
    int write_index;             // 写缓冲区的索引

    // 防止竞态问题
    pthread_mutex_t read_lock;  // 读缓冲区的锁
    pthread_mutex_t write_lock; // 写缓冲区的锁
} buffer_t;

sub_buffer_t *app_buffer_init_sub_buffer(int capacity)
{
    sub_buffer_t *sub_buffer = malloc(sizeof(sub_buffer_t));
    if (sub_buffer == NULL)
    {
        log_error("malloc sub_buffer error");
        return NULL;
    }

    sub_buffer->ptr = malloc(capacity * sizeof(uint8_t));
    sub_buffer->capacity = capacity;
    sub_buffer->len = 0;

    memset(sub_buffer->ptr, 0, capacity);
    return sub_buffer;
}

buffer_handle_t *app_buffer_init(int capacity)
{
    buffer_t *buffer = malloc(sizeof(buffer_t));
    if (buffer == NULL)
    {
        log_error("malloc buffer error");
        return NULL;
    }
    // 初始化第一个缓冲区
    buffer->sub_buffer[0] = app_buffer_init_sub_buffer(capacity);
    if (buffer->sub_buffer[0] == NULL)
    {
        log_error("app_buffer_init_sub_buffer error");
        free(buffer);
        return NULL;
    }
    buffer->sub_buffer[1] = app_buffer_init_sub_buffer(capacity);
    if (buffer->sub_buffer[1] == NULL)
    {
        log_error("app_buffer_init_sub_buffer error");
        free(buffer);
        free(buffer->sub_buffer[0]);
        return NULL;
    }
    // 读写缓冲区的内部下标不一致
    buffer->read_index = 0;
    buffer->write_index = 1;
    pthread_mutex_init(&buffer->read_lock, NULL);
    pthread_mutex_init(&buffer->write_lock, NULL);

    log_info("app_buffer_init success");

    return (void*)buffer;
}

void app_buffer_deinit(buffer_handle_t  *buffer_handle)
{
    buffer_t *buffer = (buffer_t *)buffer_handle;
    free(buffer->sub_buffer[0]->ptr);
    free(buffer->sub_buffer[0]);
    free(buffer->sub_buffer[1]->ptr);
    free(buffer->sub_buffer[1]);
    // free(buffer->sub_buffer);
    pthread_mutex_destroy(&buffer->read_lock);
    pthread_mutex_destroy(&buffer->write_lock);
    free(buffer);
    log_info("app_buffer_deinit success");
}

GATE_STATE_T app_buffer_write(buffer_handle_t *buffer_handle, char *data, uint8_t len)
{
    buffer_t *buffer = (buffer_t *) buffer_handle;
    // 拿到写锁，
    pthread_mutex_lock(&buffer->write_lock);
    // 找到写缓冲区
    sub_buffer_t *write_buffer = buffer->sub_buffer[buffer->write_index];
    // 开始写数据，判断是否有空间
    if (write_buffer->capacity - write_buffer->len < len + 1)
    {
        log_warn("app_buffer_write no space: 剩余：%d, 需要：%d", write_buffer->capacity - write_buffer->len, len + 1);
        pthread_mutex_unlock(&buffer->write_lock);
        return GATE_ERRO;
    }

    // 先写长度
    write_buffer->ptr[write_buffer->len] = len;
    // 再写数据
    memcpy(write_buffer->ptr + write_buffer->len + 1, data, len);
    // 更新长度
    write_buffer->len += len + 1;

    pthread_mutex_unlock(&buffer->write_lock);
    log_info("app_buffer_write success");
    return GATE_OK;
}

int app_buffer_read(buffer_handle_t *buffer_handle, char *data, int data_capacity)
{
    buffer_t *buffer = (buffer_t *) buffer_handle;
    // 写获取锁
    pthread_mutex_lock(&buffer->read_lock);
    sub_buffer_t *read_buffer = buffer->sub_buffer[buffer->read_index];

    //开始交换缓冲区，并读数据
    if (read_buffer->len == 0)
    {
        // 没有数据读，准备交换缓冲区
        pthread_mutex_lock(&buffer->write_lock);
        // log_info("app_buffer_read no data");
        buffer->read_index ^= buffer->write_index;
        buffer->write_index ^= buffer->read_index;
        buffer->read_index ^= buffer->write_index;
        pthread_mutex_unlock(&buffer->write_lock);

        read_buffer = buffer->sub_buffer[buffer->read_index];
        //交换之后还是没有数据，所以结束
        if (read_buffer->len == 0)
        {
            // log_warn("app_buffer_read no data");
            pthread_mutex_unlock(&buffer->read_lock);
            return 0;
        }
    }

    char len = read_buffer->ptr[0];
    if (len > data_capacity)
    {
        log_warn("app_buffer_read no data,data_capacity:%d,len:%d",data_capacity,len);
        pthread_mutex_unlock(&buffer->read_lock);
        return 0;
    }
    //在首字符读取数据
    memcpy(data, read_buffer->ptr + 1, len);
    // 移动数据
    memmove(read_buffer->ptr, read_buffer->ptr + len + 1, read_buffer->len - len - 1);
    read_buffer->len -= len + 1;

    pthread_mutex_unlock(&buffer->read_lock);

    log_info("app_buffer_read success,len:%d",len);
    return len;
}