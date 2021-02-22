#ifndef s3_lock_included
#define s3_lock_included
//----------------------------------------------------------------------

#ifdef S3_WIN32

  //#include <windows.h>

  class S3_Lock {
    private:
      CRITICAL_SECTION  MCriticalSection;

    public:

      S3_Lock() {
        InitializeCriticalSection(&MCriticalSection);
      }

      ~S3_Lock() {
        DeleteCriticalSection(&MCriticalSection);
      }

      void lock(void) {
        EnterCriticalSection(&MCriticalSection);
      }

      bool tryLock(void) {
        return TryEnterCriticalSection( &MCriticalSection );
      }

      void unlock(void) {
        LeaveCriticalSection(&MCriticalSection);
      }

  };

#endif // S3_WIN32

//----------------------------------------------------------------------

#ifdef S3_LINUX

  // -lpthread
  //#include <pthread.h>

  class S3_Lock {
    private:
      pthread_mutex_t MMutex; // = PTHREAD_MUTEX_INITIALIZER;

    public:

      S3_Lock() {
        pthread_mutex_init(&MMutex,S3_NULL);
      }

      ~S3_Lock() {
        pthread_mutex_destroy(&MMutex);
      }

      void lock(void) {
        pthread_mutex_lock(&MMutex);
      }

      bool tryLock(void) {
        return (pthread_mutex_trylock(&MMutex)==0);
      }

      void unlock(void) {
        pthread_mutex_unlock(&MMutex);
      }

  };

#endif

//----------------------------------------------------------------------
#endif

