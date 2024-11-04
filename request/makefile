# 컴파일러 설정
CC = gcc

# 컴파일 옵션
CFLAGS = -Wall -g
LDFLAGS = -static

# 소스 파일들
SRCS = request.c

# 실행 파일 이름
TARGET = request

# 목적 파일들
OBJS = $(SRCS:.c=.o)

# 기본 빌드 타겟
all: $(TARGET)

# 실행 파일 빌드 규칙
$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJS)

# 개별 소스 파일에 대한 목적 파일 생성 규칙
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# 클린업 규칙
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean