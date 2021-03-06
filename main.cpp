#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <tuple>
#include <utility>
#include <memory>
#include <fstream>
#include <string>
#include <sstream>
#include <sys/stat.h>

#define abs(x) ((x)>0?(x):-(x))
#define sqr(x) (x)*(x)

const int edge = 52;

int start_x; // position where enemies start their way
int start_y;
int end_x;// finish
int end_y;
std::vector<std::vector<int>> map(30, std::vector<int>(30, 0));

int dist(int x, int y)// manheten distance to the end
{
    return abs(end_x-x) + abs(end_y-y);
}

int count_of(std::pair<int, int> a)//how many cell tower can shoot from this position
{
    int i = a.first;
    int j = a.second;
    int x = (map[i-1][j] == 1) + (map[i-2][j] == 1) + (map[i-1][j-1] == 1) + (map[i-2][j-1] == 1) +
      (map[i-1][j-2] == 1) + (map[i-2][j-2] == 1) + (map[i-1][j+2] == 1) + (map[i-2][j+2] == 1) +
      (map[i-1][j+1] == 1) + (map[i-2][j+1] == 1) + (map[i][j-2] == 1) + (map[i][j-1] == 1) +
      (map[i][j+2] == 1) + (map[i][j+1] == 1) + (map[i+1][j-2] == 1) + (map[i+1][j-1] == 1) +
      (map[i+1][j] == 1) + (map[i+1][j+1] == 1) + (map[i+1][j+2] == 1);
    if(j != 18) x += (map[i+2][j-2] == 1) + (map[i+2][j-1] == 1) + (map[i+2][j] == 1) +
      (map[i+2][j+1] == 1) + (map[i+2][j+2] == 1);
    return x;
}

bool comp(std::pair<int, int> a, std::pair<int, int> b)//comparator for sorting
{
    int x = count_of(a);
    int y = count_of(b);
    return x > y;
}

class textures//object for every texture
{
public:
    sf::Texture ok;//for "ok" button
    sf::Texture add;//for "add" button
    sf::Texture cancel;//for "cancel" button
    sf::Texture map;// for every cell in the map
    textures(std::string ok, std::string add, std::string cancel, std::string map)
    {
        this->ok.loadFromFile(ok);
        this->add.loadFromFile(add);
        this->cancel.loadFromFile(cancel);
        this->map.loadFromFile(map);
    }
};

