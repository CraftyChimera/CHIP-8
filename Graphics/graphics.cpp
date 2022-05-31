#include "graphics.hpp"

bool UpdatePixel(sf::Texture &texture, byte x, byte y, byte n, vector<byte> data)
{
    sf::Image image = texture.copyToImage();
    bool res = false;
    sf::Color w = sf::Color::White, b = sf::Color::Black;
    byte v_x = x & (0x40 - 1), v_y = y & (0x20 - 1);
    for (int i = 0; i < n; i++)
    {
        int step = 0, mark = 1 << 8;
        while (mark > 0)
        {
            if ((mark & data[i]) > 0)
            {
                if (image.getPixel((v_x + step), v_y) == w)
                {
                    image.setPixel((v_x + step), v_y, b);
                    res = true;
                }

                else
                    image.setPixel((v_x + step), v_y, w);
            }
            step++;
            if (v_x + step > 0x40)
                break;

            mark = mark >> 1;
        }
        v_y++;
        if (v_y > 0x20)
            break;
    }
    texture.update(image);
    return res;
};

void DrawWindow(sf::RenderWindow &window, sf::Texture texture)
{
    sf::Sprite sprite;
    sprite.setTexture(texture);
    window.draw(sprite);
    window.display();
};
