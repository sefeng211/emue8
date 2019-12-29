#ifndef CPU_H__
#define CPU_H__

#include "Memory.h"
#include "Register.h"
#include "Gfx.h"
#include <memory>

#define CPU_FREQUENCY 800 // Cycles per seconds

class Cpu
{
public:
  Cpu(const std::shared_ptr<Memory>& aMemory,
      const std::shared_ptr<Register>& aRegister,
      const std::shared_ptr<Gfx>& aGfx);

  void ExecuteOpcode();

private:
  uint16_t pc = 0x200; // Program Counter starts as 0x200

  uint16_t GetNextOpcode();
  std::shared_ptr<Memory> mMemory;
  std::shared_ptr<Register> mRegister;
  std::shared_ptr<Gfx> mGfx;

  /* ------------------ CPU Instructions ------------- */
  void Do0nnn();
  void Do00E0();
  void Do1nnn();
  void Do2nnn();
  void Do3xkk();
  void Do4xkk();
  void Do5xy0();
  // LD Vx, byte
  // Set Vx = kk
  // The interpreter puts the value kk into register Vx
  void Do6xkk(uint8_t x, uint8_t kk);
  // Add Vx, byte
  void Do7xkk(uint8_t x, uint8_t kk);
  void Do8xy0();
  void Do8xy1();
  void Do8xy2();
  void Do8xy3();
  void Do8xy4();
  void Do8xy5();
  void Do8xy6();
  void Do8xy7();
  void Do8xyE();
  void Do9xy0();
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
  void DoFx15(uint8_t x);
  void DoFx18();
  void DoFx1E();
  void DoFx29();
  void DoFx33();
  void DoFx55();
  void DoFx65();
};

#endif
