#include "Cpu/cpu.hpp"
using std::cout;

int main()
{
    std::string_view s = "tests/IBM_Logo.ch8";
    CPU cpu;
    bool flag = true;
    vector<byte> data = read_file(s);
    cpu.Load_Data(data);
    sf::RenderWindow window(sf::VideoMode(64, 32), "CHIP-8");
    sf::Texture texture;
    if (!texture.loadFromFile("assets/in.png"))
    {
        std::cout << "error Loading the file";
        return 0;
    };
    DrawWindow(window, texture);
    window.setPosition(sf::Vector2i(200, 300));

    while (window.isOpen())
    {
        window.clear(sf::Color::Black);
        sf::Event event;
        while (flag && cpu.pc < 0x1000 - 1)
        {
            while (window.pollEvent(event))
            {

                switch (event.type)
                {
                case sf::Event::Closed:
                {
                    window.close();
                    flag = false;
                    break;
                }
                default:
                    break;
                }
            }
            word part1 = cpu.ram[cpu.pc] << 8, part2 = cpu.ram[cpu.pc + 1];
            word instruction = part1 + part2;
            cpu.pc += 2;
            byte first = (instruction >> 12);
            if (instruction == 0x00e0)
                cpu.Clear_Screen(window);
            if (first == 0x1)
                cpu.Jump(instruction);
            if (first == 0x6)
                cpu.Set_Register(instruction);
            if (first == 0x7)
                cpu.Add_Register(instruction);
            if (first == 0xA)
                cpu.Set_Index(instruction);
            if (first == 0xD)
                cpu.Draw(instruction, window, texture);
        }
    }

    return 0;
}