#include "./header/http_request/http_request.h"
#include "./header/thread/thread.h"

void *request_function_1(void *arg)
{
    send_http_request("1.209.148.92", 80, "/info1.php");
}

void *request_function_2(void *arg)
{
    send_http_request("1.209.148.92", 80, "/info1.php");
}

int main(void)
{
    int num_threads = 2;
    thread_func functions[] = {request_function_1, request_function_2};
    create_and_run_threads(functions, num_threads);
    return 0;
}
