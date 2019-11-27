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
  int width, height, nStride, nSliceHeight, nLenY, nLenU, nLenV;
  int x1, y1, x2, y2, w, d, x, y, W;
  char *buf, *p, *q1, *q2;
  assert(argc==4 || !"Format: sample_yuv_dbl_roi width height x1,y1,x2,y2,w,d");

  width  = atoi(argv[1]); nStride      = align_up(width,  5);
  height = atoi(argv[2]); nSliceHeight = align_up(height, 4);

  assert(6 == sscanf(argv[3], "%d,%d,%d,%d,%d,%d", &x1, &y1, &x2, &y2, &w, &d));
  x1 ^= (x1 & 1); y1 ^= (y1 & 1); x2 ^= (x2 & 1); y2 ^= (y2 & 1);
  W = align_up(2*w, 5);
  d = align_up(d, 4);

  nLenY = nStride * nSliceHeight;  nLenU = nLenV = nLenY / 4;
  assert( (buf = malloc(nLenY)) );
  assert(W*d<=nLenY);
  assert(w>=0); assert(2*w<=width); assert(d>=0); assert(d<=height);
  assert(x1>=0); assert(x1+w<=x2); assert(x2+w<=width);
  assert(y1>=0); assert(y1+d<=height); assert(y2>=0); assert(y2+d<=height);

  fread(buf, nLenY, 1, stdin);
  while (!feof(stdin))
  {
    for(p=buf, q1=buf+y1*nStride+x1, q2=buf+y2*nStride+x2, y=0;
        y<d; 
        q1+=nStride-w, q2+=nStride-w, y++)
    {
      for(x=0; x<w; ++x)
        *p++ = *q1++;
      for(x=0; x<w; ++x)
        *p++ = *q2++;
      p += (W-2*w);
    }
    assert(p-buf==W*d);
    fwrite(buf, W*d, 1, stdout); fflush(stdout);

    fread(buf, nLenU, 1, stdin);
    for(p=buf, q1=buf+(y1/2*nStride+x1)/2, q2=buf+(y2/2*nStride+x2)/2, y=0;
        y<d/2; 
        q1+=(nStride-w)/2, q2+=(nStride-w)/2, y++)
    {
      for(x=0; x<w/2; ++x)
        *p++ = *q1++;
      for(x=0; x<w/2; ++x)
        *p++ = *q2++;
      p += (W/2-w);
    }
    assert(p-buf==W*d/4);
    fwrite(buf, W*d/4, 1, stdout); fflush(stdout);

    fread(buf, nLenV, 1, stdin);
    for(p=buf, q1=buf+(y1/2*nStride+x1)/2, q2=buf+(y2/2*nStride+x2)/2, y=0;
        y<d/2; 
        q1+=(nStride-w)/2, q2+=(nStride-w)/2, y++)
    {
      for(x=0; x<w/2; ++x)
        *p++ = *q1++;
      for(x=0; x<w/2; ++x)
        *p++ = *q2++;
      p += (W/2-w);
    }
    assert(p-buf==W*d/4);
    fwrite(buf, W*d/4, 1, stdout); fflush(stdout);

    fread(buf, nLenY, 1, stdin);
  }

  free(buf);

  return 0;
}
