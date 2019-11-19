#include <time.h>

static struct timespec time0 = { .tv_sec=0, .tv_nsec=0 };

unsigned long millis() 
{
  struct timespec time1;
  clock_gettime(CLOCK_REALTIME, &time1);

  if (time0.tv_sec==0 && time0.tv_nsec==0)
  {
    time0.tv_sec =time1.tv_sec;
    time0.tv_nsec=time1.tv_nsec;
  }

  return 1000*(time1.tv_sec-(long long)time0.tv_sec) + (time1.tv_nsec-(long)time0.tv_nsec)/1000000;  
}
