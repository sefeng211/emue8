#ifndef GFX_H__
#define GFX_H__

#include <SDL2/SDL.h>

#define WINDOW_HEIGHT 320
#define WINDOW_WIDTH 640
#define LOGIC_HEIGHT 32
#define LOGIC_WIDTH 64

#define REFRESH_RATE 60 // A second refresh 60 times
class Gfx
{
public:
  Gfx(const int aCpuFrequency);
  ~Gfx();

  void UpdateScreen();

  void PrintPixels();

  void DoDxyn(uint8_t aVx, uint8_t aVy, uint8_t* aByteArray, size_t aSize);
private:
  SDL_Window* mWindow = nullptr;
  SDL_Renderer *mRenderer = nullptr;
  SDL_Texture* mTexture = nullptr;

  // Store the pixels representation in memory
  // Each pixel has 2 possible values, 0 or 1
  uint32_t mPixels[LOGIC_WIDTH * LOGIC_HEIGHT];

  const int mRefreshCycle; // update the screen at every refresh cycle
};
#endif
