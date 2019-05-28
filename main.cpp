#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode(600, 200), "SFML works!", sf::Style::Fullscreen);
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    while (window.isOpen())
    {

        window.clear(sf::Color(150, 150, 150));
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
            shape.setPosition((float)mouse_pos.x, (float)mouse_pos.y);
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
            window.close();
        sf::Texture texture;
        texture.loadFromFile("texture.png");
        sf::Sprite sprite;
        sprite.setTexture(texture);
        int edge = 52;
        sprite.setTextureRect(sf::IntRect(10, 10, edge, edge));
        for(int i = 0; i < 20; i++)
            for(int j = 0; j < 20; j++)
            {
                sprite.setPosition(400+i*(edge+1), j*(edge+1));
                sprite.setColor(sf::Color(255, 255, 255, 200));
                if(j == 1 && i > 1) sprite.setColor(sf::Color(255, 255, 0, 200));
                if(j == 1 && i == 19) sprite.setColor(sf::Color(0, 0, 255, 200));
                if(j > 1 && j < 18 && i == 2) sprite.setColor(sf::Color(255, 255, 0, 200));
                if(j == 5 && i > 1 && i < 18) sprite.setColor(sf::Color(255, 255, 0, 200));
                if(j > 4 && j < 19 && i == 17) sprite.setColor(sf::Color(255, 255, 0, 200));
                if(j > 4 && j < 18 && i == 13) sprite.setColor(sf::Color(255, 255, 0, 200));
                if(j > 4 && j < 12 && i == 9) sprite.setColor(sf::Color(255, 255, 0, 200));
                if(j > 12 && j < 17 && i == 6) sprite.setColor(sf::Color(255, 255, 0, 200));
                if(j == 18 && 8 < i && i < 18) sprite.setColor(sf::Color(255, 255, 0, 200));
                if(j == 17 && 1 < i && i < 10) sprite.setColor(sf::Color(255, 255, 0, 200));
                if(j == 12 && 5 < i && i < 10) sprite.setColor(sf::Color(255, 255, 0, 200));
                if(j == 19 && i == 9) sprite.setColor(sf::Color(255, 0, 0, 200));
                if(j == 3 && 3 < i && i < 19) sprite.setColor(sf::Color(0, 255, 0, 200));
                if(j == 15 && 7 < i && i < 12) sprite.setColor(sf::Color(0, 255, 0, 200));
                if(j == 14 && 7 < i && i < 12) sprite.setColor(sf::Color(0, 255, 0, 200));
                if(6 < j && j < 17 && i == 15) sprite.setColor(sf::Color(0, 255, 0, 200));
                if(6 < j && j < 14 && i == 11) sprite.setColor(sf::Color(0, 255, 0, 200));
                if(6 < j && j < 16 && i == 4) sprite.setColor(sf::Color(0, 255, 0, 200));
                if(6 < j && j < 11 && i == 5) sprite.setColor(sf::Color(0, 255, 0, 200));
                if(6 < j && j < 11 && i == 6) sprite.setColor(sf::Color(0, 255, 0, 200));
                if(6 < j && j < 11 && i == 7) sprite.setColor(sf::Color(0, 255, 0, 200));
                // Draw it
                window.draw(sprite);
            }

        window.draw(shape);
        window.display();
    }

    return 0;
}
