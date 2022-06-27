#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <array>
#include <vector>
#include <stack>
typedef uint8_t byte;
typedef uint16_t word;
using std::vector;

vector<byte> read_file(std::string);
byte getKeyPressed(sf::Uint32, bool &);