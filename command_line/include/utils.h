#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

// 파일 유틸리티
bool file_exists(const char *path);

// 로깅 유틸리티
void log_error(const char *message);
void log_info(const char *message);

#endif