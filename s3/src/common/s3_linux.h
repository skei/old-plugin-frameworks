#ifndef s3_linux_included
#define s3_linux_included
//----------------------------------------------------------------------

#include <pthread.h>

#ifdef S3_XLIB
  #include "common/s3_xlib.h"
#endif

#ifdef S3_XRENDER
  #include "common/s3_xrender.h"
#endif

#ifdef S3_XFT
  #include "common/s3_xft.h"
#endif

//----------------------------------------------------------------------

#include <time.h>

// usleep is deprecated

int S3_Sleep(long ms) {
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


//----------------------------------------------------------------------
#endif
