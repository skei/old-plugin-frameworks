/*
 * This file is part of Axonlib.
 *
 * Axonlib is free software: you can redistribute it and/or modify
 * it under the terms of the Axonlib License, either version 1.0
 * of the License, or (at your option) any later version.
 *
 * Axonlib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE_AX for more details.
 *
 * You should have received a copy of the Axonlib License
 * If not, see <http://axonlib.googlecode.com/>.
 */

#ifndef axMallocDebug_included
#define axMallocDebug_included

// NOTE: should be included at the end of axMalloc.h ! 

// -----------------------------------------------------------------------------
// enable local debug
// -----------------------------------------------------------------------------
#if defined (AX_DEBUG) && defined (AX_DEBUG_MEM)
  #include "axDebug.h"

  #ifdef AX_NO_MALLOC
    #include "malloc.h"
  #endif

  #ifdef AX_WIN32
    #define malloc_usable_size _msize
  #endif

  // global data, can be 'dangerous' for multiple instances
  static __thread int _axMemTotal = 0;

  // malloc debug
  __unused static void* axMallocDebug
    (register unsigned int _size, const char* _file, const unsigned int _line,
    const unsigned int flag = 0)
  {
    char _name[20];
    #ifdef AX_NO_MALLOC
      void* _ptr = malloc(_size);
      _axMemTotal += malloc_usable_size(_ptr);
      if (flag)
        axStrcpy(_name, "malloc(new), ");
      else
        axStrcpy(_name, "malloc, ");
    #else
      void* _ptr = axMalloc(_size);
      _axMemTotal += bucket2size[*(unsigned int*)((char*)_ptr-4)];
      if (flag)
        axStrcpy(_name, "axMalloc(new), ");
      else
        axStrcpy(_name, "axMalloc, ");
    #endif
    // output cout / log
    _trace
    (
      "[" << axGetFileName(_file) << ":" << _line << "] " << _name <<
      (void*)&_ptr << ", " << _size << ", " << _axMemTotal
    );
    return _ptr;
  }

  // calloc debug
  __unused static void* axCallocDebug
    (register const unsigned int _n, register unsigned int _size,
    const char* _file, const unsigned int _line)
  {
    char _name[20];
    #ifdef AX_NO_MALLOC
      void* _ptr = calloc(_n, _size);
      unsigned int size = malloc_usable_size(_ptr);
      _axMemTotal += size;
      axStrcpy(_name, "calloc, ");
    #else
      void* _ptr = axCalloc(_n, _size);
      unsigned int size = bucket2size[*(unsigned int*)((char*)_ptr-4)];
      _axMemTotal += size;
      axStrcpy(_name, "axCalloc, ");
    #endif
    // output cout / log
    _trace
    (
      "[" << axGetFileName(_file) << ":" << _line << "] " << _name <<
      (void*)&_ptr << ", " << _n << ", " << _size << ", " << size << ", " <<
      _axMemTotal
    );
    return _ptr;
  }

  // realloc debug
  __unused static void* axReallocDebug
    (void* _ptr, register const unsigned int _size, const char* _file,
    const unsigned int _line)
  {
    char _name[20];
    #ifdef AX_NO_MALLOC
      int nsize = malloc_usable_size(_ptr);
      if (_axMemTotal - nsize >= 0)
        _axMemTotal -= nsize;
      else
        _axMemTotal = 0;
      void* _ptr0 = realloc(_ptr, _size);
      _axMemTotal += malloc_usable_size(_ptr0);
      axStrcpy(_name, "realloc, ");
    #else
      int nsize = bucket2size[*(unsigned int*)((char*)_ptr-4)];
      if (_axMemTotal - nsize >= 0)
        _axMemTotal -= nsize;
      void* _ptr0 = axRealloc(_ptr, _size);
      _axMemTotal += bucket2size[*(unsigned int*)((char*)_ptr0-4)];;
      axStrcpy(_name, "axRealloc, ");
    #endif
    // output cout / log
    _trace
    (
      "[" << axGetFileName(_file) << ":" << _line << "] " << _name <<
      (void*)&_ptr << ", " << (void*)&_ptr0 <<
      ", " << _size << ", " << _axMemTotal
    );

    return _ptr0;
  }

  // free debug
  __unused static void axFreeDebug
    (void* _ptr, const char* _file, const unsigned int _line,
    const unsigned int flag = 0)
  {
    if (_axMemTotal && _ptr)
    {
      int _size = 0;
      char _name[20];
      #ifdef AX_NO_MALLOC
        _size = malloc_usable_size(_ptr);
        if (flag)
          axStrcpy(_name, "free(delete), ");
        else
          axStrcpy(_name, "free, ");
      #else
        unsigned int b = *(unsigned int*)((char*)_ptr-4);
        if (b < AX_M_MAX_BUCKETS)
          _size = bucket2size[b];
        if (flag)
          axStrcpy(_name, "axFree(delete), ");
        else
          axStrcpy(_name, "axFree, ");
      #endif
      // output cout / log
      if (_axMemTotal - _size >= 0)
      {
        _axMemTotal -= _size;
        _trace
        (
          "[" << axGetFileName(_file) << ":" << _line << "] " << _name <<
          (void*)&_ptr << ", " << _size << ", " << _axMemTotal
        );

        #ifdef AX_NO_MALLOC
          free(_ptr);
        #else
          axFree(_ptr);
        #endif
      }
      else
        _axMemTotal = 0;
    }
  }

  // no debug for these: _ax*
  #undef _axMalloc
  #undef _axCalloc
  #undef _axRealloc
  #undef _axFree
  __axmalloc_inline void* _axMalloc (register const unsigned int size)
    { return axMalloc(size); }
  __axmalloc_inline void* _axCalloc (register const unsigned int n,
    register unsigned int size)  { return axCalloc(n, size); }
  __axmalloc_inline void* _axRealloc (register void* _ptr,
    register const unsigned int size) { return axRealloc(_ptr, size); }
  __axmalloc_inline void  _axFree (register void* _ptr)
    { axFree(_ptr); }

  // clear previous definitions (if any)
  #ifdef AX_NO_MALLOC
    #undef axMalloc
    #undef axCalloc
    #undef axRealloc
    #undef axFree
  #endif

  // macro overrides here
  #define axMalloc(s)     axMallocDebug   (s, __FILE__, __LINE__)
  #define axCalloc(n, s)  axCallocDebug   (n, s, __FILE__, __LINE__)
  #define axRealloc(p, s) axReallocDebug  (p, s, __FILE__, __LINE__)
  #define axFree(p)       axFreeDebug     (p, __FILE__, __LINE__)

  // same for the standard methods if 'AX_NO_MALLOC'
  //#ifdef AX_NO_MALLOC
    #define malloc(s)       axMallocDebug   (s, __FILE__, __LINE__)
    #define calloc(n, s)    axCallocDebug   (n, s, __FILE__, __LINE__)
    #define realloc(p, s)   axReallocDebug  (p, s, __FILE__, __LINE__)
    #define free(p)         axFreeDebug     (p, __FILE__, __LINE__)
  //#endif

  #ifdef AX_DEBUG_NEW
    // define some helpers for the delete operator
    static __thread char* ax_del_file;
    static __thread unsigned int ax_del_line;
    unsigned int axDebugSetDelete(const char* file, const unsigned int line)
    {
      ax_del_file = (char*)file;
      ax_del_line = line;
      return 1;
    }

    // overload operators new & delete with debug
    __axmalloc_inline void* operator new (const size_t size,
      const char* file, unsigned int line) throw (std::bad_alloc)
    {
      return axMallocDebug(size, file, line, 1);
    }
    __axmalloc_inline void* operator new[] (const size_t size,
      const char* file, unsigned int line) throw (std::bad_alloc)
    {
      return axMallocDebug(size, file, line, 1);
    }
    __axmalloc_inline void operator delete (void* ptr) throw()
    {
      return axFreeDebug(ptr, ax_del_file, ax_del_line, 1);
    }
    __axmalloc_inline void operator delete[] (void* ptr) throw()
    {
     return axFreeDebug(ptr, ax_del_file, ax_del_line, 1);
    }

    #define new new(__FILE__, __LINE__)
    #define delete if(axDebugSetDelete(__FILE__, __LINE__)) delete

  #endif // AX_DEBUG_NEW

#else // AX_DEBUG && AX_DEBUG_MEM

  // overload operators new & delete without debug
  #include <new>

  __axmalloc_inline void* operator new (size_t size)
    throw (std::bad_alloc)
  {
    return axMalloc(size);
  }
  __axmalloc_inline void* operator new[] (size_t size)
    throw (std::bad_alloc)
  {
    return axMalloc(size);
  }
  __axmalloc_inline void operator delete (void* ptr) throw()
  {
    return axFree(ptr);
  }
  __axmalloc_inline void operator delete[] (void* ptr) throw()
  {
    return axFree(ptr);
  }

#endif // AX_DEBUG && AX_DEBUG_MEM

#endif // axMallocDebug_included
