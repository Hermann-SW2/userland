OBJS=encode.o
BIN=i420toh264.bin
LDFLAGS+=-lilclient

include ../Makefile.include

all:
	gcc -Wall -pedantic -Wextra sample_yuv2grey.c -o sample_yuv2grey
	gcc -Wall -pedantic -Wextra sample_yuv_brightness.c -o sample_yuv_brightness

clean3:
	rm -f sample_yuv2grey sample_yuv_brightness
	make clean
