# 컴파일러 설정
CC = gcc
CFLAGS = -g -Wall -DCURL_STATICLIB
LDFLAGS = -static

# 기본 경로 설정
LIB_PATH = /usr/local
ARCH = x86_64-linux-gnu
SYS_LIB = /usr/lib/$(ARCH)

# 컴파일에 필요한 헤더 파일 경로
INCLUDES = -I$(LIB_PATH)/include

# 링킹할 라이브러리 목록
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

# 빌드 대상 설정
TARGET = test
SRCS = test.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)