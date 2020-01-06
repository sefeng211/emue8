#include <algorithm>
#include <iterator>

#include "Keyboard.h"

Keyboard::Keyboard() {}
bool
Keyboard::IsKeyUp(uint8_t aKey)
{
  return !mKeyboard[aKey];
}

void
Keyboard::SetKeyDown(KeyboardMapping aKey)
{
  mKeyboard[uint8_t(aKey)] = true;
}

void
Keyboard::SetKeyUp(KeyboardMapping aKey)
{
  mKeyboard[uint8_t(aKey)] = false;
}

bool
Keyboard::IsAnyKeyPressed()
{
  return std::any_of(std::begin(mKeyboard),
                     std::end(mKeyboard),
                     [](bool& aIsKeyPressed) { return aIsKeyPressed; });
}

uint8_t
Keyboard::GetAnyKeyPressed()
{
  for (size_t i = 0; i < uint8_t(KeyboardMapping::COUNT); i++) {
    if (mKeyboard[i]) {
      return i;
    }
  }

  printf("GetAnyKeyPressed should return a pressed key\n");
  std::exit(1);
}
