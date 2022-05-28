#include "Cpu/cpu.hpp"
using std::cout;

int main()
{
    std::string_view s = "tests/IBM_Logo.ch8";
    CPU cpu;
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
        // bool pressed = false;

        while (cpu.pc < 0xFFF)
        {
            DrawWindow(window, texture);
            while (window.pollEvent(event))
            {

                switch (event.type)
                {
                /*case sf::Event::KeyPressed:
                {
                    pressed = true;
                    if (event.key.code >= sf::Keyboard::Num0 && event.key.code <= sf::Keyboard::Num9)
                        a = event.key.code - sf::Keyboard::Num0;
                }*/
                case sf::Event::Closed:
                {
                    window.close();
                    break;
                }
                default:
                    break;
                    // pressed = false;
                }
            }
            word part1 = cpu.ram[cpu.pc] << 8, part2 = cpu.ram[cpu.pc + 1];
            word instruction = part1 + part2;
            cpu.pc += 2;
            byte first = (instruction >> 12);
            switch (first)
            {
            case 0x0:
            {
                if (instruction == 0x00e0)
                    cpu.Clear_Screen(window);
                if (instruction == 0x00ee)
                    cpu.Sub(instruction);
                break;
            }
            case 0x1:
            {
                cpu.Jump(instruction);
                break;
            }
            case 0x2:
            {
                cpu.Sub(instruction);
                break;
            }
            case 0x3:
            case 0x4:
            case 0x5:
            {
                cpu.Skip(instruction);
                break;
            }
            case 0x6:
            {
                cpu.Set_Register(instruction);
                break;
            }
            case 0x7:
            {
                cpu.Add_Register(instruction);
                break;
            }
            case 0x8:
            {
                cpu.Arithmetic(instruction);
                break;
            }
            case 0x9:
            {
                cpu.Skip(instruction);
                break;
            }
            case 0xA:
            {
                cpu.Set_Index(instruction);
                break;
            }
            case 0xB:
            {
                cpu.Jump_Offset(instruction);
                break;
            }
            case 0xC:
            {
                cpu.Random(instruction);
                break;
            }
            case 0xD:
            {
                cpu.Draw(instruction, window, texture);
                break;
            }
            case 0xE:
            {
                break;
            }

            case 0xF:
            {
                // 07,15,18,0A
                byte value = instruction & ((1 << 8) - 1);
                switch (value)
                {
                case 0x1e:
                {
                    cpu.Add_Index(instruction);
                    break;
                }
                case 0x29:
                {
                    cpu.Font(instruction);
                    break;
                }
                case 0x33:
                {
                    cpu.BCD(instruction);
                    break;
                }
                case 0x55:
                case 0x65:
                {
                    cpu.Store_and_Load(instruction);
                    break;
                }
                default:
                    break;
                }
                break;
            }
            }
        }
    }
    return 0;
}