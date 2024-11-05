#include <unistd.h>
#include <stdio.h>
#include "../header/http_request/http_request.h"
#include "../header/json/json_builder.h"

int main(void)
{
    JsonObject *obj = json_create_object();
    json_add_string(obj, "log_type", "tc_connect_continue");
    json_add_string(obj, "user_id", "00-15-5d-a9-c9-8a");
    json_add_string(obj, "time_stamp", "2024-08-08 13:51:25");
    json_add_string(obj, "mac", "00-15-5d-a9-c9-8a");
    json_add_string(obj, "local_ip", "172.22.107.42");
    json_add_string(obj, "web_ip", "121.134.218.173");
    json_add_string(obj, "config_linux_hash", "88a31795700d20d898b7f2d870bdbddaa47be2e497529a8ac1aea55c45d11f74");
    json_add_number(obj, "tc_cpu_usage", 1.0);
    json_add_number(obj, "memory_load", 5);
    json_add_number(obj, "process_cnt", 538);
    json_add_number(obj, "net_usage", 17188205);
    json_add_string(obj, "operating_mode", "detect");

    const char *data = json_stringify(obj);

    "\"user_id\":\"00-15-5d-a9-c9-8a\","
    "\"time_stamp\":\"2024-08-08 13:51:25\","
    "\"mac\":\"00-15-5d-a9-c9-8a\","
    "\"local_ip\":\"172.22.107.42\","
    "\"web_ip\":\"121.134.218.173\","
    "\"config_linux_hash\":\"88a31795700d20d898b7f2d870bdbddaa47be2e497529a8ac1aea55c45d11f74\","
    "\"tc_cpu_usage\":1.0,"
    "\"cpu_usage\":1.0,"
    "\"memory_load\":5,"
    "\"process_cnt\":538,"
    "\"net_usage\":17188205,"
    "\"operating_mode\":\"detect\""
    "}";

    printf("json data: %s\n", data);

    for (int i = 0; i < 10; i++)
    {
        printf("send polling post request %d\n", i);
        send_http_post_request("59.6.79.138", 8080, "/tc_connect_continue.php", data);
        sleep(1);
    }
    return 0;
}