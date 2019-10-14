#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <cmath>

void initShape(sf::ConvexShape &arrow)
{
    arrow.setPosition({400, 300});

    arrow.setFillColor(sf::Color(0xFF, 0xFF, 0));
    arrow.setOutlineColor(sf::Color(0, 0, 0));
    arrow.setOutlineThickness(2);

    arrow.setPointCount(7);
    arrow.setPoint(0, {100, 0});
    arrow.setPoint(1, {50, 50});
    arrow.setPoint(2, {50, 25});
    arrow.setPoint(3, {0, 25});
    arrow.setPoint(4, {0, -25});
    arrow.setPoint(5, {50, -25});
    arrow.setPoint(6, {50, -50});
}

void onMouseMove(const sf::Event::MouseMoveEvent &event, sf::Vector2f &mousePosition)
{
    mousePosition = {float(event.x), float(event.y)};
}

sf::Vector2f toEuclidean(float radius, float angle)
{
    return {
        radius * std::cos(angle),
        radius * std::sin(angle)};
}

float toDegrees(float radians)
{
    return float(double(radians) * 180.0 / M_PI);
}

void pollEvents(sf::RenderWindow &window, sf::Vector2f &mousePosition)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::Closed:
            window.close();
            break;
        case sf::Event::MouseMoved:
            onMouseMove(event.mouseMove, mousePosition);
            break;
        default:
            break;
        }
    }
}

void update(const sf::Vector2f &mousePosition, sf::ConvexShape &arrow)
{
    sf::Vector2f delta = mousePosition - arrow.getPosition();
    float angle = std::atan2(delta.y, delta.x);
    arrow.setRotation(toDegrees(angle));
}

void redrawFrame(sf::RenderWindow &window, sf::ConvexShape &arrow)
{
    window.clear(sf::Color(0xff, 0xff, 0xff));
    window.draw(arrow);
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

    sf::ConvexShape arrow;
    initShape(arrow);

    sf::Vector2f mousePosition;

    while (window.isOpen())
    {
        pollEvents(window, mousePosition);
        update(mousePosition, arrow);
        redrawFrame(window, arrow);
    }
}