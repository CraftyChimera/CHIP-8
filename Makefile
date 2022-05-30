flags=-lsfml-graphics -lsfml-window -lsfml-system -Wall -Wextra -Wshadow -pedantic

all: cpu base graphics
	g++ -o main main.cpp cpu.o base.o graphics.o $(flags)

base: Base/base.cpp Base/base.hpp
	g++ Base/base.cpp -c -o base.o $(flags)

cpu: Cpu/cpu.cpp Cpu/cpu.hpp
	g++ Cpu/cpu.cpp -c -o cpu.o $(flags)

graphics: Graphics/graphics.cpp Graphics/graphics.hpp
	g++ Graphics/graphics.cpp -c -o graphics.o $(flags)

clean:
	rm *.o main

test: test.cpp
	g++ test.cpp -o test $(flags)