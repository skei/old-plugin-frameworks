#ifndef kode_debug_memory_included
#define kode_debug_memory_included
//----------------------------------------------------------------------

//#define KODE_DEBUG_MEMORY
//#define KODE_DEBUG_MEMORY_PRINT

//----------

#ifdef KODE_DEBUG_MEMORY

#include "base/kode.h"
#include "base/kode_list.h"

#include "debug/kode_debug_callstack.h"

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

class KODE_MemoryNode
: public KODE_ListNode{
  
public:
  
  void*             ptr                           = nullptr;
  uint32_t          size                          = 0;
  uint32_t          mode                          = 0; // 0=malloc, 1=new
  char              file[KODE_MAX_PATH_LENGTH]    = {0};
  char              func[KODE_MAX_STRING_LENGTH]  = {0};
  uint32_t          line                          = 0;

public:

  KODE_MemoryNode(void* p, uint32_t s, uint32_t m, const char* fil, const char* fun, uint32_t lin) {
    //KODE_Trace("ptr: %p\n",p);
    ptr  = p;
    size = s;
    mode = m;
    KODE_Strcpy(file,fil);
    KODE_Strcpy(func,fun);
    line = lin;
  }

};

//----------

//typedef list<KODE_MemoryNode*> KODE_MemoryNodes;
//typedef KODE_List<KODE_MemoryNode*> KODE_MemoryNodes;

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

class KODE_DebugMemory {

//------------------------------
private:
//------------------------------

  KODE_List   MNodes;
  uint32_t    MNumAllocations = 0;
  uint32_t    MAllocated      = 0;
  uint32_t    MMaxAllocated   = 0;
  uint32_t    MLargestBlock   = 0;

//------------------------------
public:
//------------------------------

  KODE_DebugMemory() {
    //MNodes.clear();
    //MNodes.reset();
    //MNodes = KODE_NULL;
  }

  //----------

  ~KODE_DebugMemory() {
    dumpMemoryNodes();
    #ifndef KODE_NO_AUTODELETE
    deleteMemoryNodes();
    #endif
    //MNodes.clear();
    //MNodes.reset();
  }

//------------------------------
private:
//------------------------------

  // /home/skei/test -> test

  const char* strip_path(const char* buffer) {
    const char *slash;
    //const char *backslash;
    slash = KODE_Strrchr(buffer,'/');
    //backslash = KODE_Strrchr(buffer,'\\');//+1;
    if (slash) {
      return slash + 1;
    }
    //else if (backslash) {
    //  _copy_buffer(backslash + 1); // see above
    //}
    return buffer;
  }
  
  //----------

  void appendMemoryNode(void* ptr, uint32_t size, uint32_t mode, const char* file, const char* func, uint32_t line) {
    //KODE_Trace("ptr: %p\n",ptr);
    KODE_MemoryNode* node = ::new KODE_MemoryNode(ptr,size,mode,file,func,line);
    //MNodes.push_back(node);
    MNodes.insertTail(node);
    MNumAllocations += 1;
    MAllocated += size;
    if (MAllocated > MMaxAllocated) MMaxAllocated = MAllocated;
    if (size > MLargestBlock) MLargestBlock = size;
  }

  //----------

  KODE_MemoryNode* findMemoryNode(void* ptr) {
    if (ptr == nullptr) { KODE_Trace("nullptr\n"); }
    //for (KODE_MemoryNodes::iterator it = MNodes.begin(); it != MNodes.end(); it++) {
    KODE_MemoryNode* node = (KODE_MemoryNode*)MNodes.head();
    while (node) {
      //KODE_MemoryNode* node = *it;
      if (node->ptr == ptr) {
        //KODE_Trace("node (%p) found -> %p\n",ptr,node->ptr);
        return node;
      }
      node = (KODE_MemoryNode*)node->next();
    }
    //KODE_Trace("node (%p) not found\n",ptr);    
    return KODE_NULL;
  }

  //----------

  void removeMemoryNode(void* ptr) {
    //KODE_Trace("ptr: %p\n",ptr);
    KODE_MemoryNode* node = findMemoryNode(ptr);
    if (node) {
      MNumAllocations -= 1;
      MAllocated -= node->size;
      MNodes.removeNode(node);
      ::delete node;
    }
    //KODE_TRACE;
    //for (KODE_MemoryNodes::iterator it = MNodes.begin(); it != MNodes.end(); it++) {
    //  KODE_MemoryNode* node = *it;
    //  if (node->ptr == ptr) {
    //    MNumAllocations -= 1;
    //    MAllocated -= node->size;
    //    delete node;
    //    MNodes.erase(it);
    //    return;
    //  }
    //}
  }

