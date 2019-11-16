#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>

int align_up(int x, int y)
{
  return ((x-1)|((1<<y)-1))+1;
}

struct timespec time0 = { .tv_sec=0, .tv_nsec=0 };

long long diff(struct timespec *t)
{
  return 1000000*(t->tv_sec-(long long)time0.tv_sec) + (t->tv_nsec-(long)time0.tv_nsec)/1000;
}

int main(int argc, char *argv[])
{
  int width, height, nStride, nSliceHeight, nLenY, nLenU, nLenV;
  char *buf;
  FILE *fpts=NULL;
  assert(argc==4 || !"Format: sample_yuv_brightness width height file.pts");

  width  = atoi(argv[1]); nStride      = align_up(width,  5);
  height = atoi(argv[2]); nSliceHeight = align_up(height, 4);

  nLenY = nStride * nSliceHeight;  nLenU = nLenV = nLenY / 4;
  assert( (buf = malloc(nLenY)) );

  fread(buf, nLenY, 1, stdin);
  while (!feof(stdin))
  {
    struct timespec time1;
    clock_gettime(CLOCK_REALTIME, &time1);
    if (!fpts)
    {
      assert( (fpts=fopen(argv[3], "w"))!=NULL );
      fprintf(fpts, "# timecode format v2\n");
      fprintf(fpts, "%lld.%03lld\n", diff(&time1)/1000, diff(&time1)%1000);
      time0.tv_sec  = time1.tv_sec;
      time0.tv_nsec = time1.tv_nsec;
    } else {
      fprintf(fpts, "%lld.%03lld\n", diff(&time1)/1000, diff(&time1)%1000);
    }

    fwrite(buf, nLenY, 1, stdout); fflush(stdout);

    fread(buf, nLenU, 1, stdin);
    fwrite(buf, nLenU, 1, stdout); fflush(stdout);

    fread(buf, nLenV, 1, stdin);
    fwrite(buf, nLenV, 1, stdout); fflush(stdout);

    fread(buf, nLenY, 1, stdin);
  }

  free(buf);
  if (!fpts)
    fclose(fpts);

  return 0;
}
