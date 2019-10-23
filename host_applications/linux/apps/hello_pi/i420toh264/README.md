# i420toh264

Tool i420toh264 was built from [hello_pi/hello_encode](../hello_encode) with only few changes, see [diff for commit d4a5f005d91116fe1fa747c07ab1aeb75d5c732b](https://github.com/Hermann-SW2/userland/commit/d4a5f005d91116fe1fa747c07ab1aeb75d5c732b).  

It allows to convert [raspividyuv](https://www.raspberrypi.org/documentation/raspbian/applications/camera.md) video to .h264 encoded video.  
Reason for not using raspivid to create .h264 encoded video directly is, that in pipeline a "plugin" can process each and every YUV frame of the video stream easily, and either analyze+control and/or modify the frames, before final conversion to .h264.

All you need to know about [raspividyuv](https://www.raspberrypi.org/documentation/raspbian/applications/camera.md) frame [I420](https://fourcc.org/pixel-format/yuv-i420/) memory layout for processing:  
* each Y or U or V pixel value is stored as 1 byte  
* Y pixel values are followed by U and then V pixel values
* rows are stored from top to bottom  
* pixels in a row are stored from left to right  
* align_up(a,b) is minimal multiple of b which is ≥a  
![YUV frame layout](YUV_frame.png)

## Building on Pi

To build i420toh264 on the pi, first build the libs:

    pushd /opt/vc/src/hello_pi
    make -C libs/ilclient
    make -C libs/vgfont
    popd

then in this directory, build i420toh264 and samples:

    make

## Execution on Pi

    $ ./i420toh264
    Usage: ./i420toh264 <filename> <width> <height>
    $
or

    ./i420toh264.bin

### Minimal use in pipeline

Record with raspividyuv on v2 camera, create tst.h264:

    raspividyuv -md 5 -w 1640 -h 922 -o - | ./i420toh264 tst.h264 1640 922

### Simple non-modifying pipeline

[sample_yuv_brightness.c](sample_yuv_brightness.c) receives yuv video frames from raspividyuv, and sends them unmodified to i420toh264 tool ([encode.c](encode.c)), which creates tst.h264. It analyzes the brightness of each frame by inspecting all frame pixels and outputs either "dark" or "bright". For example run below scene was dark, and light was turned on and off.  
Similar "plugin" can be used to detect airplane in the sky and control pan tilt camera system servos for always centered recording of the airplane.

    $ raspividyuv -md 5 -w 1640 -h 922 -o - -t 8000 -awb greyworld -fps 2 | \
    > ./sample_yuv_brightness 1640 922 | \
    > ./i420toh264 tst.h264 1640 922 > /dev/null
    bright
    bright
    dark
    dark
    dark
    bright
    bright
    dark
    dark
    bright
    $ 

### Simple modifying pipeline

[sample_yuv2grey.c](sample_yuv2grey.c) receives YUV video frames from raspividyuv, and sends them modified to i420toh264 tool ([encode.c](encode.c)), which creates tst.h264. It keeps Y value, but sets U and V values to 128 for all pixels, which is "to grey conversion" of input frame.  
Of course a modifying "plugin" can control eg. servos as well.

    $ time ( \
    > raspividyuv -md 5 -w 1640 -h 922 -o - -t 15000 -fps 25 | \
    > ./sample_yuv2grey 1640 922 | \
    > ./i420toh264 tst.h264 1640 922 > /dev/null \
    > )
    
    real	0m15.238s
    user	0m0.311s
    sys	0m13.464s
    $ 

