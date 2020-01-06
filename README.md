Emue8 is a Chip-8 emulator written in C++ and SDL2.

I have only tested on `Linux`, however I think it's compatible on other platforms.

`SDL2` and `gcc` are required to build this project.

To Compile
```
    mkdir build
    cd build
    cmake ..
    make

```
To compile the debugging version, which is going to print a lot of debugging messages
```
    cmake -DDEBUGGING_MESSAGE=ON ..
    make
```

To run a ROM
```
    ./emue8 <path to the ROM>
```

