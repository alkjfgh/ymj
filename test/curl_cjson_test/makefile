CC = gcc
CFLAGS = -g -Wall -DCURL_STATICLIB
LDFLAGS = -static

# curl 설치 경로
LIB_PATH = /usr/local

# 아키텍처
ARCH = aarch64-linux-gnu

# 시스템 라이브러리 경로
SYS_LIB = /usr/lib/$(ARCH)

# 헤더 파일 경로
INCLUDES = -I$(LIB_PATH)/include

# 라이브러리 (순서 중요)
LIBS = $(LIB_PATH)/lib/libcurl.a \
       $(LIB_PATH)/lib/libcjson.a \
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
       -lpthread -lz -ldl

# 타겟
TARGET = test

# 소스 파일
SRCS = test.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)