  //----------

  void deleteMemoryNodes() {
    //KODE_TRACE;
    KODE_MemoryNode* node = (KODE_MemoryNode*)MNodes.head();
    while (node) {
      KODE_MemoryNode* next = (KODE_MemoryNode*)node->next();
      ::delete node;
      node = next;
    }
    //KODE_TRACE;
  }

  //----------

  void dumpMemoryNodes() {
    KODE_DTrace("----------------------------------------------------------------------\n");
    KODE_DTrace("memory tracer:\n");
    KODE_DTrace("----------------------------------------------------------------------\n");
    KODE_DTrace("num allocations:         %i\n",MNumAllocations);
    KODE_DTrace("allocated:               %i\n",MAllocated);
    KODE_DTrace("max allocated:           %i\n",MMaxAllocated);
    KODE_DTrace("largest allocated block: %i\n",MLargestBlock);
    KODE_DTrace("----------------------------------------------------------------------\n");
    if (MNumAllocations > 0) {
      uint32_t i = 0;
      KODE_MemoryNode* node = (KODE_MemoryNode*)MNodes.head();
      while (node) {
        KODE_DTrace("%i : [%s:%s:%i] %s(%i) = %p\n",i,node->func,strip_path(node->file),node->line,node->mode == 1 ? "new" : "malloc",node->size,node->ptr);
        node = (KODE_MemoryNode*)node->next();
        i += 1;
      }
      KODE_DTrace("----------------------------------------------------------------------\n");
    }
  }

//------------------------------
public:
//------------------------------

  void* _malloc(uint32_t size, const char* file, uint32_t line, const char* func) {
    void* ptr = ::malloc(size);
    #ifdef KODE_DEBUG_MEMORY_PRINT
    KODE_DTrace("[%s:%s:%i] _malloc(%i) = %p\n",func,strip_path(file),line,size,ptr);
    #endif
    appendMemoryNode(ptr,size,0,file,func,line);
    return ptr;
  }

  //----------

  //void* _calloc(uint32_t num, uint32_t size, const char* file, uint32_t line, const char* func) {
  //  return nullptr;
  //}

  //----------

  void* _realloc(void* ptr, uint32_t size, const char* file, uint32_t line, const char* func) {
    void* newptr = ::realloc(ptr,size);
    #ifdef KODE_DEBUG_MEMORY_PRINT
    KODE_DTrace("[%s:%s:%i] _realloc(%p,%i) = %p\n",func,strip_path(file),line,ptr,size,newptr);
    #endif
    removeMemoryNode(ptr);
    appendMemoryNode(newptr,size,0,file,func,line);
    return newptr;
  }

  //----------

  void _free(void* ptr, const char* file, uint32_t line, const char* func) {
    #ifdef KODE_DEBUG_MEMORY_PRINT
    KODE_DTrace("[%s:%s:%i] _free(%p)\n",func,strip_path(file),line,ptr);
    #endif
    removeMemoryNode(ptr);
    ::free(ptr);
  }

  //----------

  void* _new(uint32_t size, const char* file, uint32_t line, const char* func) {
    void* ptr = ::malloc(size);
    #ifdef KODE_DEBUG_MEMORY_PRINT
    KODE_DTrace("[%s:%s:%i] _new(%i) = %p\n",func,strip_path(file),line,size,ptr);
    #endif
    appendMemoryNode(ptr,size,1,file,func,line);
    return ptr;
  }

  //----------

  void _delete(void* ptr, const char* file, uint32_t line, const char* func) {
    #ifdef KODE_DEBUG_MEMORY_PRINT
    if (line==0)
      KODE_DTrace("[?] _delete(%p)\n",ptr);
    else
      KODE_DTrace("[%s:%s:%i] _delete(%p)\n",func,strip_path(file),line,ptr);
    //KODE_DumpCallStack;
    #endif
    removeMemoryNode(ptr);
    ::free(ptr);
  }

  //----------

};

//----------------------------------------------------------------------

KODE_DebugMemory  KODE_GLOBAL_DEBUG_MEMORY;

//----------------------------------------------------------------------

  // this is a bit of a mess..
  // isn't there a better way to pass the file/line(func to _delete?
  // static? __thread? volatile?

