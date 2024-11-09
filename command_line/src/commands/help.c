#include "commands.h"
#include <stdio.h>

void display_help(bool first_section) {
    if (first_section) {
        printf("사용법: command [옵션]\n\n");
        printf("기본 옵션:\n");
        printf("  -h              도움말 표시\n");
        printf("  /?              도움말 표시 (Windows 스타일)\n");
        printf("  -D              로그 삭제 후 재시작\n");
        printf("  -0              사용자 입력 없이 설치\n");
        printf("  -1              사용자 입력으로 설치\n");
        // ... 추가 도움말 내용
    } else {
        printf("추가 옵션:\n");
        printf("  -i <url>        서버에 연결\n");
        printf("  -a <path>       화이트리스트에 추가\n");
        printf("  -ah <path>      해시를 화이트리스트에 추가\n");
        // ... 추가 도움말 내용
    }
}