# CHIP-8 Interpreter

A CHIP-8 interpreter written in C++

## Prerequisites

SFML and cmake are required for this project.<br/>Install them using

```bash
sudo apt-get install libsfml-dev make
```

## Running

Place your rom in roms/ folder. Then You can run the rom using

```bash
make all
./main (rom)
```
For example to play Pong,run
```bash
make all
./main Pong.ch8
```

## Keyboard Mapping:

![Alt text](/assets/Keyboard.png)

on the Keyboard correspond to

![Alt text](/assets/VIP.png)

on the interpreter
