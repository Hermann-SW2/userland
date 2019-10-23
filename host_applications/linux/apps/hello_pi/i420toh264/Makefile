OBJS=encode.o
BIN=i420toh264.bin
LDFLAGS+=-lilclient

include ../Makefile.include

all:
	gcc -Wall -pedantic -Wextra sample_yuv2grey.c -O6 -o sample_yuv2grey
	gcc -Wall -pedantic -Wextra sample_yuv_brightness.c -O6 -o sample_yuv_brightness
	gcc -Wall -pedantic -Wextra sample_yuv_alpha.c -O6 -o sample_yuv_alpha

CLEAN:
	rm -f sample_yuv2grey sample_yuv_brightness sample_yuv_alpha
	make clean
