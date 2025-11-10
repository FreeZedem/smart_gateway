#include "cjson/cJSON.h"
#include "log/log.h"

void parse(void)
{
    char *json = "{\"name\":\"cjson\",\"type\":\"library\"}";

    cJSON *root = cJSON_Parse(json);
    if (root == NULL)
    {
        log_error("parse json failed");
        return;
    }
    log_info("name: %s", cJSON_GetObjectItem(root, "name")->valuestring);
    log_info("type: %s", cJSON_GetObjectItem(root, "type")->valuestring);

    cJSON_Delete(root);
}

void toJsonStr(void)
{
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "name", "cjson");
    cJSON_AddStringToObject(root, "type", "library");
    char *s = cJSON_PrintUnformatted(root);
    log_info("json: %s", s);
    cJSON_Delete(root);
}

int main(int argc, char const *argv[])
{
    // parse();
    toJsonStr();
    return 0;
}
