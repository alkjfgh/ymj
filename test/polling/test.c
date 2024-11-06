#include <unistd.h>
#include <stdio.h>
#include "../../header/http_request/http_request.h"
#include "../../header/json/json_builder.h"
#include <time.h>
#include <stdlib.h>

double get_random_cpu_usage()
{
    return (double)(rand() % 100); // 0.0 ~ 99.9 사이 값
}

int get_random_memory_load()
{
    return rand() % 100; // 0 ~ 99 사이 값
}

int get_random_process_cnt()
{
    return 500 + (rand() % 100 * rand()); // 500 ~ 59999
}

long get_random_net_usage()
{
    return 15000000 + (rand() % 5000000 * rand()); // 15MB ~ 20MB 사이 값
}

char *get_current_timestamp()
{
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    static char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", t);
    return timestamp;
}

// 랜덤 MAC 주소 생성
char *get_random_mac()
{
    static char mac[18];
    sprintf(mac, "%02x-%02x-%02x-%02x-%02x-%02x",
            rand() % 256, rand() % 256, rand() % 256,
            rand() % 256, rand() % 256, rand() % 256);
    return mac;
}

// 랜덤 IP 주소 생성
char *get_random_ip()
{
    static char ip[16];
    sprintf(ip, "%d.%d.%d.%d",
            rand() % 256, rand() % 256, rand() % 256, rand() % 256);
    return ip;
}

// 랜덤 해시 생성 (64자 길이)
char *get_random_hash()
{
    static char hash[65];
    const char hex_chars[] = "0123456789abcdef";
    for (int i = 0; i < 64; i++)
    {
        hash[i] = hex_chars[rand() % 16];
    }
    hash[64] = '\0';
    return hash;
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
    json_add_number(obj, "cpu_usage", 1.0);
    json_add_number(obj, "memory_load", 5);
    json_add_number(obj, "process_cnt", 538);
    json_add_number(obj, "net_usage", 17188205);
    json_add_string(obj, "operating_mode", "detect");

    for (int i = 1; i <= 500; i++)
    {
        printf("\n\n==========================\n\njson data: ");
        printf("%s\n", json_serialize_pretty(obj, 0));
        printf("send polling post request %d\n\n", i);
        const char *data = json_stringify(obj);

        send_http_post_request("59.6.79.138", 8080, "/tc_connect_continue.php", data);
        // send_http_post_request("54.173.64.189", 80, "/anything", data);

        json_update_string(obj, "user_id", get_random_mac());
        json_update_string(obj, "time_stamp", get_current_timestamp());
        json_update_string(obj, "mac", get_random_mac());
        json_update_string(obj, "local_ip", get_random_ip());
        json_update_string(obj, "web_ip", get_random_ip());
        json_update_string(obj, "config_linux_hash", get_random_hash());
        json_update_number(obj, "tc_cpu_usage", get_random_cpu_usage());
        json_update_number(obj, "cpu_usage", get_random_cpu_usage());
        json_update_number(obj, "memory_load", get_random_memory_load());
        json_update_number(obj, "process_cnt", get_random_process_cnt());
        json_update_number(obj, "net_usage", get_random_net_usage());
        json_update_string(obj, "operating_mode", (rand() % 2) ? "detect" : "test");

        sleep(1);
    }

    json_free(obj);
    return 0;
}