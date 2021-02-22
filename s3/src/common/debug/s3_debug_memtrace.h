#ifndef s3_debug_memtrace_included
#define s3_debug_memtrace_included
//----------------------------------------------------------------------

// parts of the code by lubomir i ivanov (for axonlib)
// used with permission

//----------------------------------------------------------------------

#ifdef S3_DEBUG_MEM
#ifndef S3_GDIPLUS // gdi+ redefines new, and messes up with ours..

#include <iostream>
//using namespace std;

#include "common/s3_utils.h"   // SStripPath

//----------------------------------------------------------------------

/*
  in case we want to change them...
  by not using SMalloc, etc, we avoid keeping track of the small allocs for
  the mem nodes, and also for the proper/real allocations
*/

#define s3_memtrace_malloc   malloc
#define s3_memtrace_calloc   calloc
#define s3_memtrace_realloc  realloc
#define s3_memtrace_free     free

//----------------------------------------------------------------------

struct s3_debug_memnode {
  s3_debug_memnode* MPrev;
  s3_debug_memnode* MNext;
  void*     MAddr;
  int       MSize;
  char      MFile[S3_MAX_STRING_SIZE+1];
  char      MFunc[S3_MAX_STRING_SIZE+1];
  int       MLine;
  bool      MNew;
};

//----------------------------------------------------------------------

class S3_Debug_MemTrace {

  private:

    s3_debug_memnode* MHead;
    s3_debug_memnode* MTail;
    int                 MAllocated;
    int                 MAllocatedNum;
    int                 MAllocatedMax;
    int                 MNumAlloc;
    int                 MNumFree;

  //----------------------------------------
  //
  //----------------------------------------

  public:

    S3_Debug_MemTrace() {
      MHead          = S3_NULL;
      MTail          = S3_NULL;
      MAllocated     = 0;
      MAllocatedNum  = 0;
      MAllocatedMax  = 0;
      MNumAlloc      = 0;
      MNumFree       = 0;
    }

    //----------

    ~S3_Debug_MemTrace() {
      dump();
      #ifdef S3_DEBUG_SLEEP_AFTER_MEMDUMP
      Sleep(S3_DEBUG_SLEEP_AFTER_MEMDUMP_MS);
      #endif

    }

  //----------------------------------------
  //
  //----------------------------------------

  private:

    // append to tail...
    void _appendNode(s3_debug_memnode* ANode) {
      if (MTail) {
        // xx XX --
        ANode->MPrev = MTail;
        ANode->MNext = S3_NULL;
        MTail->MNext = ANode;
        MTail = ANode;
      }
      else {
        // -- XX --
        MHead = ANode;
        ANode->MPrev = S3_NULL;
        ANode->MNext = S3_NULL;
        MTail = ANode;
      }
      MAllocated += ANode->MSize;
      MAllocatedNum++;
      if (MAllocated > MAllocatedMax) MAllocatedMax = MAllocated;
    }

    //----------

    // remove node
    void _removeNode(s3_debug_memnode* ANode) {
      s3_debug_memnode* next = ANode->MNext;
      s3_debug_memnode* prev = ANode->MPrev;
      if (prev) {
        if (next) {
          //  xx XX xx
          prev->MNext = next;
          next->MPrev = prev;
        }
        else {
          // xx XX -- (tail)
          prev->MNext = NULL;
          MTail = prev;
        }
      }
      else { // !head
        if (next) {
          // -- XX xx (head)
          MHead = next;
          next->MPrev = NULL;
        }
        else {
          // -- XX -- (single)
          MHead = NULL;
          MTail = NULL;
        }
      } // !next
      MAllocated -= ANode->MSize;
      MAllocatedNum--;
      //MFreedNum++;
    }

  //----------------------------------------
  //
  //----------------------------------------

  public:

