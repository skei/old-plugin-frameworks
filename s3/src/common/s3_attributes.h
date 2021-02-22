#ifndef s3_attributes_included
#define s3_attributes_included
//----------------------------------------------------------------------

//----------------------------------------
// win32
// dllexport, dllimport, cdecl, stdcall
//----------------------------------------

#ifdef S3_WIN32

  /*
    On Microsoft Windows targets and Symbian OS targets the dllexport attribute
    causes the compiler to provide a global pointer to a pointer in a DLL, so
    that it can be referenced with the dllimport attribute
  */

  #define _S3_DLLEXPORT __attribute__ ((dllexport))

  /*
    On Microsoft Windows and Symbian OS targets, the dllimport attribute causes
    the compiler to reference a function or variable via a global pointer to a
    pointer that is set up by the DLL exporting the symbol.
  */

  #define _S3_DLLIMPORT __attribute__ ((dllimport))

  /*
    On the Intel 386, the cdecl attribute causes the compiler to assume that
    the calling function pops off the stack space used to pass arguments.
  */

  #define _S3_CDECL __attribute__ ((cdecl))

  /*
    On the Intel 386, the stdcall attribute causes the compiler to assume that
    the called function pops off the stack space used to pass arguments, unless
    it takes a variable number of arguments.
  */

  #define _S3_STDCALL __attribute__ ((stdcall))

#endif

//----------

#ifdef S3_LINUX
  #define _S3_DLLEXPORT
  #define _S3_DLLIMPORT
  #define _S3_CDECL
  #define _S3_STDCALL
#endif

//----------------------------------------
// linux
// dll public/local (-fvisibility=hidden)
//----------------------------------------

/*

  https://gcc.gnu.org/wiki/Visibility

  In your header files, wherever you want an interface or API made public
  outside the current DSO, place __attribute__ ((visibility ("default"))) in
  struct, class and function declarations you wish to make public (it's easier
  if you define a macro as this). You don't need to specify it in the
  definition. Then, alter your make system to pass -fvisibility=hidden to each
  call of GCC compiling a source file.

    #define DLL_PUBLIC __attribute__ ((visibility ("default")))
    #define DLL_LOCAL  __attribute__ ((visibility ("hidden")))

    extern "C" DLL_PUBLIC void function(int a);

    class DLL_PUBLIC SomeClass {
      int c;
      DLL_LOCAL void privateMethod();  // Only for use within this DSO
    public:
      Person(int _c) : c(_c) { }
      static void foo(int a);
    };

*/

#ifdef S3_LINUX
  #define _S3_DLLPUBLIC __attribute__ ((visibility ("default")))
  #define _S3_DLLLOCAL __attribute__ ((visibility ("hidden")))
  #undef _S3_DLLEXPORT
  #define _S3_DLLEXPORT _S3_DLLPUBLIC
#endif

#ifdef S3_WIN32
  #define _S3_DLLPUBLIC
  #define _S3_DLLLOCAL
#endif

//----------------------------------------
// attributes
//----------------------------------------

// https://gcc.gnu.org/onlinedocs/gcc/Function-Attributes.html#Function-Attributes
// https://gcc.gnu.org/onlinedocs/gcc/Variable-Attributes.html#Variable-Attributes
// https://gcc.gnu.org/onlinedocs/gcc/Type-Attributes.html#Type-Attributes

//#define __aligned_t(x)                    __attribute__ ((aligned (x)))
//#define __may_alias_t                     __attribute__ ((may_alias))

//----------------------------------------

/*
  The alias attribute causes the declaration to be emitted as an alias for
  another symbol, which must be specified.
*/

#define _S3_ALIAS(name) __attribute__ ((alias (name)))

/*
  This attribute specifies a minimum alignment for the function, measured in
  bytes.
*/

#define _S3_ALIGNED(x) __attribute__ ((aligned (x)))

/*
  Ask the compiler to align a variable or field to the maximum useful alignment
  for the target machine you are compiling for.
*/

