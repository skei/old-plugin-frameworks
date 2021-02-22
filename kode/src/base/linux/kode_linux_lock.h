#ifndef kode_linux_lock_included
#define kode_linux_lock_included
//----------------------------------------------------------------------

//#ifdef KODE_LINUX

  // -lpthread
  //#include <pthread.h>

  class KODE_Lock {
    private:
      pthread_mutex_t MMutex; // = PTHREAD_MUTEX_INITIALIZER;

    public:

      KODE_Lock() {
        pthread_mutex_init(&MMutex,KODE_NULL);
      }

      ~KODE_Lock() {
        pthread_mutex_destroy(&MMutex);
      }

      void lock(void) {
        pthread_mutex_lock(&MMutex);
      }

      bool tryLock(void) {
        return (pthread_mutex_trylock(&MMutex) == 0);
      }

      void unlock(void) {
        pthread_mutex_unlock(&MMutex);
      }

  };

//#endif

//----------------------------------------------------------------------

//#ifdef KODE_WIN32
//
//  //#include <windows.h>
//
//  class KODE_Lock {
//    private:
//      CRITICAL_SECTION  MCriticalSection;
//
//    public:
//
//      KODE_Lock() {
//        InitializeCriticalSection(&MCriticalSection);
//      }
//
//      ~KODE_Lock() {
//        DeleteCriticalSection(&MCriticalSection);
//      }
//
//      void lock(void) {
//        EnterCriticalSection(&MCriticalSection);
//      }
//
//      bool tryLock(void) {
//        return TryEnterCriticalSection( &MCriticalSection );
//      }
//
//      void unlock(void) {
//        LeaveCriticalSection(&MCriticalSection);
//      }
//
//  };
//
//#endif // KODE_WIN32

//----------------------------------------------------------------------
#endif
