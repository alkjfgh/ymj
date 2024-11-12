#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "libdebug.h"

///////////////////////////////////////////////
// debug log
//////////////////////////////////////////////

int g_debug_limit_level = 4; //디버그 레벨(defautl:4), 해당 파일전역 변수 

/*
기능: 디버그 레벨을 설정하는 함수
*/
int set_debug_level(int level)
{
    g_debug_limit_level = level;
    return 0;
}
/*
기능: 디버그 레벨을 반환하는 함수
*/
int get_debug_level()
{
    return g_debug_limit_level;
}

/*
기능: 로그 파일에 로그를 기록하는 기능을 수행, 파일이름은 날짜로 자동 생성
인자: file_name -- 저장할 로그 파일 이름(NULL이면  년-월-일.log 로 자동 설정) 
         section - 로그 섹션, message - 로그 메시지
리턴: 성공시 0, 실패시 음수 값
*/
/*
int debug_log_v0( const char* file_name,const char *section, const char *message) {
    
    
    
    int BUFFER_SIZE = 256;

    char filename[256];
    char time_buffer[BUFFER_SIZE];
     
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    if(NULL == file_name)
    {
        
        strftime(time_buffer, BUFFER_SIZE, "%Y-%m-%d", t);
        sprintf(filename,"%s.log",time_buffer);
    }
    else
    {
        sprintf(filename,"%s",file_name);
    }

    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        perror("Failed to open log file");
        return -1;
    }

    

    time_buffer[BUFFER_SIZE];
    strftime(time_buffer, BUFFER_SIZE, "%Y-%m-%d %H:%M:%S", t);

    // Write log entry to file: time, section, message
    fprintf(file, "%s, %s, %s", time_buffer, section, message);

    fclose(file);
    return 0;
}
*/

/*
기능: 로그 파일에 로그를 기록하는 기능을 수행, 파일이름은 날짜로 자동 생성
인자: 
    level - 로그 레벨,
    file_name -- 저장할 로그 파일 이름(NULL이면  년-월-일.log 로 자동 설정) 
    section - 로그 섹션, 
    message - 로그 메시지 
리턴: 성공시(0), 실패시(음수 값)

level:  
    0: 동작로그, 최소화 
    1: 차단/탐지 행위없이 이벤트 발생 동작로그만 기록
    2: 동작로그 자세히(최대로그) 기록
    3: 함수 호출 로그  
    4: 특수 디버깅 로그 
*/
int debug_log(int level, const char* file_name,const char *section, const char *message) {
    if(g_debug_limit_level < level )
    {
        return -2;
    }

    //int BUFFER_SIZE = 256;

    char filename[256];
    char time_buffer[512];
     
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    if(NULL == file_name)
    {
        
        //strftime(time_buffer, BUFFER_SIZE, "%Y-%m-%d", t);
        strftime(time_buffer,sizeof(time_buffer), "%Y-%m-%d", t);
        sprintf(filename,"%s.log",time_buffer);
    }
    else
    {
        sprintf(filename,"%s",file_name);
    }

    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        perror("Failed to open log file");
        fclose(file);
        
        return -1;
    }

     
    strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%d %H:%M:%S", t);

    // Write log entry to file: time, section, message
    fprintf(file, "%s,%d,%s,%s", time_buffer,level, section, message);

    fclose(file);

    return 0;
}
