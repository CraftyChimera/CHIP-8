#include "cpu.hpp"
#include <random>

constexpr byte extractX(word a)
{
    return (a >> 8) & (0xF);
}

CPU::CPU()
{
    vector<byte> font_data = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80} // F
    ;
    ram.fill(0);
    pc = 0x200;
    I = 0x0;
    delay_t = 0x0;
    sound_t = 0x0;
    V.fill(0);
    for (int i = 0; i <= 0x40; i++)
        ram[0x50 + i] = font_data[i];
}

void CPU::Dec_Timer()
{
    if (delay_t > 0)
        delay_t--;
    if (sound_t > 0)
        sound_t--;
}

void CPU::Get_Key(word a, byte b)
{
    byte regX = extractX(a);
    V[regX] = b;
}

void CPU::Debug()
{
    word x = (ram[pc] << 8) + ram[pc + 1];
    instruction_list.push_back(x);
    if (instruction_list.size() > 8)
        instruction_list = vector<word>(instruction_list.begin() + 1, instruction_list.end());
    std::cout << "Instructions\t";
    for (auto y : instruction_list)
        std::cout << y << " ";
    std::cout << "\n";
    for (int t = 0; t < 4; t++)
    {
        for (int i = 4 * t; i < 4 * (t + 1); i++)
            std::cout << std::hex << "V" << i << ": " << (int)V[i] << "\t";
        std::cout << "\n";
    }
    std::cout << "I: " << I << "\n";
    std::cout << "Delay_t: " << (int)delay_t << "\n";
    std::cout << "Sound: " << (int)sound_t << "\n";
    std::cout << "\n\n\n\n\n";
}

void CPU::Load_Data(vector<byte> data)
{
    for (size_t i = 0; i < data.size(); i++)
        ram[pc + i] = data[i];
}

void CPU::Clear_Screen(sf::RenderWindow &window, sf::Texture &texture)
{
    sf::Image blank_screen;
    blank_screen.create(65, 32);
    texture.update(blank_screen);
    DrawWindow(window, texture);
}

void CPU::Jump(word a)
{
    pc = a & 0xFFF;
}

void CPU::Jump_Offset(word a)
{
    pc = (a & 0xFFF) + V[0];
}

void CPU::Set_Register(word a)
{
    byte value = a & 0xFF;
    byte reg = extractX(a);
    V[reg] = value;
}

void CPU::Add_Register(word a)
{
    byte value = a & 0xFF;
    byte reg = extractX(a);
    word x = V[reg] + value;
    V[reg] = x & 0xFF;
}

void CPU::Set_Index(word a)
{
    I = a & 0xFFF;
}

void CPU::Add_Index(word a)
{
    byte reg = extractX(a);
    V[0xF] = 0;
    if (I + V[reg] >= 0x1000)
        V[0xF] = 1;
    I = (I + V[reg]) & 0xFFF;
}

void CPU::Random(word a)
{
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<word> uni(0, 0x100);
    word random_integer = uni(rng);
    byte value = (a & 0xFF), reg = extractX(a);
    V[reg] = value & random_integer;
}

void CPU::Draw(word a, sf::RenderWindow &window, sf::Texture &texture)
{
    byte n, x, y, regX, regY;
    n = a & 0xF;
    a >>= 4;
    regY = a & 0xF;
    a >>= 4;
    regX = a & 0xF;
    y = V[regY];
    x = V[regX];
    vector<byte> data(ram.begin() + I, ram.begin() + I + n);
    V[0xF] = UpdatePixel(texture, x, y, n, data);
    DrawWindow(window, texture);
}

void CPU::Sub(word a)
{
    if (a == 0x00ee)
    {
        pc = Stack.top();
        Stack.pop();
        return;
    }
    Stack.push(pc);
    Jump(a);
}

void CPU::Skip(word a)
{
    byte first = a >> 12;
    byte regX = extractX(a);

    if (first == 3 || first == 4)
    {
        byte value = a & 0xFF;
        if (first == 3)
            pc += ((V[regX] == value) * 2);
        if (first == 4)
            pc += ((V[regX] != value) * 2);
        return;
    }

    if ((a & 0xF) == 0)
    {
        byte regY = (a >> 4) & 0xF;
        if (first == 5)
            pc += ((V[regX] == V[regY]) * 2);
        if (first == 9)
            pc += ((V[regX] != V[regY]) * 2);
    }
}

void CPU::Arithmetic(word a)
{
    byte op = a & 0xF;
    byte regY = (a >> 4) & 0xF;
    byte regX = extractX(a);
    if (op == 0)
        V[regX] = V[regY];
    if (op == 1)
        V[regX] = V[regX] | V[regY];
    if (op == 2)
        V[regX] = V[regX] & V[regY];
    if (op == 3)
        V[regX] = V[regX] ^ V[regY];
    if (op == 4)
    {
        word sum = V[regX] + V[regY];
        if (sum >= 0x100)
            V[0xF] = 1;
        else
            V[0xF] = 0;
        V[regX] = sum & 0xFF;
    }
    if (op == 5)
    {
        V[0xF] = 1;
        if (V[regX] < V[regY])
            V[0xF] = 0;
        V[regX] = V[regX] - V[regY];
    }
    if (op == 6) // problematic
    {
        V[0xF] = V[regX] & 1;
        V[regX] >>= 1;
    }
    if (op == 7)
    {
        V[0xF] = 1;
        if (V[regY] < V[regX])
            V[0xF] = 0;
        V[regX] = V[regY] - V[regX];
    }
    if (op == 0xe) // problematic
    {
        byte MSB = V[regX] >> 7;
        V[0xF] = MSB;
        V[regX] <<= 1;
    }
}

