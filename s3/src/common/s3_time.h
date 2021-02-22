#ifndef s3_time_included
#define s3_time_included
//----------------------------------------------------------------------

//#include <iostream>
//#include <ctime>
//#include <unistd.h>
#include <sys/time.h>
#include <unistd.h> // sleep


class S3_Time {
  private:

      //__time_t tv_sec;        // long int
      //__suseconds_t tv_usec;  // long int

    timeval MCreateTime;
    timeval MStartTime;
    timeval MTempTime;
  public:
    S3_Time();
    ~S3_Time();
    void  restart(void);
    int32 sec(void);
    int32 us(void);
    int32 ms(void);
};

void S3_Sleep(uint32 ms);

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

S3_Time::S3_Time() {
  gettimeofday(&MCreateTime,0);
  restart();
}

S3_Time::~S3_Time() {
}

void S3_Time::restart(void) {
  gettimeofday(&MStartTime,0);
}

int32 S3_Time::sec(void) {
  gettimeofday(&MTempTime,0);
  int32 sec = (MTempTime.tv_sec - MStartTime.tv_sec);
  return sec;
}

int32 S3_Time::ms(void) {
  gettimeofday(&MTempTime,0);
  //int32 ms = (sec() * 1000) + (us() / 1000);
  //return us() / 1000L;
  int32 ms = (MTempTime.tv_sec  - MStartTime.tv_sec) * 1000L
           + (MTempTime.tv_usec - MStartTime.tv_usec) / 1000L;
  return ms;
}

int32 S3_Time::us(void) {
  gettimeofday(&MTempTime,0);
  int32 us = (MTempTime.tv_sec  - MStartTime.tv_sec) * 1000000L
           + (MTempTime.tv_usec - MStartTime.tv_usec);
  return us;
}


//----------------------------------------------------------------------

void S3_Sleep(uint32 ms) {
  usleep(ms*1000); // ms*1000;
}

//----------------------------------------------------------------------
#endif
