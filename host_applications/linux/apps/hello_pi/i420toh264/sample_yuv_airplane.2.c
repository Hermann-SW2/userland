#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct { int y,u,v; }
  red  = { .y=82, .v=240, .u=90 },
  blue = { .y=41, .v=110, .u=240 };

int align_up(int x, int y)
{
  return ((x-1)|((1<<y)-1))+1;
}

int min(int a, int b) { return a<b ? a : b; }
int max(int a, int b) { return a>b ? a : b; }

int main(int argc, char *argv[])
{
  int width, height, nStride, nSliceHeight, nLenY, nLenU, nLenV, B, K, W, Q, T;
  char *buf;
  assert(argc==8 || !"Format: sample_yuv_airplane width height B W K Q T");

  width  = atoi(argv[1]); nStride      = align_up(width,  5);
  height = atoi(argv[2]); nSliceHeight = align_up(height, 4);
  B      = atoi(argv[3]);
  W      = atoi(argv[4]);
  K      = atoi(argv[5]);
  Q      = atoi(argv[6]);
  T      = atoi(argv[7]);

  nLenY = nStride * nSliceHeight;  nLenU = nLenV = nLenY / 4;
  assert( (buf = malloc(nLenY)) );

  fread(buf, nLenY, 1, stdin);
  while (!feof(stdin))
  {
    unsigned char *L;
    int m=256, r=-1, c=-1, X, Y, N;

    for(int i=B; i>=0; --i)
    {
      int miq=255;
      unsigned char *q = (unsigned char*)buf + i*nStride;
      for(int j=W; j<width-W; ++j)
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

      if (i == 0)
      {
        r = -1;
      }
    }
 
    if (r < 0)
    {
      fprintf(stderr, "not found\n");
      fwrite(buf, nLenY, 1, stdout); fflush(stdout);
      fread(buf, nLenU, 1, stdin);
      fwrite(buf, nLenU, 1, stdout); fflush(stdout);
      fread(buf, nLenV, 1, stdin);
      fwrite(buf, nLenV, 1, stdout); fflush(stdout);
      fread(buf, nLenY, 1, stdin);
      continue;
    }
 
    L = (unsigned char *)buf + r * nStride;
    for(int i=W; i<width-W; ++i)
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
    Q -= (Q & 0x03);
    X = Y = N = 0;
    for(int y=max(r-Q/2,0); y<min(r+Q/2,height-1); y+=2)
    {
      L = (unsigned char *)buf + y * nStride;

      for(int x=max(c-Q/2,0); x<=min(c+Q/2,width-1); x+=2)
      {
        if (L[x]>=T)
        {
          L[x+0] = 255;
          L[x+0+nStride] = 255;
          L[x+1+nStride] = 255;
          L[x+1] = 255;
        } else {
          X += x;
          Y += y;
          ++N;
        }
      }
    }

    r = Y/N;
    c = X/N;
    c -= (c & 0x01);
    r -= (r & 0x01);

    L = (unsigned char *)buf + r * nStride;
    L[c+0] = blue.y;
    L[c+0+nStride] = blue.y;
    L[c+1+nStride] = blue.y;
    L[c+1] = blue.y;

    fwrite(buf, nLenY, 1, stdout); fflush(stdout);

    fread(buf, nLenU, 1, stdin);
    L = (unsigned char*)buf + r/2 * nStride/2;
    L[c/2] = blue.u;
    fwrite(buf, nLenU, 1, stdout); fflush(stdout);

    fread(buf, nLenV, 1, stdin);
    L = (unsigned char*)buf + r/2 * nStride/2;
    L[c/2] = blue.v;
    fwrite(buf, nLenV, 1, stdout); fflush(stdout);

    fread(buf, nLenY, 1, stdin);
  }

  free(buf);

  return 0;
}
