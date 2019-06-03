#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

#define abs(x) ((x)>0?(x):-(x))
#define sqr(x) (x)*(x)

const int edge = 52;

int start_x;
int start_y;
int end_x;
int end_y;
std::vector<std::vector<int>> map(20, std::vector<int>(20, 0));

int dist(int x, int y)
{
    return abs(end_x-x) + abs(end_y-y);
}

class enemy
{
    int next_x;
    int next_y;
    int step;
    std::vector<std::vector<bool>> vis;
public:
    int x;
    int y;
    int xp;
    enemy():
        step(1),
        xp(100),
        next_x(19),
        next_y(1),
        x(19*edge),
        y(edge),
        vis(20, std::vector<bool>(20, 0))
    {};
    void move()
    {
        if(x == edge*next_x && y == edge*next_y)
        {
            vis[next_x][next_y] = 1;
            int distance = 100;
            int move_x = next_x;
            int move_y = next_y;
            if(next_x > 0 && map[next_x-1][next_y] == 1 && dist(next_x-1, next_y) < distance && !vis[next_x-1][next_y]) distance = dist(next_x-1, next_y), move_y = next_y, move_x = next_x-1;
            if(next_x < 19 && map[next_x+1][next_y] == 1 && dist(next_x+1, next_y) < distance && !vis[next_x+1][next_y]) distance = dist(next_x+1, next_y), move_y = next_y, move_x = next_x+1;
            if(next_y > 0 && map[next_x][next_y-1] == 1 && dist(next_x, next_y-1) < distance && !vis[next_x][next_y-1]) distance = dist(next_x, next_y-1), move_x = next_x, move_y = next_y-1;
            if(next_y < 19 && map[next_x][next_y+1] == 1 && dist(next_x, next_y+1) < distance && !vis[next_x][next_y+1]) distance = dist(next_x, next_y+1), move_x = next_x, move_y = next_y+1;
            next_x = move_x;
            next_y = move_y;
        }
        if(edge*next_x < x) x -= step;
        if(edge*next_x > x) x += step;
        if(edge*next_y < y) y -= step;
        if(edge*next_y > y) y += step;
        //std::cout << edge*next_x << ' ' << edge*next_y << ' ' << x << ' ' << y << '\n';
    };
    void draw(sf::RenderWindow &window)
    {
        sf::Texture texture;
        texture.loadFromFile("texture.png");
        sf::Sprite sprite;
        sprite.setTexture(texture);
        sprite.setTextureRect(sf::IntRect(10, 10, edge-1, edge-1));
        sprite.setPosition(x, y);
        sprite.setColor(sf::Color(255, 0, 255, 200));
        window.draw(sprite);
    }
};

