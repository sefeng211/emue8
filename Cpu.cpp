#include "Cpu.h"
#include <array>
#include <iostream>

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
Cpu::ExecuteOpcode()
{
  uint16_t opcode = std::move(GetNextOpcode());
  uint8_t leastSigByte = opcode >> 12;
  printf("%x\n", leastSigByte);
  switch (leastSigByte) {
    case 0x06: {
      cout << "0x06" << endl;
      uint8_t x = (opcode & 0x0F00) >> 8;
      uint8_t kk = opcode & 0x00FF;
      Do6xkk(x, kk);
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
          break;
        }
        case 0x1E: {
          printf("Unknown 0xFx1E: %x\n", opcode);
          break;
        }
        case 0x29: {
          printf("Unknown 0xFx29: %x\n", opcode);
          break;
        }
        case 0x33: {
          printf("Unknown 0xFx33: %x\n", opcode);
          break;
        }
        case 0x55: {
          printf("Unknown 0xFx33: %x\n", opcode);
          break;
        }
        case 0x65: {
          printf("Unknown 0xFx33: %x\n", opcode);
          break;
        }
      }
      break;
    }
    default: {
      cout << "Unknown opcode: ";
      printf("%x %x\n", leastSigByte, opcode);
    } break;
  }
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
  }

  // (Vx, Vy)
  uint8_t vx = mRegister->registers[x];
  uint8_t vy = mRegister->registers[y];

  mGfx->DoDxyn(vx, vy, nBytes, n);

  // Debugging
  // printf("V%x: %x, V%x: %x \n", x, vx, y, vy);
  // printf("Data: ");
  // for (int i = 0; i < n; i++) {
  // printf("%x ", nBytes[i]);
  //}
  // printf("\n");
}

void
Cpu::DoFx15(uint8_t x) {

}
