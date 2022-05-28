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
    delay = 0x0;
    sound = 0x0;
    V.fill(0);
    for (int i = 0; i <= 0x40; i++)
        ram[0x50 + i] = font_data[i];
}
void CPU::Load_Data(vector<byte> data)
{
    for (size_t i = 0; i < data.size(); i++)
        ram[pc + i] = data[i];
}

void CPU::Clear_Screen(sf::RenderWindow &window)
{
    window.clear(sf::Color::Black);
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
    if (I + V[reg] >= 0x1000)
        V[0xF] = 1;
    I = (I + V[0xF]) & 0xFFF;
}

void CPU::Random(word a)
{
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<word> uni(0, 0x1000);
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
    UpdatePixel(texture, x, y, n, data);
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

void CPU::Skip(word a) // To be Done
{
    byte value = a & 0xFF;
    a >>= 4;
    byte regY = a & 0xF;
    a >>= 4;
    byte regX = a & 0xF;
    a >>= 4;
    byte first = a;
    if (first == 3)
        pc += ((V[regX] == value) * 2);
    if (first == 4)
        pc += ((V[regX] != value) * 2);
    if (first == 5)
        pc += ((V[regX] == V[regY]) * 2);
    if (first == 9)
        pc += ((V[regX] != V[regY]) * 2);
}

void CPU::Arithmetic(word a)
{
    byte op = a & 0xF;
    a >>= 4;
    byte regY = a & 0xF;
    a >>= 4;
    byte regX = a & 0xF;
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
        V[0xF] = 0;
        V[regX] <<= 1;
    }
}

void CPU::Skip_Key(word a, byte b)
{
    byte op = a & 0xF;
    byte regX = extractX(a);
    if (op == 0xe)
        pc += ((V[regX] == b) * 2);
    if (op == 0x1)
        pc += ((V[regX] != b) * 2);
}

void CPU::Timer(word a)
{
    byte op = a & 0xF;
    byte regX = extractX(a);
    if (op == 7)
        V[regX] = delay;
    if (op == 5)
        delay = V[regX];
    if (op == 8)
        sound = V[regX];
}

void CPU::Store_and_Load(word a) // problematic
{
    byte op = (a >> 4) & 0xF;
    byte reg = extractX(a);
    if (op == 5)
    {
        for (byte i = 0; i <= reg; i++)
            ram[I + i] = V[i];
    }
    if (op == 6)
    {
        for (byte i = 0; i <= reg; i++)
            V[i] = ram[I + i];
    }
}

void CPU::BCD(word a)
{
    byte reg = extractX(a);
    word digit = 0, value = V[reg], x = value;
    while (x > 0)
    {
        x /= 10;
        digit++;
    }
    if (value == 0)
        digit = 1;
    for (int i = digit - 1; i >= 0; i--)
    {
        ram[I + i] = value % 10;
        value /= 10;
    }
}

void CPU::Font(word a)
{
    byte reg = extractX(a);
    byte ch = (V[reg]) & 0xF;
    I = 0x40 + (ch * 5);
}