#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define max ((3280+16)*2464*3/2)

int align_up(int x, int y)
{
  return ((x-1)|(y-1))+1;
}

int main(int argc, char *argv[])
{
  int width, height, nStride, nSliceHeight, nLenY, nLenU, nLenV;
  char *buf, *HLF;
  assert(argc==3 || !"Format: YUV2Yuv width height");

  width  = atoi(argv[1]); nStride      = align_up(width,  32);
  height = atoi(argv[2]); nSliceHeight = align_up(height, 16);

  nLenY = nStride * nSliceHeight;  nLenU = nLenV = nLenY / 4;
  assert( (buf = malloc(nLenY)) );
  assert( (HLF = malloc(nLenU)) );
  memset(HLF, 0x80, nLenU);

  fread(buf, nLenY, 1, stdin);
  while (!feof(stdin))
  {
    int y;
    long sum=0;

    fwrite(buf, nLenY, 1, stdout); fflush(stdout);

    for(y=0; y<height; y++)
    {
      unsigned char *p = (unsigned char*)(buf + y * nStride);
      for(int x=0; x<width; ++x)
      {
        sum += *p++;
      }
    }
    fprintf(stderr, "%s\n", sum < (width * height * 128L) ? "dark" : "bright");

    fread(buf, nLenU, 1, stdin);
    fwrite(buf, nLenU, 1, stdout); fflush(stdout);

    fread(buf, nLenV, 1, stdin);
    fwrite(buf, nLenV, 1, stdout); fflush(stdout);

    fread(buf, nLenY, 1, stdin);
  }

  free(buf); free(HLF);

  return 0;
}
