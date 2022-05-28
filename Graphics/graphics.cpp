#include "graphics.hpp"

void UpdatePixel(sf::Texture &texture, byte x, byte y, byte n, vector<byte> data)
{
    sf::Image image = texture.copyToImage();
    sf::Color w = sf::Color::White, b = sf::Color::Black;
    byte v_x = x & (0x40 - 1), v_y = y & (0x20 - 1);
    for (int i = 0; i < n; i++)
    {
        int step = 0, mark = 1 << 8;
        while (mark > 0)
        {
            if (v_x + step >= 0x40)
                break;
            if ((mark & data[i]) > 0)
            {
                if (image.getPixel(v_x + step, v_y) == w)
                    image.setPixel(v_x + step, v_y, b);

                else
                    image.setPixel(v_x + step, v_y, w);
            }
            step++;
            mark = mark >> 1;
        }
        v_y++;
        if (v_y >= 0x20)
            break;
    }
    texture.update(image);
};

void DrawWindow(sf::RenderWindow &window, sf::Texture texture)
{
    sf::Sprite sprite;
    sprite.setTexture(texture);
    window.draw(sprite);
    window.display();
};
/*
int main()
{
    int flag = 0;
    sf::RenderWindow window(sf::VideoMode(64, 32), "hello");
    sf::Texture texture;
    if (!texture.loadFromFile("assets/in.png", sf::IntRect(10, 100, 1000, 1000)))
    {
        std::cout << "error";
    };
    window.setPosition(sf::Vector2i(200, 300));
    byte a = 0, y = 3, n = 0xF;
    while (window.isOpen())
    {
        window.clear(sf::Color::Black);
        DrawWindow(window, texture);
        sf::Event event;
        while (window.pollEvent(event))
        {

            switch (event.type)
            {
            case sf::Event::Closed:
            {
                window.close();
                break;
            }
            case sf::Event::TextEntered:
            {
                if (flag == 1)
                    break;
                flag = 1;
                byte x = static_cast<byte>(event.text.unicode);
                if (('0' <= x) && (x <= '9'))
                    x -= '0';
                else if (('A' <= x) && (x <= 'F'))
                    x = 10 + (x - 'A');
                else if (('a' <= x) && (x <= 'f'))
                    x = 10 + (x - 'a');
                else
                    break;
                UpdatePixel(texture, a, y, n, vector<byte>{0xff, 0x00, 0xff, 0x00, 0x3c, 0x00, 0x3c, 0x00, 0x3c, 0x00, 0x3c, 0x00, 0xff, 0x00, 0xff});
                UpdatePixel(texture, a + 16, y, n, vector<byte>{0xff, 0x00, 0xff, 0x00, 0x38, 0x00, 0x3f, 0x00, 0x3f, 0x00, 0x38, 0x00, 0xff, 0x00, 0xff});
                UpdatePixel(texture, a + 24, y, n, vector<byte>{0x80, 0x00, 0xe0, 0x00, 0xe0, 0x00, 0x80, 0x00, 0x80, 0x00, 0xe0, 0x00, 0xe0, 0x00, 0x80});
                UpdatePixel(texture, a + 32, y, n, vector<byte>{0xf8, 0x0, 0xfc, 0x0, 0x3e, 0x0, 0x3f, 0x0, 0x3b, 0x0, 0x39, 0x0, 0xf8, 0x0, 0xf8});
                UpdatePixel(texture, a + 40, y, n, vector<byte>{0x3, 0, 0x7, 0, 0xf, 0, 0xbf, 0, 0xfb, 0, 0xf3, 0, 0xe3, 0, 0x43});
                UpdatePixel(texture, a + 48, y, n, vector<byte>{0xe0, 0, 0xe0, 0, 0x80, 0, 0x80, 0, 0x80, 0, 0x80, 0, 0xe0, 0, 0xe0});
                break;
            }
            default:
                break;
            }
        }
    }
    return 0;
}
*/