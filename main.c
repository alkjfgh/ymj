#include <stdio.h>
#include <unistd.h>
#include "./header/http_request/http_request.h"
#include "./header/thread/thread.h"

#define SIZE 100

void remove_file(const char *path)
{
    if (access(path, F_OK) != -1)
    {
        // 파일이 존재함
        if (unlink(path) != 0)
        {
            perror("Error deleting file");
        }
        else
        {
            printf("File deleted successfully\n %s\n", path);
        }
    }
    else
    {
        printf("File does not exist\n %s\n", path);
    }
}

void *request_function_1(void *arg)
{
    for (int i = 0; i < SIZE; i++)
    {
        download_http_request("80.249.99.148", 80, "/100MB.zip", "./100MB_1.zip");
        printf("function 1 download %d\n", i);
        remove_file("./100MB_1.zip");
        sleep(1);
    }
}

void *request_function_2(void *arg)
{
    for (int i = 0; i < SIZE; i++)
    {
        download_http_request("80.249.99.148", 80, "/100MB.zip", "./100MB_2.zip");
        printf("function 2 download %d\n", i);
        remove_file("./100MB_2.zip");
        sleep(1);
    }
}

int main(void)
{
    int num_threads = 2;
    thread_func functions[] = {request_function_1, request_function_2};
    create_and_run_threads(functions, num_threads);
    return 0;
}
