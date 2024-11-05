#include <unistd.h>
#include <stdio.h>
#include "../../header/http_request/http_request.h"
#include "../../header/json/json_builder.h"
#include <time.h>
#include <stdlib.h>

double get_random_cpu_usage()
{
    return (double)(rand() % 100) / 10.0; // 0.0 ~ 9.9 사이 값
}

int get_random_memory_load()
{
    return rand() % 100; // 0 ~ 99 사이 값
}

int get_random_process_cnt()
{
    return 500 + (rand() % 100); // 500 ~ 599 사이 값
}

long get_random_net_usage()
{
    return 15000000 + (rand() % 5000000); // 15MB ~ 20MB 사이 값
}

char *get_current_timestamp()
{
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    static char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", t);
    return timestamp;
}

int main(void)
{
    srand(time(NULL)); // 난수 생성기 초기화
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

    for (int i = 1; i <= 10; i++)
    {
        const char *data = json_stringify(obj);
        printf("json data: %s\n", data);
        printf("send polling post request %d\n", i);
        send_http_post_request("59.6.79.138", 8080, "/tc_connect_continue.php", data);

        json_update_string(obj, "time_stamp", get_current_timestamp());
        json_update_number(obj, "tc_cpu_usage", get_random_cpu_usage());
        json_update_number(obj, "memory_load", get_random_memory_load());
        json_update_number(obj, "process_cnt", get_random_process_cnt());
        json_update_number(obj, "net_usage", get_random_net_usage());

        sleep(1);
    }

    json_free(obj);
    return 0;
}