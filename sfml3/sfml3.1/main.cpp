#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <algorithm>

void init(sf::ConvexShape &pointer)
{
    pointer.setPointCount(3);
    pointer.setPoint(0, {40, 0});
    pointer.setPoint(1, {-20, -20});
    pointer.setPoint(2, {-20, 20});
    pointer.setPosition({400, 300});
    pointer.setRotation(0);
    pointer.setFillColor(sf::Color(0xFF, 0x80, 0x00));
}
float toDegrees(float radians)
{
    return float(double(radians) * 180.0 / M_PI);
}
void onMouseMove(const sf::Event::MouseMoveEvent &event, sf::Vector2f &mousePosition)
{
    std::cout << "mouse x=" << event.x << " , y=" << event.y << std::endl;
    mousePosition = {float(event.x), float(event.y)};
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
int determineDirection(const float &arrowAngle, const float &mouseAngle)
{
    if (arrowAngle > mouseAngle)
    {
        if ((arrowAngle - mouseAngle) < 180)
        {
            return -1;
        }
        else
        {
            return 1;
        }
    }
    else
    {
        if ((mouseAngle - arrowAngle) <= 180)
        {
            return 1;
        }
        else
        {
            return -1;
        }
    }
}
void toCoordinateSystem(float &mouseAngle)
{
    if ((-180 <= mouseAngle) & (mouseAngle < 0))
    {
        if (mouseAngle == -180)
        {
            mouseAngle = 180;
        }
        else
        {
            mouseAngle = 360 + mouseAngle;
        }
    }
}
void moveArrow(const float &arrowAngle, float &deltaAngle)
{
    if ((arrowAngle + deltaAngle) >= 360)
    {
        deltaAngle = arrowAngle + deltaAngle - 360;
    }
    else
    {
        deltaAngle = arrowAngle + deltaAngle;
    }
}
void minimumAngle(float &deltaAngle, const float &arrowAngle, const float &mouseAngle)
{
    if (deltaAngle > 0)
    {
        deltaAngle = std::min(deltaAngle, std::abs(arrowAngle - mouseAngle));
    }
    else
    {
        deltaAngle = -std::min(std::abs(deltaAngle), std::abs(arrowAngle - mouseAngle));
    }
}
void update(const sf::Vector2f &mousePosition, sf::ConvexShape &pointer, const float &deltaTime, const float &SPEED)
{
    const sf::Vector2f delta = mousePosition - pointer.getPosition();
    const float angle = atan2(delta.y, delta.x);
    const float arrowAngle = pointer.getRotation();
    float mouseAngle = toDegrees(angle);
    toCoordinateSystem(mouseAngle);
    if (arrowAngle != mouseAngle)
    {
        float deltaAngle = SPEED * deltaTime * determineDirection(arrowAngle, mouseAngle);
        minimumAngle(deltaAngle, arrowAngle, mouseAngle);
        moveArrow(arrowAngle, deltaAngle);
        pointer.setRotation(deltaAngle);
    }
}
void redrawFrame(sf::RenderWindow &window, sf::ConvexShape &pointer)
{
    window.clear();
    window.draw(pointer);
    window.display();
}
int main()
{
    sf::Clock clock;
    constexpr unsigned WINDOW_WIDTH = 800;
    constexpr unsigned WINDOW_HEIGHT = 600;
    constexpr float SPEED = 90;

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(
        sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}),
        "Pointer follows mouse", sf::Style::Default, settings);
    sf::ConvexShape pointer;
    sf::Vector2f mousePosition;

    init(pointer);
    while (window.isOpen())
    {
        const float deltaTime = clock.restart().asSeconds();
        pollEvents(window, mousePosition);
        update(mousePosition, pointer, deltaTime, SPEED);
        redrawFrame(window, pointer);
    }
}