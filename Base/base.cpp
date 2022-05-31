#include "base.hpp"
#include <fstream>
using std::string;

vector<byte> read_file(std::string path)
{
    constexpr auto read_size = std::size_t(4096);
    auto stream = std::ifstream(path);
    stream.exceptions(std::ios_base::badbit);

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