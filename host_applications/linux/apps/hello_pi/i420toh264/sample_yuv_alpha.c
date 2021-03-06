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
  int width, height, nStride, nSliceHeight, nLenY, nLenU, nLenV, x, y;
  unsigned char *buf, *alpha, *p, *q;
  char line[1000], temp[30];
  FILE *src;
  assert(argc==4 || !"Format: sample_yuv_alpha width height alpha.pgm");

  width  = atoi(argv[1]); nStride      = align_up(width,  5);
  height = atoi(argv[2]); nSliceHeight = align_up(height, 4);

  nLenY = nStride * nSliceHeight;  nLenU = nLenV = nLenY / 4;
  assert( (buf = malloc(nLenY)) );
  assert( (alpha = malloc(nLenY)) );

  assert( (src = fopen(argv[3], "rb")) );
  assert( (fgets(line, 1000, src)) ); 
  assert( strcmp(line,"P5\n")==0 );

  assert( (fgets(line, 1000, src)) ); 
  while (line[0]=='#') { assert( (fgets(line, 1000, src)) ); }
  sprintf(temp, "%d %d\n", nStride, nSliceHeight);
fprintf(stderr, line);
fprintf(stderr, temp);
  assert( strcmp(line, temp)==0 );

  assert( (fgets(line, 1000, src)) ); 
  while (line[0]=='#') { assert( (fgets(line, 1000, src)) ); }
  assert( strcmp(line,"255\n")==0 );

  assert( 1 == fread(alpha, nLenY, 1, src) );
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
