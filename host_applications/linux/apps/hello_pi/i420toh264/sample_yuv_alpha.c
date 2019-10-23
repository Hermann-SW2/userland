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
  int width, height, nStride, nSliceHeight, nLenY, nLenU, nLenV, x, y;
  unsigned char *buf, *alpha, *p, *q;
  char line[1000];
  FILE *src;
  assert(argc==4 || !"Format: YUV2Yuv width height alpha.pgm");

  width  = atoi(argv[1]); nStride      = align_up(width,  32);
  height = atoi(argv[2]); nSliceHeight = align_up(height, 16);

  nLenY = nStride * nSliceHeight;  nLenU = nLenV = nLenY / 4;
  assert( (buf = malloc(nLenY)) );
  assert( (alpha = malloc(nLenY)) );

  assert( (src = fopen(argv[3], "rb")) );
  assert( (fgets(line, 1000, src)) ); 
  assert( strcmp(line,"P5\n")==0 );

  assert( (fgets(line, 1000, src)) ); 
  while (line[0]=='#') { assert( (fgets(line, 1000, src)) ); }
  assert( strcmp(line,"1640 922\n")==0 );

  assert( (fgets(line, 1000, src)) ); 
  while (line[0]=='#') { assert( (fgets(line, 1000, src)) ); }
  assert( strcmp(line,"255\n")==0 );

  assert( 1 == fread(alpha, 1640*922, 1, src) );
  fclose(src);

  fread(buf, nLenY, 1, stdin);
  while (!feof(stdin))
  {
    for(y=0; y<height; ++y)
    {
      for(p=buf+y*nStride, q=alpha+y*width, x=0; x<width; ++x)
      {
        p[x] = (((unsigned)p[x]) * q[x]) / 255;
      }
    }
    fwrite(buf, nLenY, 1, stdout); fflush(stdout);

    fread(buf, nLenU, 1, stdin);
    fwrite(buf, nLenU, 1, stdout); fflush(stdout);

    fread(buf, nLenV, 1, stdin);
    fwrite(buf, nLenV, 1, stdout); fflush(stdout);

    fread(buf, nLenY, 1, stdin);
  }

  free(buf); free(alpha);

  return 0;
}
