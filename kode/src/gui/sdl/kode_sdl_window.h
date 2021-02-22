#ifndef kode_sdl_window_included
#define kode_sdl_window_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "base/kode_rect.h"
#include "gfx/kode_bitmap.h"
#include "gui/kode_drawable.h"
#include "gui/base/kode_base_window.h"
#include "gui/sdl/kode_sdl.h"

//----------------------------------------------------------------------

typedef void (*kode_render_frame_callback_t)(KODE_Bitmap* ABitmap, float ATime);

//----------------------------------------------------------------------

class KODE_SdlWindow
: public KODE_BaseWindow {

//------------------------------
private:
//------------------------------

  SDL_Window*   MWindow   = KODE_NULL;
  SDL_Renderer* MRenderer = KODE_NULL;
  SDL_Texture*  MTexture  = KODE_NULL;
  uint32_t      MWidth  = 0;
  uint32_t      MHeight = 0;
  KODE_Bitmap   MBitmap;
  kode_render_frame_callback_t MRenderCallback = KODE_NULL;
  
//------------------------------
public:
//------------------------------
  
  KODE_SdlWindow(uint32_t AWidth, uint32_t AHeight, uint32_t AScale=1)
  : KODE_BaseWindow(AWidth,AHeight) {
    MWidth = AWidth;
    MHeight = AHeight;
    SDL_Init(SDL_INIT_VIDEO);
    MWindow = SDL_CreateWindow(
      "intro",
      SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED,
      AWidth * AScale,
      AHeight * AScale,
      SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE
    );
    MRenderer = SDL_CreateRenderer(
      MWindow,
      -1,
      SDL_RENDERER_PRESENTVSYNC
    );
    MTexture = SDL_CreateTexture(
      MRenderer,
      SDL_PIXELFORMAT_ARGB8888,
      SDL_TEXTUREACCESS_STREAMING,
      AWidth,
      AHeight
    );
  }
  
  //----------
  
  virtual ~KODE_SdlWindow() {
    SDL_DestroyWindow(MWindow);
    SDL_DestroyRenderer(MRenderer);
    SDL_DestroyTexture(MTexture);
    SDL_Quit();
  }
  
//------------------------------
public: // drawable
//------------------------------

  bool          isSdl()           override { return true; }
  uint32_t      getWidth()        override { return MWidth; }
  uint32_t      getHeight()       override { return MHeight; }
  uint32_t      getDepth()        override { return 32; }
  SDL_Window*   getSdlWindow()    override { return MWindow; }
  SDL_Renderer* getSdlRenderer()  override { return MRenderer; }
  SDL_Texture*  getSdlTexture()   override { return MTexture; }

  //----------
  
  void setRenderCallback(kode_render_frame_callback_t ARenderCallback) {
    MRenderCallback = ARenderCallback;
  }
  
  //----------
  
  virtual void on_renderFrame(KODE_Bitmap* ABitmap, float AElapsed) {
    if (MRenderCallback) MRenderCallback(ABitmap,AElapsed); // cur_time
  }

//------------------------------
public:
//------------------------------

  void setPos(int32_t AXpos, int32_t AYpos) override {
    SDL_SetWindowPosition(MWindow,AXpos,AYpos);
  }
  
  //----------
  
  void setSize(int32_t AWidth, int32_t AHeight) override {
    SDL_SetWindowSize(MWindow,AWidth,AHeight);
  }
  
  //----------
  
  void setTitle(const char* ATitle) override {
    SDL_SetWindowTitle(MWindow,ATitle);
  }
  
  //----------
  
  void open(void) override {
    SDL_ShowWindow(MWindow);
    // realignChildren?
  }
  
  //----------
  
  void close(void) override {
    SDL_HideWindow(MWindow);
  }
  
  //----------
  
  //void flush(void) override {}
  //void sync(void) override {}
  //void reparent(void* AParent) override {}
  //void startTimer(uint32_t ms) override {}
  //void stopTimer(void) override {}
  //void setCursor(uint32_t ACursor) override {}
  //void setCursorPos(int32_t AXpos, int32_t AYpos) override {}
  //void hideCursor(void) override {}
  //void showCursor(void) override {}
  //void grabCursor(void) override {}
  //void releaseCursor(void) override {}
  
  //----------
  
  void beginPaint(void) override {
    uint8_t* ptr;
    int32_t  stride;
    SDL_LockTexture(
      MTexture,
      KODE_NULL,
      (void**)&ptr,
      (int*)&stride
    );
    //MBitmap.setup(MWidth,MHeight,stride,ptr);
    MBitmap.setup(MWidth,MHeight,32,stride,ptr);
  }
  
  //----------
  
  //void paint(KODE_Rect ARect) override {}
  
  //----------
  
  void endPaint(void) override {
    SDL_UnlockTexture(MTexture);
  }
  
  //----------
  
  void blit(int32_t dst_x, int32_t dst_y, KODE_Drawable* ADrawable, int32_t src_x, int32_t src_y, int32_t src_w, int32_t src_h) override {
    SDL_RenderCopy(MRenderer,MTexture,KODE_NULL,KODE_NULL);
    SDL_RenderPresent(MRenderer);
  }
  
  //----------
  
  void blit(int32_t dst_x, int32_t dst_y, KODE_Drawable* ADrawable, KODE_Rect src) override {
    blit(dst_x,dst_y,ADrawable,src.x,src.y,src.w,src.h);
  }
  
  //----------
  
  //void invalidate(KODE_Rect ARect) override {}
  //void sendEvent(uint32_t AData, uint32_t AType) override {}
  
  //----------
  
  void eventLoop(void) override {
    //_sdl_renderLoop(MRenderCallback);
    //K64_TIMELINE.setup();
    uint32_t frame_count = 0;
    bool quit = false;
    SDL_Event event;
    //KODE_AUDIO.startMusic();
    //uint32 timer = timer_start();
    float prev_time = 0;
    //if (K64_AUDIO.hasMusic()) prev_time = K64_AUDIO.getPosition();
    //else
    prev_time = (float)SDL_GetTicks() / 1000.0f;
    while (!quit) {
      while (SDL_PollEvent(&event)) {
        switch (event.type) {
          case SDL_QUIT:
            quit = true;
            break;
          case SDL_KEYDOWN:
            if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) quit = true;
            break;
        }
      }
      float cur_time; // in seconds
      //if (K64_AUDIO.hasMusic())
      //  cur_time = K64_AUDIO.getPosition();
      //else
      cur_time = (float)SDL_GetTicks() / 1000.0f;
      float elapsed = cur_time - prev_time;
      prev_time = cur_time;
      //KODE_Bitmap* bitmap = _sdl_beginPaint();
      beginPaint();
      on_renderFrame(&MBitmap,elapsed);
      endPaint();
      //_sdl_endPaint();
      //_sdl_blitBuffer();
      blit(0,0,KODE_NULL,0,0,0,0);
      frame_count += 1;
    }
  }
  
  //----------
  
  //void renderLoop(float ATime, KODE_RenderCallback) {}
  
//------------------------------
public:
//------------------------------

  /*
  void do_notify(KODE_Widget* ASender, int32_t AMsg) override {}
  void do_update(KODE_Widget* ASender) override {}
  void do_redraw(KODE_Widget* ASender, KODE_Rect ARect) override {}
  void do_moved(KODE_Widget* ASender, float AXpos, float AYpos) override {}
  void do_resized(KODE_Widget* ASender, float AWidth, float AHeight) override {}
  void do_setCursor(KODE_Widget* ASender, int32_t ACursor) override {}
  void do_setCursorPos(KODE_Widget* ASender, float AXpos, float AYpos) override {}
  void do_hint(KODE_Widget* ASender, const char* AHint) override {}
  void do_wantKeyEvents(KODE_Widget* ASender) override {}
  void do_setModal(KODE_Widget* ASender) override {}
  */
  //void do_startDrag(KODE_Widget* ASender, float AXpos, float AYpos, uint32_t AMode=0) override {}
  //void do_endDrag(KODE_Widget* ASender, float AXpos, float AYpos, uint32_t AMode=0) override {}
};


//----------------------------------------------------------------------
#endif