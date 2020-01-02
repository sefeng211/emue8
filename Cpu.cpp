#include <array>
#include <bitset>
#include <cstdlib>
#include <iostream>

#include "Cpu.h"

using namespace std;

Cpu::Cpu(const std::shared_ptr<Memory>& aMemory,
         const std::shared_ptr<Register>& aRegister,
         const std::shared_ptr<Gfx>& aGfx)
{
  mMemory = aMemory;
  mRegister = aRegister;
  mGfx = aGfx;

  StoreFontSet();
}

void
Cpu::StoreFontSet()
{
  for (size_t i = 0; i < FONTSET_SIZE; i++) {
    mMemory->SaveByteTo(i, FONTSET[i]);
  }
}

void
Cpu::DoOperation()
{

  if (mCycleCount <= 0) {
    mCycleCount = CPU_FREQUENCY;
    std::get<1>(m60Hz) = RATE_60;
  }

  ExecuteOpcode();

  if (DoThingsAt60Hz()) {
    mGfx->UpdateScreen();
    UpdateTimer();
    // mTimer->UpdateTimer();
    std::get<1>(m60Hz) -= 1;
  }

  mCycleCount -= 2;
}

void
Cpu::ExecuteOpcode()
{
  uint16_t opcode = std::move(GetNextOpcode());
  uint8_t leastSigByte = opcode >> 12;
  printf("%x\n", opcode);
  switch (leastSigByte) {
    case 0x01: {
      cout << "0x01" << endl;
      uint16_t nnn = opcode & 0x0FFF;
      Do1nnn(nnn);
      break;
    }
    case 0x03: {
      cout << "0x03" << endl;
      uint8_t x = (opcode & 0x0F00) >> 8;
      uint8_t kk = opcode & 0x00FF;
      Do3xkk(x, kk);
      break;
    }
    case 0x04: {
      cout << "0x04" << endl;
      uint8_t x = (opcode & 0x0F00) >> 8;
      uint8_t kk = opcode & 0x00FF;
      Do4xkk(x, kk);
      break;
    }
    case 0x05: {
      cout << "0x05" << endl;
      uint8_t x = (opcode & 0x0F00) >> 8;
      uint8_t y = (opcode & 0x00F0) >> 4;
      Do5xy0(x, y);
      break;
    }
    case 0x06: {
      cout << "0x06" << endl;
      uint8_t x = (opcode & 0x0F00) >> 8;
      uint8_t kk = opcode & 0x00FF;
      Do6xkk(x, kk);
      break;
    }
    case 0x08: {
      cout << "0x08" << endl;
      uint8_t x = (opcode & 0x0F00) >> 8;
      uint8_t y = (opcode & 0x00F0) >> 4;
      uint8_t n = (opcode & 0x000F);
      if (n == 0x04) {
        Do8xy4(x, y);
        break;
      } else if (n == 0x00) {
        Do8xy0(x, y);
        break;
      } else if (n == 0x05) {
        Do8xy5(x, y);
        break;
      } else if (n == 0x03) {
        Do8xy3(x, y);
        break;
      } else {
        printf("Unsupported %x\n", opcode);
        std::exit(1);
      }
    }
    case 0x09: {
      uint8_t x = (opcode & 0x0F00) >> 8;
      uint8_t y = (opcode & 0x00F0) >> 4;
      cout << "0x09" << endl;
      Do9xy0(x, y);
      break;
    }
    case 0x0A: {
      cout << "0x0A" << endl;
      uint16_t nnn = opcode & 0x0FFF;
      DoAnnn(nnn);
      break;
    }

    case 0x0D: {
      printf("0x0D: %x\n", opcode);
      uint8_t x = (opcode & 0x0F00) >> 8;
      uint8_t y = (opcode & 0x00F0) >> 4;
      uint8_t n = (opcode & 0x000F);
      DoDxyn(x, y, n);
      break;
    }

    case 0x07: {
      printf("0x07: %x\n", opcode);
      uint8_t x = (opcode & 0x0F00) >> 8;
      uint8_t kk = opcode & 0x00FF;
      Do7xkk(x, kk);
      break;
    }

    case 0x0F: {
      uint8_t x = (opcode & 0x0F00) >> 8;
      uint8_t subopcode = opcode & 0x00FF;
      switch (subopcode) {
        case 0x15: {
          printf("0xFx15: %x\n", opcode);
          DoFx15(x);
          break;
        }
        case 0x18: {
          printf("Unknown 0xFx18: %x\n", opcode);
          std::exit(1);
        }
        case 0x1E: {
          DoFx1E(x);
          break;
        }
        case 0x29: {
          DoFx29(x);
          break;
        }
        case 0x33: {
          printf("Unknown 0xFx33: %x\n", opcode);
          std::exit(1);
        }
        case 0x55: {
          DoFx55(x);
          break;
        }
        case 0x65: {
          DoFx65(x);
          break;
        }
        default: {
          cout << "Unknown opcode: ";
          printf("%x %x\n", leastSigByte, opcode);
          std::exit(1);
          break;
        }
      }
      break;
    }
    default: {
      cout << "Unknown opcode: ";
      printf("%x %x\n", leastSigByte, opcode);
      std::exit(1);
    } break;
  }
}

