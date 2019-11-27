OBJS=encode.o
BIN=i420toh264.bin
LDFLAGS+=-lilclient

include ../Makefile.include

all:
	gcc -Wall -pedantic -Wextra sample_yuv2grey.c -O6 -o sample_yuv2grey
	gcc -Wall -pedantic -Wextra sample_yuv_dbl_roi.c -O6 -o sample_yuv_dbl_roi
	gcc -Wall -pedantic -Wextra sample_yuv_brightness.c -O6 -o sample_yuv_brightness
	gcc -Wall -pedantic -Wextra sample_yuv_alpha.c -O6 -o sample_yuv_alpha
	gcc -Wall -pedantic -Wextra sample_yuv_airplane.c -O6 -o sample_yuv_airplane
	gcc -Wall -Wextra sample_yuv_airplane.2.c -O6 -o sample_yuv_airplane.2 -lpigpio -lpthread
	gcc -Wall -Wextra sample_yuv_pts.c -O6 -o sample_yuv_pts -lpigpio -lpthread
	g++ -Wall -Wextra PID_test.cpp Arduino.cpp Arduino-PID-Library/PID_v1.cpp -o PID_test

CLEAN:
	rm -f sample_yuv2grey sample_yuv_dbl_roi sample_yuv_brightness sample_yuv_alpha sample_yuv_airplane sample_yuv_airplane.2 sample_yuv_pts
	make clean
