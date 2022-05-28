#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <array>
#include <vector>
#include <stack>
typedef unsigned char byte;
typedef unsigned short word;
using std::array;
using std::stack;
using std::vector;

vector<byte> read_file(std::string_view);