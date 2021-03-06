// Chip-8 memory map from http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#1.0
// Memory Map:
// +---------------+= 0xFFF (4095) End of Chip-8 RAM
// |               |
// |               |
// |               |
// |               |
// |               |
// | 0x200 to 0xFFF|
// |     Chip-8    |
// | Program / Data|
// |     Space     |
// |               |
// |               |
// |               |
// +- - - - - - - -+= 0x600 (1536) Start of ETI 660 Chip-8 programs
// |               |
// |               |
// |               |
// +---------------+= 0x200 (512) Start of most Chip-8 programs
// | 0x000 to 0x1FF|
// | Reserved for  |
// |  interpreter  |
// +---------------+= 0x000 (0) Start of Chip-8 RAM

#ifndef MEMORY_H__
#define MEMORY_H__

#include <cstdint>

#define MEMORY_LIMIT 4096
#define STACK_LIMIT 16

class Memory
{
public:
  Memory();
  void LoadProgramIntoMemory(const char* aFileInput);
  void LoadETIProgramIntoMemory(const char* aFileInput);

  void SaveByteTo(uint16_t aLocation, uint8_t aByte);
  uint8_t GetByteAt(std::size_t aIndex);

  void addToStack(uint8_t& aSP, uint16_t aData)
  {
    mStack[aSP] = aData;
    aSP = (uint8_t)(aSP + 1);
  }

  uint16_t popFromStack(uint8_t& aSP)
  {
    aSP = (uint8_t)(aSP - 1);
    uint16_t ret = mStack[aSP];
    return ret;
  }
  ~Memory() {}
#ifdef DEBUG
  void PrintOutMemory();
  void PrintOutMemoryByRange(std::size_t start, std::size_t end);
#endif

private:
  uint8_t data[MEMORY_LIMIT] = { 0 };

  uint16_t mStack[STACK_LIMIT] = { 0 };
};

#endif
