#include <stdio.h>
#include <math.h>

int main()
{
  int x,y,X=1640/2,Y=922/2;
  
  printf("P5\n# CREATOR: 1640x922.fine_crosshair_900.c\n1640 922\n255\n");

  for(y=0; y<922; ++y)
  {
    for(x=0; x<1640; ++x)
    {
      int r = sqrt((x-X)*(x-X)+(y-Y)*(y-Y));
      printf("%c", (r==450 | (r<450 && (x==X || y==Y)))?0x00:0xFF);
    }
  }

  return 0;
}