void CPU::Skip_Key(word a, byte b)
{
    byte op = a & 0xFF;
    byte regX = extractX(a);
    if (op == 0x9e)
        pc += ((V[regX] == b) * 2);
    if (op == 0xa1)
        pc += ((V[regX] != b) * 2);
}

void CPU::Timer(word a)
{
    byte op = a & 0xFF;
    byte regX = extractX(a);
    if (op == 0x07)
        V[regX] = delay_t;
    if (op == 0x15)
        delay_t = V[regX];
    if (op == 0x18)
        sound_t = V[regX];
}

void CPU::Store_and_Load(word a) // problematic
{
    byte op = a & 0xFF;
    byte reg = extractX(a);
    if (op == 0x55)
    {
        for (byte i = 0; i <= reg; i++)
            ram[I + i] = V[i];
    }
    if (op == 0x65)
    {
        for (byte i = 0; i <= reg; i++)
            V[i] = ram[I + i];
    }
}

void CPU::BCD(word a)
{
    byte reg = extractX(a);
    word value = V[reg];
    for (int i = 2; i >= 0; i--)
    {
        ram[I + i] = value % 10;
        value /= 10;
    }
}

void CPU::Font(word a)
{
    byte reg = extractX(a);
    byte ch = (V[reg]) & 0xF;
    I = 0x50 + (ch * 5);
}

void CPU::Run(double frequency = 540.0)
{
    sf::RenderWindow window(sf::VideoMode(65, 32), "CHIP-8");
    sf::Texture texture;

    if (!texture.loadFromFile("assets/in.png"))
    {
        std::cout << "error Loading the file\n";
        return;
    };
    sf::SoundBuffer buffer;

    if (!buffer.loadFromFile("assets/beep.wav"))
    {
        std::cout << "error Loading the file\n";
        return;
    }

    sf::Sound sound;
    sound.setBuffer(buffer);
    window.setPosition(sf::Vector2i(200, 300));

    while (window.isOpen())
    {
        window.clear(sf::Color::Black);
        sf::Event event;
        int cycle_count = 0;
        bool pressed = false, open = true;
        byte read_byte = 0;
        sf::Clock clock;
        while (open)
        {
            DrawWindow(window, texture);
            clock.restart();
            while (window.pollEvent(event))
            {
                pressed = false;
                switch (event.type)
                {
                case sf::Event::TextEntered:
                {
                    pressed = true;
                    read_byte = getKeyPressed(event.text.unicode, pressed);
                    break;
                }
                case sf::Event::Closed:
                {
                    open = false;
                    window.close();
                    break;
                }
                default:
                    break;
                }
            }

            word instruction = 0x0000;
            // Debug();
            word part1 = ram[pc] << 8, part2 = ram[pc + 1];
            instruction = part1 + part2;
            pc += 2;
            Decode_and_Execute(instruction, window, texture, read_byte, pressed);
            if (cycle_count == 0)
            {
                Dec_Timer();
                if (sound_t > 0)
                {
                    sound.play();
                }
            }

            cycle_count = (cycle_count + 1) % 9; // timers decrement at 60 hz while the main CPU runs at approximately 540 hz
            sf::Time need = sf::seconds(1 / frequency) - clock.getElapsedTime();
            sleep(need);
        }
    }
}

void CPU::Decode_and_Execute(word instruction, sf::RenderWindow &window, sf::Texture &texture, byte read_byte, bool pressed)
{
    byte first = (instruction >> 12);

    switch (first)
    {
    case 0x0:
    {
        if (instruction == 0x00e0)
            Clear_Screen(window, texture);
        if (instruction == 0x00ee)
            Sub(instruction);
        return;
    }

    case 0x1:
    {
        Jump(instruction);
        return;
    }

    case 0x2:
    {
        Sub(instruction);
        return;
    }

    case 0x3:
    case 0x4:
    case 0x5:
    case 0x9:
    {
        Skip(instruction);
        return;
    }

    case 0x6:
    {
        Set_Register(instruction);
        return;
    }

    case 0x7:
    {
        Add_Register(instruction);
        return;
    }

    case 0x8:
    {
        Arithmetic(instruction);
        return;
    }

    case 0xA:
    {
        Set_Index(instruction);
        return;
    }

    case 0xB:
    {
        Jump_Offset(instruction);
        return;
    }

    case 0xC:
    {
        Random(instruction);
        return;
    }

    case 0xD:
    {
        Draw(instruction, window, texture);
        return;
    }

    case 0xE:
    {
        byte op = instruction & 0xFF;
        if (op == 0x9e)
        {
            if (pressed)
                Skip_Key(instruction, read_byte);
        }
        if (op == 0xa1)
        {
            if (pressed)
                Skip_Key(instruction, read_byte);
            else
                pc += 2;
        }
        return;
    }

    case 0xF:
    {
        byte value = instruction & 0xFF;
        switch (value)
        {

        case 0x0a:
        {
            if (!pressed)
                pc -= 2;
            else
            {
                Get_Key(instruction, read_byte);
            }
            return;
        }

        case 0x1e:
        {
            Add_Index(instruction);
            return;
        }

        case 0x07:
        case 0x15:
        case 0x18:
        {
            Timer(instruction);
            return;
        }

        case 0x29:
        {
            Font(instruction);
            return;
        }

        case 0x33:
        {
            BCD(instruction);
            return;
        }

        case 0x55:
        case 0x65:
        {
            Store_and_Load(instruction);
            return;
        }

        default:
            return;
        }
    }
    }
}