class tower
{
    int x;
    int rearm;
    int y;
    bool is_active;
public:
    tower(int cur_x, int cur_y):
        x(cur_x),
        y(cur_y),
        rearm(0),
        is_active(1)
    {};
    bool is_pos(int ax, int ay)
    {
        return ax == x && ay == y;
    }
    void turn_off()
    {
        is_active = 0;
    }
    void turn_on()
    {
        is_active = 1;
    }
    void draw(sf::RenderWindow &window)
    {
        if(is_active)
        {
            sf::CircleShape shape(150.f);
            shape.setFillColor(sf::Color(0, 255, 255, 50));
            shape.setPosition(x*edge-125, y*edge-125);
            window.draw(shape);
        }
        sf::Texture texture;
        texture.loadFromFile("texture.png");
        sf::Sprite sprite;
        sprite.setTexture(texture);
        sprite.setTextureRect(sf::IntRect(10, 10, edge-1, edge-1));
        sprite.setPosition(x*edge, y*edge);
        sprite.setColor(sf::Color(0, 0, 0, 200));
        window.draw(sprite);
    };
    void reload(sf::RenderWindow &window, std::vector<enemy> &enemies)
    {
        for(auto i = enemies.begin(); i != enemies.end(); i++)
            if(sqr(x*edge-i->x) + sqr(y*edge-i->y) <= 22500)
            {
                sf::VertexArray line(sf::LinesStrip, 2);
                line[0].position = sf::Vector2f(i->x+edge/2, i->y+edge/2);
                line[0].color  = sf::Color::Red;
                line[1].position = sf::Vector2f(x*edge+edge/2, y*edge+edge/2);
                line[1].color = sf::Color::Red;
                window.draw(line);
                if(!rearm--)
                    i->xp--,
                    rearm = 20;
                //std::cout << i->xp << ' ' << rearm << '\n';
                //std::cin.get();
                if(i->xp <= 0)
                    enemies.erase(i);
                return;
            }
    };
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(600, 200), "SFML works!", sf::Style::Fullscreen);
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);
    int cnt = rand()%100+100;
    std::vector<enemy> enemies;
    std::vector<tower*> towers;
    tower* active = nullptr;
    while (window.isOpen())
    {
        if(!--cnt)
        {
            enemies.push_back(enemy());
            cnt = rand()%100+100;
        }
        window.clear(sf::Color(150, 150, 150));
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            int x = sf::Mouse::getPosition(window).x/edge;
            int y = sf::Mouse::getPosition(window).y/edge;
            if(map[x][y] != 2) goto out;
            if(active != nullptr)
                active->turn_off();
            towers.push_back(new tower(x, y));
            active = towers.back();
            //sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
            //shape.setPosition((float)mouse_pos.x, (float)mouse_pos.y);
        }
        out:;

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
            window.close();
        sf::Texture texture;
        texture.loadFromFile("texture.png");
        sf::Sprite sprite;
        sprite.setTexture(texture);
        sprite.setTextureRect(sf::IntRect(10, 10, edge-1, edge-1));
        for(int i = 0; i < 20; i++)
            for(int j = 0; j < 20; j++)
            {
                sprite.setPosition(i*(edge), j*(edge));
                sprite.setColor(sf::Color(255, 255, 255, 200));
                if(j == 1 && i > 1) sprite.setColor(sf::Color(255, 255, 0, 200)), map[i][j] = 1;
                if(j == 1 && i == 19) sprite.setColor(sf::Color(0, 0, 255, 200)), start_x = i, start_y = j, map[i][j] = 1;
                if(j > 1 && j < 18 && i == 2) sprite.setColor(sf::Color(255, 255, 0, 200)), map[i][j] = 1;
                if(j == 5 && i > 1 && i < 18) sprite.setColor(sf::Color(255, 255, 0, 200)), map[i][j] = 1;
                if(j > 4 && j < 19 && i == 17) sprite.setColor(sf::Color(255, 255, 0, 200)), map[i][j] = 1;
                if(j > 4 && j < 18 && i == 13) sprite.setColor(sf::Color(255, 255, 0, 200)), map[i][j] = 1;
                if(j > 4 && j < 12 && i == 9) sprite.setColor(sf::Color(255, 255, 0, 200)), map[i][j] = 1;
                if(j > 12 && j < 17 && i == 6) sprite.setColor(sf::Color(255, 255, 0, 200)), map[i][j] = 1;
                if(j == 18 && 8 < i && i < 18) sprite.setColor(sf::Color(255, 255, 0, 200)), map[i][j] = 1;
                if(j == 17 && 1 < i && i < 10) sprite.setColor(sf::Color(255, 255, 0, 200)), map[i][j] = 1;
                if(j == 12 && 5 < i && i < 10) sprite.setColor(sf::Color(255, 255, 0, 200)), map[i][j] = 1;
                if(j == 19 && i == 9) sprite.setColor(sf::Color(255, 0, 0, 200)), end_x = i, end_y = j, map[i][j] = 1;
                if(j == 3 && 3 < i && i < 19) sprite.setColor(sf::Color(0, 255, 0, 200)), map[i][j] = 2;
                if(j == 15 && 7 < i && i < 12) sprite.setColor(sf::Color(0, 255, 0, 200)), map[i][j] = 2;
                if(j == 14 && 7 < i && i < 12) sprite.setColor(sf::Color(0, 255, 0, 200)), map[i][j] = 2;
                if(6 < j && j < 17 && i == 15) sprite.setColor(sf::Color(0, 255, 0, 200)), map[i][j] = 2;
                if(6 < j && j < 14 && i == 11) sprite.setColor(sf::Color(0, 255, 0, 200)), map[i][j] = 2;
                if(6 < j && j < 16 && i == 4) sprite.setColor(sf::Color(0, 255, 0, 200)), map[i][j] = 2;
                if(6 < j && j < 11 && i == 6) sprite.setColor(sf::Color(0, 255, 0, 200)), map[i][j] = 2;
                if(6 < j && j < 11 && i == 5) sprite.setColor(sf::Color(0, 255, 0, 200)), map[i][j] = 2;
                if(6 < j && j < 11 && i == 7) sprite.setColor(sf::Color(0, 255, 0, 200)), map[i][j] = 2;
                // Draw it
                window.draw(sprite);
            }
        for(auto &i: enemies)
            i.draw(window),
            i.move();
        for(auto &i: towers)
            i->draw(window),
            i->reload(window, enemies);
        //window.draw(shape);
        window.display();
    }

    return 0;
}
