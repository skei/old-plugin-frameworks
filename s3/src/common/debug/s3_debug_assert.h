#ifndef s3_debug_assert_included
#define s3_debug_assert_included
//----------------------------------------------------------------------

// parts of the code by lubomir i ivanov (for axonlib)
// used with permission

//----------------------------------------------------------------------

#ifdef S3_DEBUG_ASSERT

  #include <iostream> // cout
  //#include "s3_debug_trace.h"

  //----------

  // for c89 compat mainly if the header is used elsewhere
  #ifdef __GNUC_STDC_INLINE__
    #define S3_STATIC_ASSERT_INLINE inline
  #else
    #define S3_STATIC_ASSERT_INLINE
  #endif

  /*
    SAssert()
    runtime assertion
  */

  unsigned int _S3_Assert () __attribute__ (( noinline ));

  unsigned int _S3_Assert (const unsigned int e, const char* file, const unsigned int line, const char* e_str) {
    if (!e) {
      //DTrace("S3_ASSERT: " << file << ", " << line << ", (" << e_str << ")");
      S3_DTrace("S3_ASSERT: %s, %i, %s\n",file,line,e_str);
      __builtin_exit(0);
    }
    return 1;
  }

  #define S3_Assert(e) _S3_Assert((e), __FILE__, __LINE__, #e)

  //----------------------------------------

  /*
    S3_StaticAssert()
    compile time assertion ( requires GCC 4.4.x )

    TODO:
    - file/line/str arguments, like S3_Assert

  */

  #if (__GNUC__ >= 4) && (__GNUC_MINOR__ >= 4)
    S3_STATIC_ASSERT_INLINE unsigned int S3_STATIC_ASSERT_TRUE (void) { return 1; }
    unsigned int S3_STATIC_ASSERT (void)
    __attribute__ (( error("### S3_STATIC_ASSERT at this point!") ))
    __attribute__ (( optimize(0) ));
    #define S3_StaticAssert(e) ( (!(e)) ? S3_STATIC_ASSERT() : S3_STATIC_ASSERT_TRUE() )
  #else
    /*
      switch can also be used. it does not generate extra code and is much
      more portable, but not very useful at the same time.
    */
    #define S3_StaticAssert(e) switch(0) { case 0: case (e): ; }
  #endif

#else // S3_DEBUG_ASSERT

  //S3_STATIC_ASSERT_INLINE unsigned int _S3_STATIC_ASSERT (void) { return 0; }
  //#define S3_Assert(e)       _S3_STATIC_ASSERT()
  //#define S3_StaticAssert(e) _S3_STATIC_ASSERT()

  #define S3_Assert(e)       {}
  #define S3_StaticAssert(e) {}


#endif // S3_DEBUG_ASSERT

//----------------------------------------------------------------------
#endif

/*
  http://www.trenki.net/content/view/18/1/

  template<bool> struct CompileTimeError;
  template<> struct CompileTimeError<true> {};

  #define STATIC_ASSERT(expr) \
    (CompileTimeError<(expr) != 0>())

*/