    void append(void* AAddr, unsigned int ASize, const char* AFile, const char* AFunc, unsigned int ALine, bool ANew) {
      //if (MStarted) {
        const char* file = S3_StripPath(AFile);
        s3_debug_memnode* node = (s3_debug_memnode*)s3_memtrace_malloc(sizeof(s3_debug_memnode)); // !!!
        MNumAlloc++;
        node->MAddr = AAddr;
        node->MSize = ASize;
        S3_Strncpy(node->MFile, file ,S3_MAX_STRING_SIZE);
        S3_Strncpy(node->MFunc, AFunc ,S3_MAX_STRING_SIZE);
        node->MLine = ALine;
        node->MNew = ANew;
        _appendNode(node);
        #ifdef S3_DEBUG_MEM_PRINT
        DTrace("+ [%s:%i:%s] %s, 0x%x, %i\n",file,ALine,AFunc,(ANew?"new":"malloc"),AAddr,ASize);
        #endif
      //} // MStarted
    }

    //----------

    //void remove(void* AAddr, __unused const char* AFile, __unused const char* AFunc, __unused unsigned int ALine, __unused bool ANew) {
    void remove(void* AAddr, _S3_UNUSED const char* AFile, _S3_UNUSED const char* AFunc, _S3_UNUSED unsigned int ALine, _S3_UNUSED bool ANew) {
      //if (MStarted) {
        #ifdef S3_DEBUG_MEM_PRINT
        const char* file = S3_StripPath(AFile);
        #endif
        s3_debug_memnode* node = MHead;
        while (node) {
          if (node->MAddr==AAddr) break;
          node = (s3_debug_memnode*)node->MNext;
        }
        if (node) {
          _removeNode(node);
          #ifdef S3_DEBUG_MEM_PRINT
          DTrace("- [%s:%i:%s] %s, 0x%x, %i\n",file,ALine,AFunc,(ANew?"delete":"free"),node->MAddr,node->MSize);
          if (node->MNew != ANew) {
            DTrace("! mismatch!!! [%s:%i:%s] %s\n",node->MFile,node->MLine,node->MFunc,(node->MNew?"new":"malloc"));
          }
          #endif
          s3_memtrace_free(node); // !!!
          MNumFree++;
        }
        //else {
        //  DTrace("! node not found !");
        //  // and it will probably crash/segfault...
        //}
      //} // MStarted
    }

    //----------

    void dump(void) {
      S3_DTrace("\n----------------------------------------\n");
      S3_DTrace("memtrace\n");
      S3_DTrace("----------------------------------------\n");
      S3_DTrace("* num alloc     %i\n",MNumAlloc);
      S3_DTrace("* num free      %i\n",MNumFree);
      S3_DTrace("* num leaks     %i\n",MAllocatedNum);
      S3_DTrace("* max allocated %i\n",MAllocatedMax);
      S3_DTrace("* leaked memory %i\n",MAllocated);
      if (MAllocated > 0)
      {
        S3_DTrace("----------------------------------------\n\n");
        s3_debug_memnode* node = (s3_debug_memnode*)MHead;
        while (node) {
          S3_DTrace("* [%s:%s:%i] %s,0x%x,%i\n",node->MFile,node->MFunc,node->MLine,(node->MNew?"new":"malloc"),node->MAddr,node->MSize);
          // dtrace("* %i\n",node->MLine);
          s3_debug_memnode* next = (s3_debug_memnode*)node->MNext;
          //if (ADelete) { free(node); }
          node = next;
        }
      }
      S3_DTrace("----------------------------------------\n");
    }

    //----------

    void* trace_malloc(const size_t size, const char* file, const char* func, unsigned int line) {
      void* ptr = s3_memtrace_malloc(size);
      append(ptr,size,file,func,line,false);
      return ptr;
    }

    //----------

    void* trace_calloc(const int num, const size_t size, const char* file, const char* func, unsigned int line) {
      void* ptr = s3_memtrace_calloc(num,size);
      append(ptr,num*size,file,func,line,false);
      return ptr;
    }

    //----------

    void* trace_realloc(void* ptr, const size_t size, const char* file, const char* func, unsigned int line) {
      void* p2 = s3_memtrace_realloc(ptr,size);
      remove(ptr,file,func,line,false);
      append(p2,size,file,func,line,false);
      return p2;
    }

    //----------

    void trace_free(void* ptr, const char* file, const char* func, unsigned int line) {
      remove(ptr,file,func,line,false);
      s3_memtrace_free(ptr);
    }

};

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

