#include <array>
#include <bitset>
#include <iostream>

#include <cstdlib>

#include "Cpu.h"

using namespace std;

Cpu::Cpu(const std::shared_ptr<Memory>& aMemory,
         const std::shared_ptr<Register>& aRegister,
         const std::shared_ptr<Gfx>& aGfx,
         const std::shared_ptr<Keyboard>& aKeyboard)
{
  mMemory = aMemory;
  mRegister = aRegister;
  mGfx = aGfx;
  mKeyboard = aKeyboard;

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
    std::get<1>(m60Hz) -= 1;
  }

  if (!mPause) {
    printf("Not Paused, Increment pc\n");
    pc += 2;
  }
  mCycleCount -= 2;
}

void
Cpu::ExecuteOpcode()
{
  uint16_t opcode = std::move(GetNextOpcode());
  uint8_t leastSigByte = opcode >> 12;
  printf("opcode: %x, pc: %d\n", opcode, pc);
  switch (leastSigByte) {
    case 0x00: {
      switch (opcode) {
        case 0x00E0: {
          cout << "0x00E0" << endl;
          Do00E0();
          break;
        }
        case 0x00EE: {
          cout << "0x00EE" << endl;
          Do00EE();
          break;
        }
        default: {
          // 0x0nnn
          uint16_t nnn = opcode & 0x0FFF;
          // Do0nnn(nnn);
          printf("Unsupported %x\n", opcode);
          // std::exit(1);
          break;
        }
      }
      break;
    }
    case 0x01: {
      cout << "0x01" << endl;
      uint16_t nnn = opcode & 0x0FFF;
      Do1nnn(nnn);
      break;
    }
    case 0x02: {
      cout << "0x02" << endl;
      uint16_t nnn = opcode & 0x0FFF;
      Do2nnn(nnn);
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
      } else if (n == 0x01) {
        Do8xy1(x, y);
        break;
      } else if (n == 0x02) {
        Do8xy2(x, y);
        break;
      } else if (n == 0x05) {
        Do8xy5(x, y);
        break;
      } else if (n == 0x06) {
        Do8xy6(x, y);
        break;
      } else if (n == 0x07) {
        Do8xy7(x, y);
        break;
      } else if (n == 0x03) {
        Do8xy3(x, y);
        break;
      } else if (n == 0x0E) {
        Do8xyE(x, y);
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

    case 0x0B: {
      uint16_t nnn = opcode & 0x0FFF;
      DoBnnn(nnn);
      break;
    }

    case 0x0C: {
      cout << "0x0C" << endl;
      uint8_t x = (opcode & 0x0F00) >> 8;
      uint8_t kk = opcode & 0x00FF;
      DoCxkk(x, kk);
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

    case 0x0E: {
      uint8_t x = (opcode & 0x0F00) >> 8;
      uint8_t subopcode = opcode & 0x00FF;
      switch (subopcode) {
        case 0x9E: {
          DoEx9E(x);
          break;
        }
        case 0xA1: {
          printf("0xExA1 %x\n", opcode);
          DoExA1(x);
          break;
        }
        default: {
          std::exit(1);
          break;
        }
      }
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
        case 0x07: {
          printf("0xFx07: %x\n", opcode);
          DoFx07(x);
          break;
        }
        case 0x0A: {
          DoFx0A(x);
          break;
        }
        case 0x15: {
          printf("0xFx15: %x\n", opcode);
          DoFx15(x);
          break;
        }
        case 0x18: {
          printf("0xFx18: %x\n", opcode);

          DoFx18(x);
          break;
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
          DoFx33(x);
          break;
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

  // pc += 2;
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

// Clear the screen
void
Cpu::Do00E0()
{
  mGfx->ClearScreen();
}

void
Cpu::Do00EE()
{
  pc = mMemory->popFromStack(mRegister->SP);
}

void
Cpu::Do1nnn(uint16_t nnn)
{
  pc = nnn - 2; // Not increase the pc;
}

void
Cpu::Do2nnn(uint16_t nnn)
{
  mMemory->addToStack(mRegister->SP, pc);
  // We want to call subrountine at nnn; Needs to decrease pc by 2
  // because at the end of execution, it'll be increased by 2
  pc = nnn - 2;
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
Cpu::Do8xy1(uint8_t x, uint8_t y)
{
  mRegister->registers[x] |= mRegister->registers[y];
}
void
Cpu::Do8xy2(uint8_t x, uint8_t y)
{
  mRegister->registers[x] &= mRegister->registers[y];
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
Cpu::Do8xy6(uint8_t x, uint8_t y)
{
  if (mRegister->registers[x] & 0x1) {
    mRegister->registers[0x0F] = 1;
  } else {
    mRegister->registers[0x0F] = 0;
  }
  mRegister->registers[x] /= 2;
}

void
Cpu::Do8xy7(uint8_t x, uint8_t y)
{
  if (mRegister->registers[y] > mRegister->registers[x]) {
    mRegister->registers[0x0F] = 1;
  } else {
    mRegister->registers[0x0F] = 0;
  }

  mRegister->registers[x] = mRegister->registers[y] - mRegister->registers[x];
}
void
Cpu::Do8xyE(uint8_t x, uint8_t y)
{
  if (mRegister->registers[x] >> 7) {
    mRegister->registers[0x0F] = 1;
  } else {
    mRegister->registers[0x0F] = 0;
  }
  mRegister->registers[x] *= 2;
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
Cpu::DoBnnn(uint16_t nnn)
{
  pc = nnn + mRegister->registers[0] - 2;
}

void
Cpu::DoCxkk(uint8_t x, uint8_t kk)
{
  uint8_t r = rand() % 256;
  mRegister->registers[x] = r & kk;
}

void
Cpu::DoEx9E(uint8_t x)
{
  if (!mKeyboard->IsKeyUp(mRegister->registers[x])) {
    pc += 2;
  }
}
void
Cpu::DoExA1(uint8_t x)
{
  if (mKeyboard->IsKeyUp(mRegister->registers[x])) {
    pc += 2;
  }
}

void
Cpu::DoDxyn(uint8_t x, uint8_t y, uint8_t n)
{
  uint16_t address = mRegister->I;

  // if (address == 0) {
  //// Error
  // cout << "This address shouldn't be 0" << endl;
  // std::exit(1);
  // return;
  //}

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
  } else {
    mRegister->registers[0x0F] = 0;
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
Cpu::DoFx07(uint8_t x)
{
  mRegister->registers[x] = mRegister->delayTimer;
}

void
Cpu::DoFx0A(uint8_t x)
{
  mPause = false;
  // Wait for a key press, store the value of the key in Vx
  if (mKeyboard->IsAnyKeyPressed()) {
    mRegister->registers[x] = mKeyboard->GetAnyKeyPressed();
  } else {
    mPause = true;
  }
}

void
Cpu::DoFx15(uint8_t x)
{
  mRegister->delayTimer = mRegister->registers[x];
}

void
Cpu::DoFx18(uint8_t x)
{
  mRegister->soundTimer = mRegister->registers[x];
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
Cpu::DoFx33(uint8_t x)
{
  uint8_t vx = mRegister->registers[x];
  uint8_t d1 = (vx / 100) % 10;
  uint8_t d2 = (vx / 10) % 10;
  uint8_t d3 = vx % 10;

  mMemory->SaveByteTo(mRegister->I, d1);
  mMemory->SaveByteTo(mRegister->I + 1, d2);
  mMemory->SaveByteTo(mRegister->I + 2, d3);
}

void
Cpu::DoFx55(uint8_t x)
{
  uint16_t address = mRegister->I;
  for (size_t i = 0; i <= x; i++) {
    mMemory->SaveByteTo(address + i, mRegister->registers[i]);
  }
}

void
Cpu::DoFx65(uint8_t x)
{
  uint16_t address = mRegister->I;
  for (size_t i = 0; i <= x; i++) {
    mRegister->registers[i] = mMemory->GetByteAt(address + i);
  }
  mMemory->PrintOutMemory();
}
