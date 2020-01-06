#include <SDL2/SDL.h>
#include <chrono>
#include <cstdio>
#include <thread>

#include "Cpu.h"
#include "Gfx.h"
#include "Keyboard.h"
#include "Memory.h"
#include "Register.h"

#define SET_KEY_DOWN(KEY) keyboard->SetKeyDown(KeyboardMapping::KEY);
#define SET_KEY_UP(KEY) keyboard->SetKeyUp(KeyboardMapping::KEY);

int
main(int argc, char* argv[])
{

  std::shared_ptr<Memory> mem = std::make_shared<Memory>();
  std::shared_ptr<Register> reg = std::make_shared<Register>();
  std::shared_ptr<Gfx> gfx = std::make_shared<Gfx>();
  std::shared_ptr<Keyboard> keyboard = std::make_shared<Keyboard>();

  mem->LoadProgramIntoMemory(argv[1]);

  Cpu cpu(mem, reg, gfx, keyboard);

#ifdef DEBUG
  mem->PrintOutMemory();
#endif

  uint32_t sleepMS = 1.0 / (CPU_FREQUENCY / 2) * 1000000;
  // The initial assumption is each opcode takes 2 cpu cycles, it means the main
  // loop should run CPU_FREQUENCY / 2 times per second. There are caveats
  // though, such as things like context switches and graphic rendering all take
  // extra cycles. So we do a minus 100 here.
  std::chrono::microseconds dura(sleepMS - 100);

  SDL_Event event;

  bool quit = false;
  while (!quit) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        quit = true;
      } else if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
          case SDLK_x:
            SET_KEY_DOWN(SDLK_x);
            break;
          case SDLK_1:
            SET_KEY_DOWN(SDLK_1);
            break;
          case SDLK_2:
            SET_KEY_DOWN(SDLK_2);
            break;
          case SDLK_3:
            SET_KEY_DOWN(SDLK_3);
            break;
          case SDLK_q:
            SET_KEY_DOWN(SDLK_q);
            break;
          case SDLK_w:
            SET_KEY_DOWN(SDLK_w);
            break;
          case SDLK_e:
            SET_KEY_DOWN(SDLK_e);
            break;
          case SDLK_a:
            SET_KEY_DOWN(SDLK_a);
            break;
          case SDLK_s:
            SET_KEY_DOWN(SDLK_s);
            break;
          case SDLK_d:
            SET_KEY_DOWN(SDLK_d);
            break;
          case SDLK_z:
            SET_KEY_DOWN(SDLK_z);
            break;
          case SDLK_c:
            SET_KEY_DOWN(SDLK_c);
            break;
          case SDLK_4:
            SET_KEY_DOWN(SDLK_4);
            break;
          case SDLK_r:
            SET_KEY_DOWN(SDLK_r);
            break;
          case SDLK_f:
            SET_KEY_DOWN(SDLK_f);
            break;
          case SDLK_v:
            SET_KEY_DOWN(SDLK_v);
            break;
          default:
            break;
        }
      } else if (event.type == SDL_KEYUP) {
        switch (event.key.keysym.sym) {
          case SDLK_x:
            SET_KEY_UP(SDLK_x);
            break;
          case SDLK_1:
            SET_KEY_UP(SDLK_1);
            break;
          case SDLK_2:
            SET_KEY_UP(SDLK_2);
            break;
          case SDLK_3:
            SET_KEY_UP(SDLK_3);
            break;
          case SDLK_q:
            SET_KEY_UP(SDLK_q);
            break;
          case SDLK_w:
            SET_KEY_UP(SDLK_w);
            break;
          case SDLK_e:
            SET_KEY_UP(SDLK_e);
            break;
          case SDLK_a:
            SET_KEY_UP(SDLK_a);
            break;
          case SDLK_s:
            SET_KEY_UP(SDLK_s);
            break;
          case SDLK_d:
            SET_KEY_UP(SDLK_d);
            break;
          case SDLK_z:
            SET_KEY_UP(SDLK_z);
            break;
          case SDLK_c:
            SET_KEY_UP(SDLK_c);
            break;
          case SDLK_4:
            SET_KEY_UP(SDLK_4);
            break;
          case SDLK_r:
            SET_KEY_UP(SDLK_r);
            break;
          case SDLK_f:
            SET_KEY_UP(SDLK_f);
            break;
          case SDLK_v:
            SET_KEY_UP(SDLK_v);
            break;
          default:
            break;
        }
      }
    }

    cpu.DoOperation();
#ifdef DEBUG
    reg->PrintRegister();
#endif

    // A note is that waking up thread also takes time, however we don't
    // accommodate them at the moment
    std::this_thread::sleep_for(dura);
  }
  return 0;
}
