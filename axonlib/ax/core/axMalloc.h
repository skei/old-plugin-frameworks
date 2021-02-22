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

/*
  axMalloc.h
  
  NOTE: currenly axMalloc = stdlib malloc 
  TODO: write something faster & that works better than the previous
  one.  
*/

#ifndef axMalloc_included
#define axMalloc_included

#ifdef AX_HOT_INLINE_MALLOC
  #define __axmalloc_inline __hotinline
#else
  #define __axmalloc_inline inline
#endif

#ifndef AX_USE_MALLOC
  #define AX_NO_MALLOC  // disable the local malloc by default
#endif

#ifdef AX_NO_MALLOC     // no axMalloc - use stdlib.h malloc
  #include "stdlib.h"
  #define axMalloc    malloc
  #define axCalloc    calloc
  #define axRealloc   realloc
  #define axFree      free

  // no debug for these
  #define _axMalloc   malloc
  #define _axCalloc   calloc
  #define _axRealloc  realloc
  #define _axFree     free

#endif // use axMalloc

// include debug here
#include "axMallocDebug.h"

#endif // axMalloc_included