/*
  being static means that multiple instances uses the same
  static_MemTrace object.. it's shared for all instances/threads..

  needs to be static, so our 'overloaded' new/delete macros can use it.
  would be better if it were part of the S3_Debug class, but i haven't found
  a nice/clean way to replace the new/delete with macros that call a
  non-static class method. if this were part of a class, this class must be
  available/accessible during our new (pun..) new/delete macros. everywhere.

  __thread ??

  multiple instances can be in a single thread, or in multiple threads.
  our problem above is not related to threads, but that many plugin instances
  have access to the same static/global class-instance.
*/

static S3_Debug_MemTrace s3_debug_memtrace;

//----------------------------------------------------------------------

// not thread-safe?
// unreliable (and dangerous?) if multiple plugin instances uses
// mem-tracing at the same time.. ??

// these should be defined as __thread
static __thread char*         s3_mem_del_file;
static __thread char*         s3_mem_del_func;
static __thread unsigned int  s3_mem_del_line;

//----------

bool S3_Debug_MemDelPrepare(const char* file, const char* func, const unsigned int line) {
  //DTrace("leaks_del_prepare:" /*<< SStripPath(file) << "," << func << "," << line*/);
  s3_mem_del_file = (char*)file;
  s3_mem_del_func = (char*)func;
  s3_mem_del_line = line;
  return true;
}

//----------

void* operator new (const size_t size, const char* file, const char* func, unsigned int line) throw (std::bad_alloc) {
  //DTrace("op new");
  void* ptr = s3_memtrace_malloc(size);
  s3_debug_memtrace.append(ptr,size,file,func,line,true);
  return ptr;
}

//----------

void* operator new[] (const size_t size, const char* file, const char* func, unsigned int line) throw (std::bad_alloc) {
  //DTrace("op new[]");
  void* ptr = s3_memtrace_malloc(size);
  s3_debug_memtrace.append(ptr,size,file,func,line,true);
  return ptr;
}

//----------

void operator delete (void* ptr) throw() {
  //DTrace("op delete");
  s3_memtrace_free(ptr);
  s3_debug_memtrace.remove(ptr,s3_mem_del_file,s3_mem_del_func,s3_mem_del_line,true);
}

//----------

void operator delete[] (void* ptr) throw() {
  //DTrace("op delete[]");
  s3_memtrace_free(ptr);
  s3_debug_memtrace.remove(ptr,s3_mem_del_file,s3_mem_del_func,s3_mem_del_line,true);
}

//----------------------------------------------------------------------

// catch malloc

#define malloc(size)      s3_debug_memtrace.trace_malloc(size,     __FILE__,__FUNCTION__,__LINE__)
#define calloc(num,size)  s3_debug_memtrace.trace_calloc(num,size, __FILE__,__FUNCTION__,__LINE__)
#define realloc(ptr,size) s3_debug_memtrace.trace_realloc(ptr,size,__FILE__,__FUNCTION__,__LINE__)
#define free(ptr)         s3_debug_memtrace.trace_free(ptr,        __FILE__,__FUNCTION__,__LINE__)

// catch op. new / delete

/*
  if i include
  error: declaration of 'operator new' as non-function
*/


#define s3_debug_new    new(__FILE__, __FUNCTION__, __LINE__)
#define s3_debug_delete if (S3_Debug_MemDelPrepare(__FILE__, __FUNCTION__, __LINE__)) delete

#define new               s3_debug_new
#define delete            s3_debug_delete

// this gts rid of the error, but we would have to replace all new/delete
// with our versions to get memory tracing... :-/

//#define NEW               debug_new
//#define DELETE            debug_delete

// http://wyw.dcweb.cn/leakage.htm
//void* operator new(size_t size, const char* file, int line);
//void* operator new[](size_t size, const char* file, int line);
//#define DEBUG_NEW new(__FILE__, __LINE__)
//#define new DEBUG_NEW

//----------------------------------------------------------------------

#undef s3_memtrace_malloc
#undef s3_memtrace_calloc
#undef s3_memtrace_realloc
#undef s3_memtrace_free

#endif // S3_GDIPLUS
#endif // S3_DEBUG_MEM

//----------------------------------------------------------------------
#endif
