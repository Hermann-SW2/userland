// gcc -Wall -pedantic -Wextra sample_laserBeam.c -O3 -o sample_laserBeam
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// https://www.99colors.net/name/blue
const unsigned char y_blue =       255 * 0.11;
const unsigned char u_blue = 128 + 255 * 0.44;
const unsigned char v_blue = 128 - 255 * 0.10;

int align_up(int x, int y)
{
  return ((x-1)|((1<<y)-1))+1;
}

int main(int argc, char *argv[])
{
  int width, height, nStride, nSliceHeight, nLenY, nLenU, nLenV, xintc, yintc;
  unsigned char *buf, mx, fnd;
  int x, y, s, adjust;
  assert(argc==5 ||
         !"Format: sample_laserBeam width height x-intercept y-intercept");

  width  = atoi(argv[1]); nStride      = align_up(width,  5);
  height = atoi(argv[2]); nSliceHeight = align_up(height, 4);
  xintc  = atoi(argv[3]);
  yintc  = atoi(argv[4]);
  adjust = !strstr(argv[0], "sample_laserBeam");

  nLenY = nStride * nSliceHeight;  nLenU = nLenV = nLenY / 4;
  assert( (buf = malloc(nLenY+nLenU+nLenV)) );

  fread(buf, nLenY+nLenU+nLenV, 1, stdin);
  while (!feof(stdin))
  {
   if (!adjust)
   {
    for(mx=0, y=0, x=xintc, s=0; y<=yintc; ++y)
    {
      s += xintc;  if (s > yintc)  { s -= yintc; --x; }

      if (buf[x+y*nStride] > mx)  mx = buf[x+y*nStride];
    }

    for(fnd=0, y=0, x=xintc, s=0; y<=yintc; ++y)
    {
      s += xintc;  if (s > yintc)  { s -= yintc; --x; }

      if (buf[x+y*nStride] == mx) 
      {
        buf[x+y*nStride]                             = y_blue;
        buf[(x/2)+(y/2)*(nStride/2) + nLenY]         = u_blue;
        buf[(x/2)+(y/2)*(nStride/2) + nLenY + nLenU] = v_blue;
        fnd = 1;
      }
      else if (fnd)
        break; 
    }
   }
   else
   {
    for(y=0, x=xintc, s=0; y<=yintc; ++y)
    {
      s += xintc;  if (s > yintc)  { s -= yintc; --x; }

      buf[x+y*nStride]                             = y_blue;
      buf[(x/2)+(y/2)*(nStride/2) + nLenY]         = u_blue;
      buf[(x/2)+(y/2)*(nStride/2) + nLenY + nLenU] = v_blue;
    }
    // fprintf(stderr, "x=%d y=%d s=%d\n", x, y, s);
   }

    fwrite(buf, nLenY+nLenU+nLenV, 1, stdout); fflush(stdout);

    fread(buf, nLenY+nLenU+nLenV, 1, stdin);
  }

  free(buf);

  return 0;
}