class enemy
{
    int next_x;//next cell on the way
    int next_y;
    int step;
    std::vector<std::vector<bool>> vis;//array of visited cells
public:
    int x;//current position
    int y;
    int xp;// how many helpth point enemy has
    enemy():
        step(1),
        xp(100),
        next_x(19),
        next_y(1),
        x(19*edge),
        y(edge),
        vis(20, std::vector<bool>(20, 0))
    {};
    void move()//move enemy to the next position
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
    void draw(sf::RenderWindow &window, std::unique_ptr<textures> &tex) // draw enemy
    {
        sf::Texture texture = tex->map;
        //texture.loadFromFile("texture.png");
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
    int x;//position
    int rearm;//how long tower reloads
    int y;
    bool is_active;// do we need to draw circle
public:
    tower(int cur_x, int cur_y):
        x(cur_x),
        y(cur_y),
        rearm(0),
        is_active(1)
    {};
    bool is_pos(int ax, int ay)//is our tower loceted in that cell
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
    void draw(sf::RenderWindow &window, std::unique_ptr<textures> &tex)// draw tower
    {
        if(is_active)
        {
            sf::CircleShape shape(150.f);
            shape.setFillColor(sf::Color(0, 255, 255, 50));
            shape.setPosition(x*edge-125, y*edge-125);
            window.draw(shape);
        }
        sf::Texture texture = tex->map;
        //texture.loadFromFile("texture.png");
        sf::Sprite sprite;
        sprite.setTexture(texture);
        sprite.setTextureRect(sf::IntRect(10, 10, edge-1, edge-1));
        sprite.setPosition(x*edge, y*edge);
        sprite.setColor(sf::Color(0, 0, 0, 200));
        window.draw(sprite);
    };
    void reload(sf::RenderWindow &window, std::vector<enemy> &enemies)//roload tower and hit enemies if reload is ended
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
    sf::RenderWindow login(sf::VideoMode(600, 150), "Login", sf::Style::None);
    sf::Font font;
    if (!font.loadFromFile("arial.ttf"))
        return 0;
    std::unique_ptr<textures> textur(new textures("ok.png", "add.png", "cancel.png", "texture.png"));
    sf::String user_str;//place, where we hold username
    sf::Text user_text;
    sf::String pass_str;//place, where we hold password
    sf::Text pass_text;
    pass_text.setFont(font);
    user_text.setFont(font);
    user_text.setString("Username: ");
    pass_text.setString("Password: ");
    user_text.setCharacterSize(24);
    pass_text.setCharacterSize(24);
    user_text.setColor(sf::Color::Red);
    pass_text.setColor(sf::Color::Red);
    user_text.setStyle(sf::Text::Bold);
    pass_text.setStyle(sf::Text::Bold);
    pass_text.setPosition(0, 75);
    sf::Texture ok = textur->ok;//load ok button
    //ok.loadFromFile("ok.png");
    sf::Texture add = textur->add;//load add button
    //add.loadFromFile("add.png");
    sf::Texture cancel = textur->cancel;//load cancel button
    //cancel.loadFromFile("cancel.png");
    sf::Sprite draw_ok;
    sf::Sprite draw_add;
    sf::Sprite draw_cancel;
    std::string last_user("");
    std::string last_pass("");
    bool passwo = 0;
    while(login.isOpen())
    {
        login.clear(sf::Color(255, 255, 255));
        sf::Event event;
        while (login.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                login.close();
            if(event.type == sf::Event::TextEntered)
                if(passwo && event.text.unicode != ' ')//check where we should write
                {
                    if (event.text.unicode == '\b')
                        pass_str.erase(pass_str.getSize() - 1, 1);
                    else if(event.text.unicode < 128)
                        pass_str += event.text.unicode;
                    pass_text.setString("Password: " + pass_str);// write to password
                } else
                {
                    if (event.text.unicode == '\b')
                        user_str.erase(user_str.getSize() - 1, 1);
                    else if(event.text.unicode < 128)
                        user_str += event.text.unicode;
                    user_text.setString("Username: " + user_str);// write to username
                }
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
            ;//login.close();

        if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            int x = sf::Mouse::getPosition(login).x;
            int y = sf::Mouse::getPosition(login).y;
            if(x > 450)
            {
                struct stat results;
                if (stat("password.txt", &results))
                    return 0;
                std::string buff;
                buff.resize(results.st_size);
                std::ifstream fin("password.txt", std::ios::in | std::ios::binary);//read file with passwords
                fin.read((char*)buff.c_str(), buff.length());
                if(y < 50)// if ok button is pressed
                {
                    std::stringstream ss;
                    ss << buff;
                    std::string usr, pas;
                    while(ss >> usr >> pas)
                        if(usr == user_str && pas == pass_str)//check if password exist
                            login.close();
                } else if(y >= 100)//check if cancel button is pressed
                    return 0;
                else if(last_user != user_str || last_pass != pass_str)//check if add button is pressed
                {
                    std::ofstream fout("password.txt", std::ios::out | std::ios::binary);
                    buff += (std::string)user_str + ' ' + (std::string)pass_str + '\n';
                    fout.write(buff.c_str(), buff.length());
                    last_user = user_str;
                    last_pass = pass_str;
                }
            } else if(y < 75)
                passwo = 0;
            else
                passwo = 1;
        }



        login.draw(user_text);//drawing everythink
        login.draw(pass_text);
        draw_ok.setTexture(ok);
        draw_add.setTexture(add);
        draw_cancel.setTexture(cancel);
        draw_ok.setPosition(450, 0);
        draw_add.setPosition(450, 50);
        draw_cancel.setPosition(450, 100);
        login.draw(draw_ok);
        login.draw(draw_add);
        login.draw(draw_cancel);
        login.display();
    }

    std::vector<std::pair<int, int>> st;//array for every available tower position
    sf::RenderWindow window(sf::VideoMode(600, 200), "SFML works!", sf::Style::Fullscreen);
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);
    int cnt = rand()%100+100;
    std::vector<enemy> enemies;
    std::vector<std::shared_ptr<tower>> towers;
    std::shared_ptr<tower> active = nullptr;
    while (window.isOpen())
    {
        if(!--cnt)
        {
            enemies.push_back(enemy());//generate enemies
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
                active->turn_off();// if we can, we create new tower, and tuen off previous
            towers.push_back(std::make_shared<tower>(x, y));
            active = towers.back();
            //sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
            //shape.setPosition((float)mouse_pos.x, (float)mouse_pos.y);
        }
        out:;

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
            window.close();
        sf::Texture texture = textur->map;
        //texture.loadFromFile("texture.png");
        sf::Sprite sprite;
        sprite.setTexture(texture);
        sprite.setTextureRect(sf::IntRect(10, 10, edge-1, edge-1));
        for(int i = 0; i < 20; i++)
            for(int j = 0; j < 20; j++)
            {
                sprite.setPosition(i*(edge), j*(edge));
                sprite.setColor(sf::Color(255, 255, 255, 200));//drawing map, if map[i][j] == 1 than this cell is road, if 2 -- plac for tower
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
        if(st.empty())
        {
            for(int i = 0; i < 20; i++)
                for(int j = 0; j < 20; j++)
                    if(map[i][j] == 2)
                        st.push_back({i, j});
            sort(st.begin(), st.end(), comp);//sort all cells
        }
        for(int i = 0; i < 10; i++)
        {
            sprite.setPosition(st[i].first*edge, st[i].second*edge);
            sprite.setColor(sf::Color(0*(12-i), 255, 20*(12-i), 255));
            window.draw(sprite);//draw optimal cells for towers
        }

        for(auto &i: enemies)//drawing and moving enemies
            i.draw(window, textur),
            i.move();
        for(auto &i: towers)//reloading and drawing towers
            i->draw(window, textur),
            i->reload(window, enemies);
        //window.draw(shape);
        window.display();
    }

    return 0;
}
