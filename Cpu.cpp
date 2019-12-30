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
  printf("%x\n", leastSigByte);
  switch (leastSigByte) {
    case 0x01: {
      uint16_t nnn = opcode & 0x0FFF;
      Do1nnn(nnn);
      break;
    }
    case 0x04: {
      cout << "0x04" << endl;
      uint8_t x = (opcode & 0x0F00) >> 8;
      uint8_t kk = opcode & 0x00FF;
      Do4xkk(x, kk);
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
      uint8_t x = (opcode & 0x0F00) >> 8;
      uint8_t y = (opcode & 0x00F0) >> 4;
      uint8_t n = (opcode & 0x000F);
      if (n == 0x04) {
        Do8xy4(x, y);
        break;
      } else {
        printf("Unsupported %x\n", opcode);
      }
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
        }
        case 0x1E: {
          printf("Unknown 0xFx1E: %x\n", opcode);
        }
        case 0x29: {
          printf("Unknown 0xFx29: %x\n", opcode);
        }
        case 0x33: {
          printf("Unknown 0xFx33: %x\n", opcode);
        }
        case 0x55: {
          printf("Unknown 0xFx33: %x\n", opcode);
        }
        case 0x65: {
          printf("Unknown 0xFx33: %x\n", opcode);
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
Cpu::Do4xkk(uint8_t x, uint8_t kk)
{
  if (mRegister->registers[x] != kk) {
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
    //printf("Read: %x\n", nBytes[i]);
    std::bitset<8> x(nBytes[i]);
    std::cout << "Read: " << x << std::endl;
    //printf("Read: %x\n", nBytes[i]);
  }

  // (Vx, Vy)
  uint8_t vx = mRegister->registers[x];
  uint8_t vy = mRegister->registers[y];

  bool collision = false;
  //mGfx->DoDxyn(vx, vy, nBytes, n, collision);
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
