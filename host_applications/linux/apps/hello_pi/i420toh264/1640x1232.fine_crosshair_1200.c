#include <stdio.h>
#include <math.h>

int main()
{
  int x,y,X=1640/2,Y=1232/2;
  
  printf("P5\n# CREATOR: 1640x1232.fine_crosshair_1200.c\n1640 1232\n255\n");

  for(y=0; y<1232; ++y)
  {
    for(x=0; x<1640; ++x)
    {
      int r = sqrt((x-X)*(x-X)+(y-Y)*(y-Y));
      printf("%c", (r==600 | (r<600 && (x==X || y==Y)))?0x00:0xFF);
    }
  }

  return 0;
}
