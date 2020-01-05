#ifndef Keyboard_H__
#define Keyboard_H__

#include <cstdint>

// 1|2|3|C
// 4|5|6|D
// 7|8|9|E
// A|0|B|F
enum class KeyboardMapping
{
  SDLK_x, // 0
  SDLK_1, // 1
  SDLK_2, // 2
  SDLK_3, // 3
  SDLK_q, // 4
  SDLK_w, // 5
  SDLK_e, // 6
  SDLK_a, // 7
  SDLK_s, // 8
  SDLK_d, // 9
  SDLK_z, // A
  SDLK_c, // B
  SDLK_4, // C
  SDLK_r, // D
  SDLK_f, // E
  SDLK_v, // F
  COUNT
};

class Keyboard
{
public:
  Keyboard();

  bool IsKeyUp(uint8_t aKey);

  bool IsAnyKeyPressed();
  uint8_t GetAnyKeyPressed();

  void SetKeyDown(KeyboardMapping aKey);
  void SetKeyUp(KeyboardMapping aKey);

  //void Reset();

  bool mKeyboard[uint8_t(KeyboardMapping::COUNT)] = { false };
};

#endif
