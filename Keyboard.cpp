#include "Keyboard.h"
#include <iterator>

Keyboard::Keyboard() {}
bool
Keyboard::IsKeyUp(uint8_t aKey)
{
  printf("Checking IsKeyUp %x\n", uint8_t(aKey));
  return !mKeyboard[aKey];
}

void
Keyboard::SetKeyDown(KeyboardMapping aKey)
{
  printf("KeyDown: %x\n", uint8_t(aKey));
  mKeyboard[uint8_t(aKey)] = true;
}

void
Keyboard::SetKeyUp(KeyboardMapping aKey)
{
  mKeyboard[uint8_t(aKey)] = false;
}
//void
//Keyboard::Reset()
//{
  //std::fill(std::begin(mKeyboard), std::end(mKeyboard), false);
//}

bool
Keyboard::IsAnyKeyPressed()
{

  for (size_t i = 0; i < uint8_t(KeyboardMapping::COUNT); i++) {
    if (mKeyboard[i]) {
      return true;
    }
  }
  return false;
}
uint8_t
Keyboard::GetAnyKeyPressed()
{
  for (uint8_t i = 0; i < uint8_t(KeyboardMapping::COUNT); i++) {
    if (mKeyboard[i]) {
      return i;
    }
  }
  printf("Shouldn't reach to here \n");
  std::exit(1);
  return 0;
}
