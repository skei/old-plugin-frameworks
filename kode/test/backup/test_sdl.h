
#define KODE_SDL
#define KODE_NO_GUI

#include "base/kode.h"
#include "gui/sdl/kode_sdl_window.h"

void render(KODE_Bitmap* ABitmap, float ATime) {
  KODE_Trace("ABitmap %p ATime %.3f\n",ABitmap,ATime);
}

int main(void) {
  KODE_SdlWindow* window = KODE_New KODE_SdlWindow(320,200);
  window->setTitle("KODE_SdlWindow");
  window->setRenderCallback(render);
  window->open();
  window->eventLoop();
  window->close();
  KODE_Delete window;
}