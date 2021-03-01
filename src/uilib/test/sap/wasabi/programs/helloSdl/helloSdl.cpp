#include <SDL/SDL.h>
#include <iostream>
#include <emscripten.h>
using namespace std;
EM_JS(char, get_character, (), {
    return Module.inputAsync();
})
EM_JS(void, ff_exit, (), {
    return Module.ff_exit();
})
EM_JS(void, ff_openWindow, (const char* s), {
    return Module.ff_openWindow(Module.UTF8ToString(s));
})
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

extern "C" int main(int argc, char** argv) {
  ff_openWindow("Hello Sdl");
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Surface *screen = SDL_SetVideoMode(256, 256, 32, SDL_SWSURFACE);
  if (SDL_MUSTLOCK(screen)) SDL_LockSurface(screen);
  for (int i = 0; i < 256; i++) {
    for (int j = 0; j < 256; j++) {
      int alpha = (i+j) % 255;
      *((Uint32*)screen->pixels + i * 256 + j) = SDL_MapRGBA(screen->format, i, j, 255-i, alpha);
    }
  }
  if (SDL_MUSTLOCK(screen)) SDL_UnlockSurface(screen);
  SDL_Flip(screen); 

  cout << "you should see a smoothly-colored square - no sharp lines but the square borders!\n" << endl 
       << "and here is some text that should be HTML-friendly: amp: |&| double-quote: |\"| quote:"
       << " |'| less-than, greater-than, html-like tags: |<cheez></cheez>|\nanother line." << endl;
  SDL_Quit();

}