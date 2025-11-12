
#include "app_buffer.h"
int main(int argc, char const *argv[])
{
    buffer_handle_t buffer_handle = app_buffer_init(10);

    app_buffer_write(buffer_handle, "world", 5);
    app_buffer_write(buffer_handle, "hel", 3);

    char data[1024];
    int read_len = app_buffer_read(buffer_handle, data, sizeof(data));
    printf("read_len: %d, data: %.*s\n", read_len, read_len, data);

    read_len = app_buffer_read(buffer_handle, data, sizeof(data));
    printf("read_len: %d, data: %.*s\n", read_len, read_len, data);

    return 0;
}
