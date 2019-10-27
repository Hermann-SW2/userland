#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

int align_up(int x, int y)
{
  return ((x-1)|((1<<y)-1))+1;
}

int main(int argc, char *argv[])
{
  int width, height, nStride, nSliceHeight, nLenY, nLenU, nLenV, B, K;
  char *buf;
  assert(argc==5 || !"Format: sample_yuv_airplane width height B K");

  width  = atoi(argv[1]); nStride      = align_up(width,  5);
  height = atoi(argv[2]); nSliceHeight = align_up(height, 4);
  B      = atoi(argv[3]);
  K      = atoi(argv[4]);

  nLenY = nStride * nSliceHeight;  nLenU = nLenV = nLenY / 4;
  assert( (buf = malloc(nLenY)) );

  fread(buf, nLenY, 1, stdin);
  while (!feof(stdin))
  {
    unsigned char *L;
    int m=256, r=-1, c=-1;

    for(int i=B; ; --i)
    {
      int miq=255;
      unsigned char *q = (unsigned char*)buf + i*nStride;
      for(int j=0; j<width; ++j)
      {
        if (q[j]<miq)  miq=q[j];
      }
  
      // fprintf(stderr, "%3d %3d\n", i, miq);
  
      if (miq > m+K)
        break;
  
      if (miq < m) 
      {
        m = miq;
        r = i;
      }
    }
  
    L = (unsigned char *)buf + r * nStride;
    for(int i=0; i<width; ++i)
    {
      if (L[i]==m)
      {
        c = i;
        break;
      }
    }

    fprintf(stderr, "%3d %3d %3d\n", r, c, m);

    // mark 2x2 area white
    //
    c -= (c & 0x01);
    r -= (r & 0x01);
    L[c+0] = 255;
    L[c+0+nStride] = 255;
    L[c+1+nStride] = 255;
    L[c+1] = 255;

    fwrite(buf, nLenY, 1, stdout); fflush(stdout);

    fread(buf, nLenU, 1, stdin);
    L = (unsigned char*)buf + r/2 * nStride/2;
    L[c/2] = 128;
    fwrite(buf, nLenU, 1, stdout); fflush(stdout);

    fread(buf, nLenV, 1, stdin);
    L = (unsigned char*)buf + r/2 * nStride/2;
    L[c/2] = 128;
    fwrite(buf, nLenV, 1, stdout); fflush(stdout);

    fread(buf, nLenY, 1, stdin);
  }

  free(buf);

  return 0;
}