#define _S3_ALIGNEDMAX __attribute__ ((aligned))

/*
  The alloc_size attribute is used to tell the compiler that the function
  return value points to memory, where the size is given by one or two of the
  functions parameters. GCC uses this information to improve the correctness of
  __builtin_object_size.
*/

#define _S3_ALLOC_SIZE1(x) __attribute__ ((alloc_size(x)))

/*
  see _S3_ALLOC_SIZE1
*/

#define _S3_ALLOC_SIZE2(x, y) __attribute__ ((alloc_size(x,y)))

/*
  Generally, functions are not inlined unless optimization is specified. For
  functions declared inline, this attribute inlines the function even if no
  optimization level is specified.
*/

#define _S3_ALWAYS_INLINE inline __attribute__ ((always_inline))

/*
*/

#define _S3_ASM_VOLATILE __asm__ __volatile__

/*
  In GCC and Clang, there is the "cleanup" attribute which runs a user-supplied
  function once a variable goes out of scope, so you can have scope-based
  destructors:
    static
    void do_cleanup(char **str) {
      free(*str);
    }
    void f(void) {
      char *s __attribute__((__cleanup__(do_cleanup))) = malloc(20);
      // s gets freed once the function exits
    }
*/

#define _S3_CLEANUP(x) __attribute__ ((__cleanup__(x)))

/*
  The constructor attribute causes the function to be called automatically
  before execution enters main()
*/

#define _S3_CONSTRUCTOR __attribute__ ((constructor))

/*
  The deprecated attribute results in a warning if the function is used
  anywhere in the source file.
*/

#define _S3_DEPRECATED __attribute__ ((deprecated))

/*
  The destructor attribute causes the function to be called automatically after
  main() completes or exit () is called.
*/

#define _S3_DESTRUCTOR __attribute__ ((destructor))

/*
*/

#define _S3_EXTERNC extern "C"

/*
*/

#define _S3_HOTINLINE ___always_inline __hot __optimize(3)

/*
*/

#define _S3_INIT_PRIORITY(x) __attribute__ ((init_priority(x)))

/*
  The malloc attribute is used to tell the compiler that a function may be
  treated as if any non-NULL pointer it returns cannot alias any other pointer
  valid when the function returns and that the memory has undefined content.
  This often improves optimization.
*/

#define _S3_MALLOC __attribute__ ((malloc))

/*
*/

#define _S3_MAY_ALIAS __attribute__ ((may_alias))

/*
  This function attribute prevents a function from being considered for
  inlining.
*/

#define _S3_NOINLINE __attribute__ ((noinline))

/*
  The noreturn keyword tells the compiler to assume that thye function cannot
  return. It can then optimize without regard to what would happen if the
  function ever did return. This makes slightly better code. More importantly,
  it helps avoid spurious warnings of uninitialized variables.
*/

#define _S3_NORETURN __attribute__ ((noreturn))

/*
*/

#define _S3_PACKED __attribute__ ((packed))

/*
  Many functions have no effects except the return value and their return value
  depends only on the parameters and/or global variables. Such a function can
  be subject to common subexpression elimination and loop optimization just as
  an arithmetic operator would be. These functions should be declared with the
  attribute pure.
*/

#define _S3_PURE __attribute__ ((pure))

/*
*/

#define _S3_SHARED __attribute__ ((shared))

/*
*/

#define _S3_SECTION(name) __attribute__ ((section (name)))

/*
*/

#define _S3_SELECTANY __attribute__ ((selectany))

/*
  The target attribute is used to specify that a function is to be compiled
  with different target options than specified on the command line.
*/

#define _S3_TARGET(value) __attribute__ ((target(value)))

/*
*/

#define _S3_TRANSPARENT_UNION __attribute__ ((transparent_union))

/*
  This attribute, attached to a function, means that code must be emitted for
  the function even if it appears that the function is not referenced. This is
  useful, for example, when the function is referenced only in inline assembly.
*/

