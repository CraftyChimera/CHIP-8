#include "Cpu/cpu.hpp"

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cout << "Usage './Chip-8 arg1' \n";
        return 0;
    }
    std::string path = "roms/" + std::string(argv[1]);
    vector<byte> data = read_file(path);

    CPU cpu;
    cpu.Load_Data(data);
    cpu.Run(600.0);
    return 0;
}