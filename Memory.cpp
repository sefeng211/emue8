#include "Memory.h"
#include <fstream>
#include <iostream>

using namespace std;

Memory::Memory() {}
void
Memory::LoadProgramIntoMemory(const char* aFileInput)
{

  std::ifstream ifs(aFileInput, std::ios::binary | std::ios::ate);
  if (!ifs) {
    cout << "Failed to open the program" << endl;
    std::exit(1);
  }

  auto end = ifs.tellg();
  ifs.seekg(0, std::ios::beg);

  auto size = std::size_t(end - ifs.tellg());

  cout << "Reading: " << aFileInput << endl;
  cout << "Size: " << size << endl;

  if (size > MEMORY_LIMIT) {
    cout << "Program exceeds the limit of the memory" << endl;
    std::exit(1);
  }

  ifs.read((char*)data + 0x200, size);
}

void
Memory::SaveByteTo(uint16_t aLocation, uint8_t aByte)
{
  data[aLocation] = aByte;
}

void
Memory::LoadETIProgramIntoMemory(const char* aFileInput)
{
  cout << "Reading ETI ROM: " << aFileInput << endl;
}

uint8_t
Memory::GetByteAt(size_t aIndex)
{
  if (aIndex >= MEMORY_LIMIT) {
    cout << "GetByteAt: Invalid index" << endl;
    std::exit(1);
  }
  return data[aIndex];
}

#ifdef DEBUG
void
Memory::PrintOutMemoryByRange(size_t aStart, size_t aEnd)
{
  size_t counter = 0;

  for (size_t i = aStart; i < aEnd; i++) {
    counter++;
    printf("%x", data[i]);
    printf(" ");

    // Print 30 hex numbers in a row
    if (counter % 30 == 0) {
      cout << endl;
    }
  }
}

void
Memory::PrintOutMemory()
{
  for (size_t i = 0; i < 4096; i++) {
    printf("%ld: %x \n", i, data[i]);
  }
  // cout << " ---------------- 0x000 (0) -------------------- " << endl;
  // PrintOutMemoryByRange(0, 0x200);
  // cout << endl;
  // cout << " ---------------- 0x200 (512) ------------------ " << endl;

  // PrintOutMemoryByRange(0x200, 0x600);
  // cout << endl;
  // cout << " ---------------- 0x600 (1536) ------------------ " << endl;

  // PrintOutMemoryByRange(0x600, 0xFFF);
  // cout << endl;
  // cout << " ---------------- 0xFFF (4095) ------------------ " << endl;
}

#endif
