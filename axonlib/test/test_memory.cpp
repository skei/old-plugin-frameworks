//#define AX_NO_MALLOC
#define AX_DEBUG_AUTO_STD
#define AX_DEBUG_MEM
#define AX_DEBUG_NEW
#define AX_DEBUG_PNG
#define AX_DEBUG_LOG "test_memory.log"

#include "axConfig.h"
#include "core/axDefines.h"
#include "core/axMalloc.h"
#include "core/axDebug.h"
#include "core/axUtils.h"
#include "core/axAssert.h"
#include "core/axRand.h"

#include "gui/axBitmapLoader.h"
#include "../ax/skins/img/skin1.h"
#include "../ax/skins/img/knob1.h"

//----------------------------------------------------------------------

int main(void)
{
  axBitmapLoader* loader1;
  axBitmapLoader* loader2;
  int i;

  axRand(666);

  //----------

  //trace("before loading png 1:");
  //trace("png_alloc_head: " << png_alloc_head);
  //trace("png_alloc_tail: " << png_alloc_tail);

  loader1 = new axBitmapLoader();
  loader1->decodePng((unsigned char*)skin1,skin1_size);

  //trace("after loading png 1:");
  //trace("png_alloc_head: " << png_alloc_head);
  //trace("png_alloc_tail: " << png_alloc_tail);

  //----------

  int num_malloc = 0;
  void* bufm[1000];

  for (i=0; i<100; i++)
  {
    int n = axRandInt(999)+1;
    bufm[i] = axMalloc(n);
    num_malloc += n;
  }
  trace("num_malloc: " << num_malloc);

  //----------

  //trace("before loading png 2:");
  //trace("png_alloc_head: " << png_alloc_head);
  //trace("png_alloc_tail: " << png_alloc_tail);

  loader2 = new axBitmapLoader();
  loader2->decodePng((unsigned char*)knob1,knob1_size);

  //trace("after loading png 2:");
  //trace("png_alloc_head: " << png_alloc_head);
  //trace("png_alloc_tail: " << png_alloc_tail);

  //----------

  int num_new = 0;
  char* bufn[1000];

  for (i=0; i<100; i++)
  {
    int n = axRandInt(999)+1;
    bufn[i] = new char[n];
    num_new += n;
  }
  trace("num_new: " << num_new);

  //----------

  for (i=0; i<100; i++) axFree( bufm[i] );
  for (i=0; i<100; i++) delete bufn[i];
  delete loader1;
  delete loader2;

}
