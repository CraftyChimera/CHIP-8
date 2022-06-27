#include "base.hpp"
#include <fstream>
using std::string;

vector<byte> read_file(std::string path)
{
    constexpr auto read_size = std::size_t(4096);
    auto stream = std::ifstream(path);
    stream.exceptions(std::ios_base::badbit);
    if (!stream.good())
    {
        std::cout << "Error while loading rom\n Check if rom exists\n";
        exit(-1);
    }
    vector<byte> out;
    auto buf = string(read_size, '\0');
    while (stream.read(&buf[0], read_size))
    {
        string temp(buf, 0, stream.gcount());
        for (auto x : temp)
            out.push_back(x);
    }
    string temp(buf, 0, stream.gcount());
    for (auto x : temp)
        out.push_back(x);
    return out;
}

byte getKeyPressed(sf::Uint32 x, bool &pressed)
{
    switch (x)
    {
    case '1':
        return 0x1;
    case '2':
        return 0x2;
    case '3':
        return 0x3;
    case '4':
        return 0xC;
    case 'q':
    case 'Q':
        return 0x4;
    case 'w':
    case 'W':
        return 0x5;
    case 'e':
    case 'E':
        return 0x6;
    case 'r':
    case 'R':
        return 0xD;
    case 'a':
    case 'A':
        return 0x7;
    case 's':
    case 'S':
        return 0x8;
    case 'd':
    case 'D':
        return 0x9;
    case 'f':
    case 'F':
        return 0xE;
    case 'z':
    case 'Z':
        return 0xA;
    case 'x':
    case 'X':
        return 0x0;
    case 'c':
    case 'C':
        return 0xB;
    case 'v':
    case 'V':
        return 0xF;
    default:
    {
        pressed = false;
        return -1;
    }
    }
}
