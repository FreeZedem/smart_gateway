#include "app_pool.h"

void task(void*args){
    log_info("task,%d",*(int*)args);
}
int main(int argc, char const *argv[])
{
    app_pool_init(10);

    for (size_t i = 0; i < 20; i++)
    {
        int* j = malloc(sizeof(int));
        *j = i;
        app_pool_add_task(task,j);
    }
    
    sleep(10);
    app_pool_deinit();
    return 0;
}
