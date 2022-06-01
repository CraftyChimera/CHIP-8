#include "../Base/base.hpp"
#include "../Graphics/graphics.hpp"

struct CPU
{
    array<byte, 0x1000> ram;
    word pc, I;
    stack<word> Stack;
    int delay, sound;
    array<byte, 0x10> V;
    vector<word> instruction_list;
    CPU();

    void Get_Key(word, byte);
    void Dec_Timer();
    void Load_Data(vector<byte>);
    void Clear_Screen(sf::RenderWindow &, sf::Texture &);
    void Jump(word);
    void Jump_Offset(word);
    void Set_Register(word);
    void Add_Register(word);
    void Set_Index(word);
    void Add_Index(word);
    void Draw(word, sf::RenderWindow &, sf::Texture &);
    void Sub(word);
    void Skip(word);
    void Skip_Key(word, byte);
    void Arithmetic(word);
    void Random(word);
    void Timer(word);
    void Store_and_Load(word);
    void BCD(word);
    void Font(word);
    void Debug();
};