#define _S3_USED __attribute__ ((used))

/*
  This attribute, attached to a function, means that the function is meant to
  be possibly unused. GCC does not produce a warning for this function.
*/

#define _S3_UNUSED __attribute__ ((unused))

/*
*/

#define _S3_VECTOR(x) __attribute__ ((vector_size (x)))

/*
*/

#define _S3_VOLATILE __volatile__

/*
  The weak attribute causes the declaration to be emitted as a weak symbol
  rather than a global. This is primarily useful in defining library functions
  that can be overridden in user code, though it can also be used with
  non-function declarations.
*/

#define _S3_WEAK __attribute__ ((weak))

/*
  The weakref attribute marks a declaration as a weak reference. Without
  arguments, it should be accompanied by an alias attribute naming the target
  symbol. Optionally, the target may be given as an argument to weakref itself.
  In either case, weakref implicitly marks the declaration as weak. Without a
  target, given as an argument to weakref or to alias, weakref is equivalent to
  weak.
*/

#define _S3_WEAKREF(name) __attribute__ ((weakref(name)))

//----------------------------------------
// gcc > 2
//----------------------------------------

// http://stackoverflow.com/questions/109710/likely-unlikely-macros-in-the-linux-kernel-how-do-they-work-whats-their

/*
  branch prediction hints to the compiler. This can help immensely for
  performance critical code. Note one should only use these hints where
  absolutely necessary because they make the code more complicated to
  read/maintain.
*/

//#if (__GNUC__ > 2) && defined(__OPTIMIZE__)
#if (__GNUC__ > 2)

  #define _S3_LIKELY(expr) __builtin_expect(!!(expr),1)
  #define _S3_UNLIKELY(expr) __builtin_expect(!!(expr),0)

#else

  /*
  */

  #define _S3_LIKELY(expr) (expr)

  /*
  */

  #define _S3_UNLIKELY(expr) (expr)

#endif

//----------------------------------------
// gcc >= 4.4
//----------------------------------------

#if (__GNUC__ >= 4) && (__GNUC_MINOR__ >= 4)


  /*
    The cold attribute on functions is used to inform the compiler that the
    function is unlikely to be executed. The function is optimized for size
    rather than speed and on many targets it is placed into special subsection
    of the text section so all cold functions appears close together improving
    code locality of non-cold parts of program. The paths leading to call of
    cold functions within code are marked as unlikely by the branch prediction
    mechanism. It is thus useful to mark functions used to handle unlikely
    conditions, such as perror, as cold to improve optimization of hot
    functions that do call marked functions in rare occasions.
  */

  #define _S3_COLD __attribute__ ((cold))

  /*
  */

  #define _S3_ERROR(message) __attribute__ ((error(message)))

  /*
    The hot attribute on a function is used to inform the compiler that the
    function is a hot spot of the compiled program. The function is optimized
    more aggressively and on many target it is placed into special subsection
    of the text section so all hot functions appears close together improving
    locality.
  */

  #define _S3_HOT __attribute__ ((hot))

  /*
    The optimize attribute is used to specify that a function is to be compiled
    with different optimization options than specified on the command line.
    Arguments can either be numbers or strings. Numbers are assumed to be an
    optimization level. Strings that begin with O are assumed to be an
    optimization option, while other options are assumed to be used with a -f
    prefix.
  */

  #define _S3_OPTIMIZE(level) __attribute__ ((optimize (level)))

  /*
  */

  #define _S3_WARNING(message) __attribute__ ((warning(message)))

#else

  #define _S3_COLD
  #define _S3_ERROR
  #define _S3_HOT
  #define _S3_OPTIMIZE(level)
  #define _S3_WARNING
  // disable TLS & show warning
  // #undef __thread
  #define _S3_THREAD
  //#warning "### Thread-local storage requires GCC 4.4.x"

#endif

//----------------------------------------------------------------------
#endif
