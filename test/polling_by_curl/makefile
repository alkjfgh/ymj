CC = gcc
CFLAGS = -g -Wall -DCURL_STATICLIB
LDFLAGS = -static

# curl 설치 경로
LIB_PATH = /usr/local

# 헤더 파일 경로
INCLUDES = -I$(LIB_PATH)/include -I$(LIB_PATH)/include -I../../header

# 라이브러리 (순서 중요)
LIBS = $(LIB_PATH)/lib/libcurl.a \
       $(LIB_PATH)/lib/libcjson.a \
       $(LIB_PATH)/lib/libcares.a \
       $(LIB_PATH)/lib/libnghttp2.a \
       /usr/lib/x86_64-linux-gnu/libidn2.a \
       /usr/lib/x86_64-linux-gnu/libpsl.a \
       /usr/lib/x86_64-linux-gnu/libssl.a \
       /usr/lib/x86_64-linux-gnu/libcrypto.a \
       /usr/lib/x86_64-linux-gnu/libzstd.a \
       /usr/lib/x86_64-linux-gnu/libbrotlidec.a \
       /usr/lib/x86_64-linux-gnu/libbrotlienc.a \
       /usr/lib/x86_64-linux-gnu/libbrotlicommon.a \
       /usr/lib/x86_64-linux-gnu/libunistring.a \
       -lpthread -lz -ldl

# 타겟
TARGET = test

# 소스 파일
SRCS = test.c ../../header/thread/thread.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)