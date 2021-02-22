#ifndef kode_linux_time_included
#define kode_linux_time_included
//----------------------------------------------------------------------

/*
  The C library function clock_t clock(void) returns the number of clock ticks
  elapsed since the program was launched. To get the number of seconds used by
  the CPU, you will need to divide by CLOCKS_PER_SEC.
  On a 32 bit system where CLOCKS_PER_SEC equals 1000000 this function will
  return the same value approximately every 72 minutes.
*/

#include <time.h>
#include "base/kode_const.h"

// CLOCKS_PER_SEC

//----------------------------------------------------------------------

class KODE_Time {

  public:

    KODE_Time() {}
    ~KODE_Time() {}

  public:

    double start(void) {
      MStartTime = clock();
      return MStartTime;
    }

    /*
      wraparound? negative?
      if clock has wrapped around since start() was called, we would
      clock_t -> long int
    */

    double sec(void) {
      clock_t currenttime = clock();
      double elapsed = currenttime - MStartTime;
      return elapsed / CLOCKS_PER_SEC; // 1000000;
    }

    double ms(void) {
      return sec() * 1000.0;
    }

    double us(void) {
      return sec() * 1000000.0;
    }

  private:
      clock_t MStartTime = 0; // cppcheck
};

//----------------------------------------------------------------------

struct KODE_CurrentTime {
  int32_t year;
  int32_t month;
  int32_t day;
  int32_t hour;
  int32_t minutes;
  int32_t seconds;
};

//int     KODE_Sleep(long ms);
//void    KODE_GetTime(KODE_CurrentTime* ATime);
//double  KODE_GetTimeMS(void);

//----------------------------------------------------------------------

// usleep is deprecated

//void KSleep(KODE_ui32 ms) {
//  usleep(ms*1000); // ms*1000;
//}

int KODE_Sleep(long ms) {
  struct timespec req, rem;
  if (ms > 999) {
      req.tv_sec = (int)(ms / 1000);                            /* Must be Non-Negative */
      req.tv_nsec = (ms - ((long)req.tv_sec * 1000)) * 1000000; /* Must be in range of 0 to 999999999 */
  }
  else {
      req.tv_sec = 0;                         /* Must be Non-Negative */
      req.tv_nsec = ms * 1000000;    /* Must be in range of 0 to 999999999 */
  }
  return nanosleep(&req , &rem);
}

//----------

void KODE_GetLocalTime(KODE_CurrentTime* ATime) {
  time_t t       = time(KODE_NULL);
  struct tm tm   = *localtime(&t); // cppcheck recommends localtime_r
  ATime->year    = tm.tm_year + 1900;
  ATime->month   = tm.tm_mon + 1;
  ATime->day     = tm.tm_mday;
  ATime->hour    = tm.tm_hour;
  ATime->minutes = tm.tm_min;
  ATime->seconds = tm.tm_sec;
  //KTrace("now: %d-%d-%d %d:%d:%d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
  //return &CODE_CURRENT_TIME;
}

//----------

double KODE_GetTimeMS(void) {
  clock_t curtime = clock();
  return (double)(curtime * 1000.0) / CLOCKS_PER_SEC;
  //return (double)curtime;
}

//----------------------------------------------------------------------
#endif

/*

//#include <iostream>
//#include <ctime>
//#include <unistd.h>
#include <sys/time.h>
#include <unistd.h> // sleep

class KTime {

private:

  //__time_t tv_sec;        // long int
  //__suseconds_t tv_usec;  // long int

  timeval   MCreateTime;
  timeval   MStartTime;
  timeval   MTempTime;

public:

  KTime() {
    gettimeofday(&MCreateTime,0);
    restart();
  }

  ~KTime() {
  }

  void restart(void) {
    gettimeofday(&MStartTime,0);
    KTrace("restart:\n");
    KTrace("  MStartTime.tv_sec %i\n", MStartTime.tv_sec);
    KTrace("  MStartTime.tv_usec %i\n",MStartTime.tv_usec);
  }

  double sec(void) {
    gettimeofday(&MTempTime,0);
    KODE_i32 sec = (MTempTime.tv_sec - MStartTime.tv_sec);
    return sec;
  }

  double us(void) {
    gettimeofday(&MTempTime,0);
    double ms = ((double)(MTempTime.tv_sec  - MStartTime.tv_sec) * 1000.0)
               + (double)(MTempTime.tv_usec - MStartTime.tv_usec) / 1000.0;
    return ms;
  }

  double ms(void) {
    gettimeofday(&MTempTime,0);
    KTrace("ms:\n");
    KTrace("  MTempTime.tv_sec %i\n", MTempTime.tv_sec);
    KTrace("  MTempTime.tv_usec %i\n",MTempTime.tv_usec);
    double us = ((double)(MTempTime.tv_sec  - MStartTime.tv_sec) * 1000000.0)
               + (double)(MTempTime.tv_usec - MStartTime.tv_usec);
    return us;
  }

};

*/