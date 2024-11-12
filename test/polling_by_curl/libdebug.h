/*
  로컬에 로그 파일을 남기는 라이브러리
*/
#ifndef LIBDEBUG_H
#define LIBDEBUG_H

#include <json-c/json.h>

int debug_log(int level,const char *filename, const char *section, const char *message) ;

int set_debug_level(int level);
int get_debug_level();



#endif 