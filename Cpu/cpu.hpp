#include "../Base/base.hpp"
#include "../Graphics/graphics.hpp"

struct CPU
{
    array<byte, 0x1000> ram;
    word pc, I;
    stack<word> Stack;
    byte delay, sound;
    array<byte, 0x10> V;
    CPU();
    void Load_Data(vector<byte>);
    void Clear_Screen(sf::RenderWindow &);
    void Jump(word);
    void Set_Register(word);
    void Add_Register(word);
    void Set_Index(word);
    void Draw(word, sf::RenderWindow &, sf::Texture &);
};