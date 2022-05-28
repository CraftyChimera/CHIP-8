#include "cpu.hpp"

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
    pc = a & ((1 << 12) - 1);
}

void CPU::Set_Register(word a)
{
    byte value = a & ((1 << 8) - 1);
    a >>= 8;
    byte reg = a & ((1 << 4) - 1);
    V[reg] = value;
}

void CPU::Add_Register(word a)
{
    byte value = a & ((1 << 8) - 1);
    a >>= 8;
    byte reg = a & ((1 << 4) - 1);
    word x = V[reg] + value;
    V[reg] = x & ((1 << 8) - 1);
}

void CPU::Set_Index(word a)
{
    I = a & ((1 << 12) - 1);
}

void CPU::Draw(word a, sf::RenderWindow &window, sf::Texture &texture)
{
    byte n, x, y, x_reg, y_reg;
    n = a & ((1 << 4) - 1);
    a >>= 4;
    y_reg = a & ((1 << 4) - 1);
    a >>= 4;
    x_reg = a & ((1 << 4) - 1);
    y = V[y_reg];
    x = V[x_reg];
    vector<byte> data(ram.begin() + I, ram.begin() + I + n);
    UpdatePixel(texture, x, y, n, data);
    DrawWindow(window, texture);
}