all: cpu base graphics
	g++ -o main main.cpp cpu.o base.o graphics.o -lsfml-graphics -lsfml-window -lsfml-system -Wall -Wextra -Wshadow -pedantic

base: Base/base.cpp Base/base.hpp
	g++ Base/base.cpp -c -o base.o -lsfml-graphics -lsfml-window -lsfml-system -Wall -Wextra -Wshadow -pedantic

cpu: Cpu/cpu.cpp Cpu/cpu.hpp
	g++ Cpu/cpu.cpp -c -o cpu.o -lsfml-graphics -lsfml-window -lsfml-system -Wall -Wextra -Wshadow -pedantic

graphics: Graphics/graphics.cpp Graphics/graphics.hpp
	g++ Graphics/graphics.cpp -c -o graphics.o -lsfml-graphics -lsfml-window -lsfml-system -Wall -Wextra -Wshadow -pedantic

clean:
	rm *.o main