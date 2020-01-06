#include "Gfx.h"
#include <algorithm>
#include <bitset>
#include <iostream>
#include <iterator>

Gfx::Gfx()
{

  // Start SDL
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    printf("Failed to init SDL\n");
    std::exit(1);
  }
  SDL_CreateWindowAndRenderer(
    WINDOW_WIDTH, WINDOW_HEIGHT, 0, &mWindow, &mRenderer);

  if (!mWindow || !mRenderer) {
    printf("Window or Renderer could not be created! SDL_Error: %s\n",
           SDL_GetError());
    std::exit(1);
  }

  mTexture = SDL_CreateTexture(mRenderer,
                               SDL_PIXELFORMAT_ARGB8888,
                               SDL_TEXTUREACCESS_STATIC,
                               LOGIC_WIDTH,
                               LOGIC_HEIGHT);

  SDL_RenderSetLogicalSize(mRenderer, LOGIC_WIDTH, LOGIC_HEIGHT);

  std::fill(std::begin(mPixels), std::end(mPixels), BLACK_PIXEL);
}

void
Gfx::UpdateScreen()
{
  int ret =
    SDL_UpdateTexture(mTexture, NULL, mPixels, LOGIC_WIDTH * sizeof(uint32_t));
  if (ret != 0) {
    printf("Failed to update Texture \n");
    std::exit(1);
  }
  SDL_RenderClear(mRenderer);
  SDL_RenderCopy(mRenderer, mTexture, NULL, NULL);
  SDL_RenderPresent(mRenderer);
}

void
Gfx::ClearScreen()
{
  std::fill(std::begin(mPixels), std::end(mPixels), BLACK_PIXEL);
}

Gfx::~Gfx()
{
  // Quit SDL
  SDL_DestroyRenderer(mRenderer);
  SDL_DestroyWindow(mWindow);
  SDL_Quit();
}

void
Gfx::DoDxyn(uint8_t aVx,
            uint8_t aVy,
            uint8_t* aByteArray,
            size_t aSize,
            bool& aCollision)
{
  for (size_t i = 0; i < aSize; i++) { // For each row
    size_t startIndex = aVx + (aVy + i) * LOGIC_WIDTH;
    for (int k = 7; k >= 0; k--) {
      bool isNotSet = aByteArray[i] & (1 << k);
      if (!isNotSet) {
        mPixels[startIndex] ^= BLACK_PIXEL;
      } else {
        // If the existing pixel is WHITE, this means the pixel is
        // going to be flipped from 1 to 0;
        if (mPixels[startIndex] == WHITE_PIXEL) {
          aCollision = true;
        }
        mPixels[startIndex] ^= WHITE_PIXEL;
      }
      startIndex++;
    }
  }
}

#ifdef DEBUG
void
Gfx::PrintPixels()
{
  for (int i = 0; i < LOGIC_WIDTH * LOGIC_HEIGHT; i++) {
    if (mPixels[i] == BLACK_PIXEL) {
      printf("0 ");
    } else {
      printf("1 ");
    }

    if (i % 64 == 0) {
      printf("\n");
    }
  }
}
#endif
