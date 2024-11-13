# 컴파일러 설정
CC = gcc

# 컴파일 옵션
CFLAGS = -g -Wall -O2 -DCURL_STATICLIB
INCLUDES = -I$(LIB_PATH)/include
#CFLAGS = -g -I./mongoose

# 기본 경로 설정
LIB_PATH ?= /usr/local
ARCH ?= $(shell uname -m)-linux-gnu
SYS_LIB ?= /usr/lib/$(ARCH)


# 링크 옵션 
# 동적 링크 라이브러리
# LDFLAGS = -lpthread -ljson-c -static
LDFLAGS = -static

# 라이브러리 (순서 중요)
LIBS = $(LIB_PATH)/lib/libcurl.a \
	$(LIB_PATH)/lib/libcares.a \
	$(LIB_PATH)/lib/libnghttp2.a \
	$(SYS_LIB)/libpsl.a \
	$(SYS_LIB)/libidn2.a \
	$(SYS_LIB)/libssl.a \
	$(SYS_LIB)/libcrypto.a \
	$(SYS_LIB)/libzstd.a \
	$(SYS_LIB)/libbrotlidec.a \
	$(SYS_LIB)/libbrotlienc.a \
	$(SYS_LIB)/libbrotlicommon.a \
	$(SYS_LIB)/libunistring.a \
	$(SYS_LIB)/libz.a \
	$(SYS_LIB)/libjson-c.a \
	$(SYS_LIB)/libsodium.a \
	-lpthread -ldl

# 타겟
TARGET = test

# 소스 파일
SRCS = test.c globaldata.c libconfig.c libdebug.c libwww_curl.c processCommand.c sendCommandResponse.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)