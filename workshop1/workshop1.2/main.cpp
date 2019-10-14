#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <cmath>

void initShape(sf::ConvexShape &arrow)
{
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

    arrow.setPosition({400, 300});
    arrow.setOrigin({25, 0});
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

float toRadians(float angle)
{
    return float(double(angle) * M_PI / 180.0);
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

void toCoordinateSystem(float &angle)
{
    if (angle < 0)
    {
        angle = 360 + angle;
    }
}

int determineDirection(const float &arrowAngle, const float &mouseAngle)
{
    if (arrowAngle > mouseAngle)
    {
        return ((arrowAngle - mouseAngle) < 180) ? -1 : 1;
    }
    else
    {
        return ((mouseAngle - arrowAngle) <= 180) ? 1 : -1;
    }
}

void chooseMinimumAngle(float &deltaAngle, float &arrowAngle, float &mouseAngle)
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

void rotateCorner(float &arrowAngle, const float &deltaAngle)
{
    if ((arrowAngle + deltaAngle) >= 360)
    {
        arrowAngle = arrowAngle + deltaAngle - 360;
    }
    else
    {
        arrowAngle = arrowAngle + deltaAngle;
    }
}

void turnArrow(sf::ConvexShape &arrow, float &mouseAngle, const float &deltaTime, float &ANGULAR_VELOCITY)
{
    toCoordinateSystem(mouseAngle);
    float arrowAngle = arrow.getRotation();
    if (arrowAngle != mouseAngle)
    {
        float deltaAngle = ANGULAR_VELOCITY * deltaTime * float(determineDirection(arrowAngle, mouseAngle));
        chooseMinimumAngle(deltaAngle, arrowAngle, mouseAngle);
        rotateCorner(arrowAngle, deltaAngle);
        arrow.setRotation(arrowAngle);
    }
}

void moveArrow(sf::ConvexShape &arrow, const float &distanceMouse, float &deltaLength)
{
    if (distanceMouse != 0)
    {
        if (deltaLength > distanceMouse)
        {
            deltaLength = distanceMouse;
        }

        sf::Vector2f deltaPosition = toEuclidean(deltaLength, toRadians(arrow.getRotation()));
        arrow.setPosition(arrow.getPosition() + deltaPosition);
    }
}

void update(const sf::Vector2f &mousePosition, sf::ConvexShape &arrow, const float &deltaTime, float &ANGULAR_VELOCITY, float LINEAR_SPEED)
{
    sf::Vector2f deltaVector = mousePosition - arrow.getPosition();
    float distanceMouse = float(std::sqrt(float(std::pow(deltaVector.x, 2)) + float(std::pow(deltaVector.y, 2))));
    float deltaLength = deltaTime * LINEAR_SPEED;
    if (distanceMouse != 0)
    {
        float mouseAngle = toDegrees(std::atan2(deltaVector.y, deltaVector.x));
        turnArrow(arrow, mouseAngle, deltaTime, ANGULAR_VELOCITY);
    }
    moveArrow(arrow, distanceMouse, deltaLength);
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
    float ANGULAR_VELOCITY = 1000;
    float LINEAR_SPEED = 100;

    sf::ConvexShape arrow;
    sf::Vector2f mousePosition;
    sf::Clock clock;
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(
        sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}),
        "Arrow follows mouse", sf::Style::Default, settings);

    initShape(arrow);
    while (window.isOpen())
    {
        const float deltaTime = clock.restart().asSeconds();
        pollEvents(window, mousePosition);
        update(mousePosition, arrow, deltaTime, ANGULAR_VELOCITY, LINEAR_SPEED);
        redrawFrame(window, arrow);
    }
}