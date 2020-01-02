#ifndef CPU_H__
#define CPU_H__

#include "Gfx.h"
#include "Memory.h"
#include "Register.h"
#include <memory>
#include <tuple>

#define CPU_FREQUENCY 800 // Cycles per seconds
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
      const std::shared_ptr<Gfx>& aGfx);

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

  uint16_t GetNextOpcode();
  bool DoThingsAt60Hz();

  void StoreFontSet();
  void UpdateTimer();

private:
  /* ------------------ CPU Instructions ------------- */
  void Do0nnn();
  void Do00E0();
  void Do1nnn(uint16_t nnn);
  void Do2nnn();
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
  void Do8xy1();
  void Do8xy2();
  void Do8xy3(uint8_t x ,uint8_t y);
  void Do8xy4(uint8_t x, uint8_t y);
  void Do8xy5(uint8_t x, uint8_t y);
  void Do8xy6();
  void Do8xy7();
  void Do8xyE();
  void Do9xy0(uint8_t x, uint8_t y);
  // LD I, addr
  // Set I = nnnn
  // The value of register I is set to nnn
  void DoAnnn(uint16_t nnn);
  void DoBnnn();
  void DoCxkk();
  // DRW Vx, Vy, nibble
  void DoDxyn(uint8_t x, uint8_t y, uint8_t n);
  void DoEx9E();
  void DoExA1();
  void DoFx07();
  void DoFx0A();
  // LD DT, Vx
  void DoFx15(uint8_t x);
  void DoFx18();
  // Add I, Vx
  void DoFx1E(uint8_t x);
  void DoFx29(uint8_t x);
  void DoFx33();
  void DoFx55(uint8_t x);
  void DoFx65(uint8_t x);
};

#endif
