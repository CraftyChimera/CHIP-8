#include "Cpu/cpu.hpp"

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cout << "Usage './main arg1' \n";
        return 0;
    }
    std::string path = "tests/", name(argv[1]);
    path += name;
    CPU cpu;
    vector<byte> data = read_file(path);
    byte read_byte = 0;
    bool open = true;
    cpu.Load_Data(data);
    sf::RenderWindow window(sf::VideoMode(65, 32), "CHIP-8");
    sf::Texture texture;
    if (!texture.loadFromFile("assets/in.png"))
    {
        std::cout << "error Loading the file\n";
        return -1;
    };
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile("assets/beep.wav"))
    {
        std::cout << "error Loading the file\n";
        return -1;
    }
    sf::Sound sound;
    sound.setBuffer(buffer);
    window.setPosition(sf::Vector2i(200, 300));
    while (window.isOpen())
    {
        window.clear(sf::Color::Black);
        sf::Event event;
        int speed = 2;
        int cycle_count = 0;
        bool pressed = false;
        sf::Clock clock;
        while (open)
        {
            DrawWindow(window, texture);
            clock.restart();
            while (window.pollEvent(event))
            {

                switch (event.type)
                {
                case sf::Event::TextEntered:
                {
                    pressed = true;
                    switch (event.text.unicode)
                    {
                    case '1':
                    {
                        read_byte = 0x1;
                        break;
                    }
                    case '2':
                    {
                        read_byte = 0x2;
                        break;
                    }
                    case '3':
                    {
                        read_byte = 0x3;
                        break;
                    }
                    case '4':
                    {
                        read_byte = 0xC;
                        break;
                    }
                    case 'q':
                    case 'Q':
                    {
                        read_byte = 0x4;
                        break;
                    }
                    case 'w':
                    case 'W':
                    {
                        read_byte = 0x5;
                        break;
                    }
                    case 'e':
                    case 'E':
                    {
                        read_byte = 0x6;
                        break;
                    }
                    case 'r':
                    case 'R':
                    {
                        read_byte = 0xD;
                        break;
                    }
                    case 'a':
                    case 'A':
                    {
                        read_byte = 0x7;
                        break;
                    }
                    case 's':
                    case 'S':
                    {
                        read_byte = 0x8;
                        break;
                    }
                    case 'd':
                    case 'D':
                    {
                        read_byte = 0x9;
                        break;
                    }
                    case 'f':
                    case 'F':
                    {
                        read_byte = 0xE;
                        break;
                    }
                    case 'z':
                    case 'Z':
                    {
                        read_byte = 0xA;
                        break;
                    }
                    case 'x':
                    case 'X':
                    {
                        read_byte = 0x0;
                        break;
                    }
                    case 'c':
                    case 'C':
                    {
                        read_byte = 0xB;
                        break;
                    }
                    case 'v':
                    case 'V':
                    {
                        read_byte = 0xF;
                        break;
                    }
                    default:
                    {
                        pressed = false;
                    }
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
            // cpu.Debug();
            word part1 = cpu.ram[cpu.pc] << 8,
                 part2 = cpu.ram[cpu.pc + 1];
            instruction = part1 + part2;
            cpu.pc += 2;
            if (cycle_count == 0)
            {
                cpu.Dec_Timer();
                if (cpu.sound > 0)
                {
                    sound.play();
                }
            }
            cycle_count = (cycle_count + 1) % 9; // timers decrement at 60 hz while the main CPU runs at approximately 540 hz
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