bool
Cpu::DoThingsAt60Hz()
{
  return mCycleCount <= std::get<0>(m60Hz) * std::get<1>(m60Hz);
}

uint16_t
Cpu::GetNextOpcode()
{
  uint8_t firstByte = mMemory->GetByteAt(pc);
  uint8_t secondByte = mMemory->GetByteAt(pc + 1);

  pc += 2;

  return (firstByte << 8) | secondByte;
}

void
Cpu::UpdateTimer()
{
  if (mRegister->delayTimer > 0) {
    mRegister->delayTimer -= 1;
  }
  if (mRegister->soundTimer > 0) {
    mRegister->soundTimer -= 1;
  }
}

void
Cpu::Do1nnn(uint16_t nnn)
{
  pc = nnn;
}

void
Cpu::Do3xkk(uint8_t x, uint8_t kk)
{
  if (mRegister->registers[x] == kk) {
    pc += 2;
  }
}
void
Cpu::Do4xkk(uint8_t x, uint8_t kk)
{
  if (mRegister->registers[x] != kk) {
    pc += 2;
  }
}

void
Cpu::Do5xy0(uint8_t x, uint8_t y)
{
  if (mRegister->registers[x] == mRegister->registers[y]) {
    pc += 2;
  }
}
void
Cpu::Do6xkk(uint8_t x, uint8_t kk)
{
  mRegister->registers[x] = kk;
}

void
Cpu::Do7xkk(uint8_t x, uint8_t kk)
{
  mRegister->registers[x] += kk;
}

void
Cpu::Do8xy0(uint8_t x, uint8_t y)
{
  mRegister->registers[x] = mRegister->registers[y];
}

void
Cpu::Do8xy3(uint8_t x, uint8_t y)
{
  mRegister->registers[x] ^= mRegister->registers[y];
}

void
Cpu::Do8xy4(uint8_t x, uint8_t y)
{
  uint8_t sum = mRegister->registers[x] + mRegister->registers[y];

https: // stackoverflow.com/a/49024747/6766207
  if (sum < x) {
    mRegister->registers[0x0F] = 1;
  } else {
    mRegister->registers[0x0F] = 0;
  }

  mRegister->registers[x] = sum;
}

void
Cpu::Do8xy5(uint8_t x, uint8_t y)
{
  if (mRegister->registers[x] > mRegister->registers[y]) {
    mRegister->registers[0x0F] = 1;
  } else {
    mRegister->registers[0x0F] = 0;
  }

  mRegister->registers[x] = mRegister->registers[x] - mRegister->registers[y];
}
void
Cpu::Do9xy0(uint8_t x, uint8_t y)
{
  if (mRegister->registers[x] != mRegister->registers[y]) {
    pc += 2;
  }
}
void
Cpu::DoAnnn(uint16_t nnn)
{
  mRegister->I = nnn;
}

void
Cpu::DoDxyn(uint8_t x, uint8_t y, uint8_t n)
{
  uint16_t address = mRegister->I;

  if (address == 0) {
    // Error
    cout << "This address shouldn't be 0" << endl;
    return;
  }

  uint8_t nBytes[n];

  // Get the rows that we need to draw
  for (int i = 0; i < n; i++) {
    nBytes[i] = mMemory->GetByteAt(i + address);
    // printf("Read: %x\n", nBytes[i]);
    std::bitset<8> x(nBytes[i]);
    std::cout << "Read: " << x << std::endl;
    // printf("Read: %x\n", nBytes[i]);
  }

  // (Vx, Vy)
  uint8_t vx = mRegister->registers[x];
  uint8_t vy = mRegister->registers[y];

  bool collision = false;
  // mGfx->DoDxyn(vx, vy, nBytes, n, collision);
  mGfx->DoDxyn(vx, vy, nBytes, n, collision);

  if (collision) {
    mRegister->registers[0x0F] = 1;
  }

  // Debugging
  // printf("V%x: %x, V%x: %x \n", x, vx, y, vy);
  // printf("Data: ");
  // for (int i = 0; i < n; i++) {
  // printf("%x ", nBytes[i]);
  //}
  // printf("\n");
}

void
Cpu::DoFx15(uint8_t x)
{
  mRegister->delayTimer = mRegister->registers[x];
}

void
Cpu::DoFx1E(uint8_t x)
{
  mRegister->I += mRegister->registers[x];
}

void
Cpu::DoFx29(uint8_t x)
{
  uint8_t digit = mRegister->registers[x];
  // Times 5 to get memory location
  mRegister->I = digit * 0x05;
}

void
Cpu::DoFx55(uint8_t x)
{
  uint16_t address = mRegister->I;
  for (size_t i = 0; i <= x; i++) {
    printf("Saving %d to %x \n", mRegister->registers[i], address + i);
    mMemory->SaveByteTo(address + i, mRegister->registers[i]);
  }

  mRegister->I += x + 1;
}

void
Cpu::DoFx65(uint8_t x)
{
  uint16_t address = mRegister->I;
  for (size_t i = 0; i <= x; i++) {
    printf("Loading %d at %d in %x \n", mMemory->GetByteAt(address + i), address + i, i);
    mRegister->registers[i] = mMemory->GetByteAt(address + i);
  }
  mMemory->PrintOutMemory();
  mRegister->I += x + 1;
}
