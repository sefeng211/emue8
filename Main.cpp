#include <SDL2/SDL.h>
#include <chrono>
#include <cstdio>
#include <thread>

#include "Cpu.h"
#include "Gfx.h"
#include "Memory.h"
#include "Register.h"

int
main(int argc, char* argv[])
{

  std::shared_ptr<Memory> mem = std::make_shared<Memory>();
  std::shared_ptr<Register> reg = std::make_shared<Register>();
  std::shared_ptr<Gfx> gfx = std::make_shared<Gfx>();

  mem->LoadProgramIntoMemory(argv[1]);

  Cpu cpu(mem, reg, gfx);

  // Assume each opcode takes 2 cpu cycles, it means the main loop should
  // run CPU_FREQUENCY / 2 times per second
  uint32_t sleepMS = 1.0 / (CPU_FREQUENCY / 2) * 1000000;
  std::chrono::microseconds dura(sleepMS);

  SDL_Event event;

  bool quit = false;
  while (!quit) {

  //for (int i= 0; i< 30; i++) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        quit = true;
      }
    }

    cpu.DoOperation();
    // cpu.ExecuteOpcode(); // 2 cpu cycle

    // gfx->UpdateScreen(cpuCycle);

    reg->PrintRegister();

    // A note is that waking up thread also takes time, however we don't
    // accommodate them at the moment
    std::this_thread::sleep_for(dura);
  }
  return 0;
}
