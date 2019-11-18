#include <stdio.h>
#include <math.h>

int main()
{
  int x,y,X=640/2,Y=480/2;
  
  printf("P5\n# CREATOR: 640x480.fine_crosshair_600.c\n640 480\n255\n");

  for(y=0; y<480; ++y)
  {
    for(x=0; x<640; ++x)
    {
      int r = sqrt((x-X)*(x-X)+(y-Y)*(y-Y));
      printf("%c", (r==225 | (r<225 && (x==X || y==Y)))?0x00:0xFF);
    }
  }

  return 0;
}
