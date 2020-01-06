#ifndef GFX_H__
#define GFX_H__

#include <SDL2/SDL.h>

#define WINDOW_HEIGHT 320
#define WINDOW_WIDTH 640
#define LOGIC_HEIGHT 32
#define LOGIC_WIDTH 64

#define BLACK_PIXEL 0x00000000
#define WHITE_PIXEL 0xFFFFFFFF

#define REFRESH_RATE 60 // A second refresh 60 times

class Gfx
{
public:
  Gfx();
  ~Gfx();

  void UpdateScreen();
  void ClearScreen();
  void DoDxyn(uint8_t aVx,
              uint8_t aVy,
              uint8_t* aByteArray,
              size_t aSize,
              bool& aCollision);

#ifdef DEBUG
  void PrintPixels();
#endif

private:
  SDL_Window* mWindow = nullptr;
  SDL_Renderer* mRenderer = nullptr;
  SDL_Texture* mTexture = nullptr;

  // Store the pixels representation in memory
  // Each pixel has 2 possible values, 0 or 1
  uint32_t mPixels[LOGIC_WIDTH * LOGIC_HEIGHT];
};
#endif
