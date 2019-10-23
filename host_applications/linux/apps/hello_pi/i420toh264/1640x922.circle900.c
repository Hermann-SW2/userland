#include <stdio.h>
#include <math.h>

int main()
{
  int x,y,X=1640/2,Y=922/2,X2=1000,Y2=700;
  
  printf("P5\n# CREATOR: 1640x922.circle900.c\n1640 922\n255\n");

  for(y=0; y<922; ++y)
  {
    for(x=0; x<1640; ++x)
    {
      double r = sqrt((x-X)*(x-X)+(y-Y)*(y-Y));
      double r2 = sqrt((x-X2)*(x-X2)+(y-Y2)*(y-Y2));
      unsigned char ch = (r>450)?0x00:(r>250)?(450-r)/200*255:0xFF;
      printf("%c", (r2<220)?0xFF:ch);
    }
  }

  return 0;
}
