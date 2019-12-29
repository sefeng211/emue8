#include <cstdio>

#include "Cpu.h"
#include "Gfx.h"
#include "Memory.h"
#include "Register.h"

int
main(int argc, char* argv[])
{

  std::shared_ptr<Memory> mem = std::make_shared<Memory>();
  std::shared_ptr<Register> reg = std::make_shared<Register>();
  std::shared_ptr<Gfx> gfx = std::make_shared<Gfx>(CPU_FREQUENCY);

  mem->LoadProgramIntoMemory(argv[1]);

  Cpu cpu(mem, reg, gfx);

  // for (;;) {
  for (int i = 0; i < 30; i++) {
    cpu.ExecuteOpcode();
    gfx->UpdateScreen();

    reg->PrintRegister();
    // break;
  }
  return 0;
}
