#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

void pollEvents(sf::RenderWindow &window)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::Closed:
            window.close();
            break;
        default:
            break;
        }
    }
}

void redrawFrame(sf::RenderWindow &window, sf::Sprite &sprite)
{
    window.clear(sf::Color(0xff, 0xff, 0xff));
    window.draw(sprite);
    window.display();
}

int main()
{
    constexpr unsigned WINDOW_WIDTH = 800;
    constexpr unsigned WINDOW_HEIGHT = 600;

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(
        sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}),
        "Arrow follows mouse", sf::Style::Default, settings);

    sf::Texture texture;
    sf::Sprite sprite;
    if (!texture.loadFromFile("cat.png"))
    {
        return 0;
    }
    sprite.setTexture(texture);
    sprite.setPosition({400, 300});
    while (window.isOpen())
    {
        pollEvents(window);
        redrawFrame(window, sprite);
    }
}