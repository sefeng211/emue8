// Chip-8 has 15 8-bit general purpose registers named V0..V1 and up to VE

#ifndef REGISTER_H__
#define REGISTER_H__

#include <cstdio>

class Register
{
public:
  Register() {}

#ifdef DEBUG
  void PrintRegister() {
    for (int i = 0; i< 16; i++) {
      printf("%x ", registers[i]);
    }
    printf("I:%x \n", I);
    printf("Delay Timer:%x \n", delayTimer);
    printf("Sound Timer:%x \n", soundTimer);
  }
#endif

  uint8_t registers[16] = {0}; // 16 general-purpose registers
  uint16_t I = 0;
  uint8_t delayTimer = 0;
  uint8_t soundTimer = 0;
};

#endif
