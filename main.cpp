#include "Cpu/cpu.hpp"

int main()
{
    std::string_view s = "tests/tetris.rom";
    CPU cpu;
    vector<byte> data = read_file(s);
    byte read_byte = 0;
    bool open = true;
    cpu.Load_Data(data);
    sf::RenderWindow window(sf::VideoMode(64, 32), "CHIP-8");
    sf::Texture texture;
    if (!texture.loadFromFile("assets/in.png"))
    {
        std::cout << "error Loading the file\n";
        return 0;
    };
    DrawWindow(window, texture);
    window.setPosition(sf::Vector2i(200, 300));
    while (window.isOpen())
    {
        window.clear(sf::Color::Black);
        sf::Event event;
        int speed = 4000;
        int fake = 0;
        bool pressed = false;
        sf::Clock clock;
        while (open)
        {
            clock.restart();
            while (window.pollEvent(event))
            {

                switch (event.type)
                {
                case sf::Event::TextEntered:
                {
                    pressed = true;
                    if (event.text.unicode >= '0' && event.text.unicode <= '9')
                        read_byte = event.text.unicode - '0';
                    else if (event.text.unicode >= 'A' && event.text.unicode <= 'F')
                        read_byte = event.text.unicode - 'A' + 10;
                    else if (event.text.unicode >= 'a' && event.text.unicode <= 'f')
                        read_byte = event.text.unicode - 'a' + 10;
                    else
                    {
                        if (event.text.unicode == 'g')
                        {
                            if (speed == 2)
                                speed = 500;
                            else
                                speed = 2;
                        }
                        pressed = false;
                    }
                    break;
                }
                case sf::Event::Closed:
                {
                    open = false;
                    window.close();
                    break;
                }
                default:
                    pressed = false;
                    break;
                }
            }
            word instruction = 0x0000;
            cpu.Debug();
            word part1 = cpu.ram[cpu.pc] << 8,
                 part2 = cpu.ram[cpu.pc + 1];
            instruction = part1 + part2;
            cpu.pc += 2;
            if (fake == 0)
            {
                cpu.Dec_Timer();
                DrawWindow(window, texture);
            }
            fake = (fake + 1) % 9;
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
            case 0x9:
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
                byte op = instruction & 0xFF;
                if (op == 0x9e)
                {
                    if (pressed)
                        cpu.Skip_Key(instruction, read_byte);
                }
                if (op == 0xa1)
                {
                    if (pressed)
                        cpu.Skip_Key(instruction, read_byte);
                    else
                        cpu.pc += 2;
                }
                break;
            }

            case 0xF:
            {
                byte value = instruction & 0xFF;
                switch (value)
                {
                case 0x0a:
                {
                    if (!pressed)
                        cpu.pc -= 2;
                    else
                    {
                        cpu.Get_Key(instruction, read_byte);
                    }
                    break;
                }
                case 0x1e:
                {
                    cpu.Add_Index(instruction);
                    break;
                }
                case 0x07:
                case 0x15:
                case 0x18:
                {
                    cpu.Timer(instruction);
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
            sf::Time need = sf::milliseconds(speed) - clock.getElapsedTime();
            sf::Clock clock2;
            while (clock2.getElapsedTime() < need)
                ;
        }
    }
    return 0;
}