//  /*static __thread*/ const char*   kode_delete_file;
//  /*static __thread*/ unsigned int  kode_delete_line;
//  /*static __thread*/ const char*   kode_delete_func;

  char      kode_delete_file[KODE_MAX_PATH_LENGTH] = {0};
  char      kode_delete_func[KODE_MAX_STRING_LENGTH] = {0};
  uint32_t  kode_delete_line = 0;
//
  bool KODE_SetupDeletePrefix(const char* file, unsigned int line, const char* func) {
    //kode_delete_file = (char*)file;
    //kode_delete_line = line;
    //kode_delete_func = (char*)func;
    KODE_Strcpy(kode_delete_file,file);
    KODE_Strcpy(kode_delete_func,func);
    kode_delete_line = line;
    return true;
  }

  //----------

  void* operator new (const size_t size, const char* file, unsigned int line, const char* func) /*throw(std::bad_alloc)*/ {
    return KODE_GLOBAL_DEBUG_MEMORY._new(size,file,line,func);
  }

  void* operator new [] (const size_t size, const char* file, unsigned int line, const char* func) /*throw(std::bad_alloc)*/ {
    return KODE_GLOBAL_DEBUG_MEMORY._new(size,file,line,func);
  }

  void operator delete (void* ptr) _GLIBCXX_USE_NOEXCEPT {
  //void operator delete (void* ptr) /*throw()*/ {
    KODE_GLOBAL_DEBUG_MEMORY._delete(ptr,kode_delete_file,kode_delete_line,kode_delete_func);
    kode_delete_file[0] = 0;
    kode_delete_func[0] = 0;
    kode_delete_line = 0;
  }

  //void operator delete[] (void* ptr) /*throw()*/ {
  void operator delete [] (void* ptr) _GLIBCXX_USE_NOEXCEPT {
    KODE_GLOBAL_DEBUG_MEMORY._delete(ptr,kode_delete_file,kode_delete_line,kode_delete_func);
    kode_delete_file[0] = 0;
    kode_delete_func[0] = 0;
    kode_delete_line = 0;
  }

  // test

  //void operator delete(void*, std::size_t) _GLIBCXX_USE_NOEXCEPT {
  //  printf("delete\n");
  //}

  //void operator delete[](void*, std::size_t) _GLIBCXX_USE_NOEXCEPT {
  //  printf("delete[]\n");
  //}

  //void operator delete(void*, const std::nothrow_t&) _GLIBCXX_USE_NOEXCEPT {
  //  printf("delete 1\n");
  //}

  //void operator delete[](void*, const std::nothrow_t&) _GLIBCXX_USE_NOEXCEPT {
  //  printf("delete 1\n");
  //}

  //inline void operator delete  (void*, void*) _GLIBCXX_USE_NOEXCEPT {
  //  printf("delete 1\n");
  //}

  //inline void operator delete[](void*, void*) _GLIBCXX_USE_NOEXCEPT {
  //  printf("delete 1\n");
  //}


//----------------------------------------------------------------------

  #define KODE_Malloc(size)      KODE_GLOBAL_DEBUG_MEMORY._malloc(size,__FILE__,__LINE__,__FUNCTION__)
  //#define KODE_Calloc(num,size)  KODE_GLOBAL_DEBUG_MEMORY._calloc(num,size,__FILE__,__LINE__,__FUNCTION__)
  #define KODE_Realloc(ptr,size) KODE_GLOBAL_DEBUG_MEMORY._realloc(ptr,size,__FILE__,__LINE__,__FUNCTION__)
  #define KODE_Free(ptr)         KODE_GLOBAL_DEBUG_MEMORY._free(ptr,__FILE__,__LINE__,__FUNCTION__)
  #define KODE_New               new(__FILE__, __LINE__,__FUNCTION__)
  #define KODE_Delete            if (KODE_SetupDeletePrefix(__FILE__, __LINE__, __FUNCTION__)) delete

//----------------------------------------------------------------------
#else // KODE_DEBUG_MEMORY
//----------------------------------------------------------------------

  #define KODE_Malloc   malloc
  //#define KODE_Calloc   calloc
  #define KODE_Realloc  realloc
  #define KODE_Free     free
  #define KODE_New      new
  #define KODE_Delete   delete

#endif // KODE_DEBUG_MEMORY

//----------------------------------------------------------------------
#endif