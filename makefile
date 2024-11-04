# 컴파일러 설정
CC = gcc

# 컴파일 옵션
CFLAGS = -g  
 

# 링크 옵션 
# 동적 링크 라이브러리
LDFLAGS = -lpthread -static
 
# 정적 링크 라이브러리
LIBS = 
 

#---------------------------------------------------
# 소스 파일들
#---------------------------------------------------
SRCS = main.c ./header/http_request/http_request.c ./header/thread/thread.c

#---------------------------------------------------
# 실행 파일 이름
#---------------------------------------------------
TARGET = main

# 목적 파일들
OBJS = $(SRCS:.c=.o)

# 기본 빌드 타겟
all: $(TARGET)

# 실행 파일 빌드 규칙
$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LIBS) $(LDFLAGS) 


# 개별 소스 파일에 대한 목적 파일 생성 규칙
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ 

# 클린업 규칙
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
