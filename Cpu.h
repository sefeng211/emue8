#ifndef CPU_H__
#define CPU_H__

#include <memory>
#include <tuple>

#include "Gfx.h"
#include "Keyboard.h"
#include "Memory.h"
#include "Register.h"

#define CPU_FREQUENCY 1600 // Cycles per seconds
#define RATE_60 60
#define FONTSET_SIZE 80

static uint8_t FONTSET[FONTSET_SIZE] = {
  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
  0x20, 0x60, 0x20, 0x20, 0x70, // 1
  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

class Cpu
{
public:
  Cpu(const std::shared_ptr<Memory>& aMemory,
      const std::shared_ptr<Register>& aRegister,
      const std::shared_ptr<Gfx>& aGfx,
      const std::shared_ptr<Keyboard>& aKeyboard);

  void DoOperation();

  void ExecuteOpcode();

private:
  uint16_t pc = 0x200; // Program Counter starts as 0x200

  uint32_t mCycleCount = CPU_FREQUENCY;

  std::tuple<const int, int> m60Hz =
    std::make_tuple(CPU_FREQUENCY / RATE_60, RATE_60);

  std::shared_ptr<Memory> mMemory;
  std::shared_ptr<Register> mRegister;
  std::shared_ptr<Gfx> mGfx;
  std::shared_ptr<Keyboard> mKeyboard;

  uint16_t GetNextOpcode();
  bool DoThingsAt60Hz();

  void StoreFontSet();
  void UpdateTimer();

private:
  bool mPause = false;
  /* ------------------ CPU Instructions ------------- */
  void Do0nnn();
  void Do00E0();
  void Do00EE();
  void Do1nnn(uint16_t nnn);
  void Do2nnn(uint16_t nnn);
  void Do3xkk(uint8_t x, uint8_t kk);
  // SNE Vx, byte
  void Do4xkk(uint8_t x, uint8_t kk);
  void Do5xy0(uint8_t x, uint8_t y);
  // LD Vx, byte
  // Set Vx = kk
  // The interpreter puts the value kk into register Vx
  void Do6xkk(uint8_t x, uint8_t kk);
  // Add Vx, byte
  void Do7xkk(uint8_t x, uint8_t kk);
  void Do8xy0(uint8_t x, uint8_t y);
  void Do8xy1(uint8_t x, uint8_t y);
  void Do8xy2(uint8_t x, uint8_t y);
  void Do8xy3(uint8_t x, uint8_t y);
  void Do8xy4(uint8_t x, uint8_t y);
  void Do8xy5(uint8_t x, uint8_t y);
  void Do8xy6(uint8_t x, uint8_t y);
  void Do8xy7(uint8_t x, uint8_t y);
  void Do8xyE(uint8_t x, uint8_t y);
  void Do9xy0(uint8_t x, uint8_t y);
  // LD I, addr
  // Set I = nnnn
  // The value of register I is set to nnn
  void DoAnnn(uint16_t nnn);
  void DoBnnn(uint16_t nnn);
  void DoCxkk(uint8_t x, uint8_t kk);
  // DRW Vx, Vy, nibble
  void DoDxyn(uint8_t x, uint8_t y, uint8_t n);
  void DoEx9E(uint8_t x);
  void DoExA1(uint8_t x);
  void DoFx07(uint8_t x);
  void DoFx0A(uint8_t x);
  // LD DT, Vx
  void DoFx15(uint8_t x);
  void DoFx18(uint8_t x);
  // Add I, Vx
  void DoFx1E(uint8_t x);
  void DoFx29(uint8_t x);
  void DoFx33(uint8_t x);
  void DoFx55(uint8_t x);
  void DoFx65(uint8_t x);
};

#endif
