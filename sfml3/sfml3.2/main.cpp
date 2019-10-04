#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <cmath>

struct Eye
{
    sf::ConvexShape whiteOfEye;
    sf::ConvexShape pupilOfTheEye;
    sf::Vector2f position;
    float rotation = -90;
    float angle = 0;
};
void drawEllips(sf::ConvexShape &shape, const sf::Vector2f &radius);
void initEye(Eye &eye, sf::Vector2f position);
void onMouseMove(const sf::Event::MouseMoveEvent &event, sf::Vector2f &mousePosition);
void pollEvents(sf::RenderWindow &window, sf::Vector2f &mousePosition);
sf::Vector2f toEuvclidean(float radius, float angle);
float toDegrees(float radians);
void updatePupilElements(const sf::Vector2f &mousePosition, Eye &eye, sf::Vector2f &delta);
void update(const sf::Vector2f &mousePosition, Eye &eye);
void redrawFrame(sf::RenderWindow &window, Eye &eye);

void drawEllips(sf::ConvexShape &shape, const sf::Vector2f &radius)
{
    constexpr int pointCount = 200;
    shape.setPointCount(pointCount);
    for (int pointNo = 0; pointNo < pointCount; ++pointNo)
    {
        float angle = float(2 * M_PI * pointNo) / float(pointCount);
        sf::Vector2f point = {
            radius.x * std::sin(angle),
            radius.y * std::cos(angle)};
        shape.setPoint(pointNo, point);
    }
}
void initEye(Eye &eye, sf::Vector2f position)
{
    eye.position = position;
    const sf::Vector2f eyeRaius = {100.f, 50.f};
    const sf::Vector2f pupilRadius = {20.f, 10.f};
    eye.whiteOfEye.setRotation(eye.rotation);
    eye.pupilOfTheEye.setRotation(eye.rotation);
    eye.whiteOfEye.setPosition(eye.position);
    eye.pupilOfTheEye.setPosition(eye.position);

    eye.whiteOfEye.setFillColor(sf::Color(0xFF, 0xFF, 0xFF));
    eye.pupilOfTheEye.setFillColor(sf::Color(0x00, 0x00, 0x00));
    drawEllips(eye.whiteOfEye, eyeRaius);
    drawEllips(eye.pupilOfTheEye, pupilRadius);

    updatePupilElements(eye);
}
void onMouseMove(const sf::Event::MouseMoveEvent &event, sf::Vector2f &mousePosition)
{
    std::cout << "mouse x=" << event.x << ", y=" << event.y << std::endl;
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
sf::Vector2f toEuvclidean(float radius, float angle)
{
    return {
        radius * std::cos(angle),
        radius * std::sin(angle)};
}
float toDegrees(float radians)
{
    return float(double(radians) * 180.0 / M_PI);
}
void updatePupilElements(const sf::Vector2f &mousePosition, Eye &eye, sf::Vector2f &delta)
{
    sf::Vector2f trackRadius = {20.f, 40.f};
    float R = float(delta.y) / float(std::sin(eye.angle));
    float r1 = float() std::cos(eye.angle) float r2 if (R > r)
    {
        eye.pupilOfTheEye.setPosition({eye.position.x + (trackRadius.x * std::cos(eye.angle)), eye.position.y + (trackRadius.y * std::sin(eye.angle))});
    }
    else
    {
        eye.pupilOfTheEye.setPositon(mousePosition);
    }
}
void update(const sf::Vector2f &mousePosition, Eye &eye)
{
    sf::Vector2f delta = mousePosition - eye.position;
    eye.angle = std::atan2(delta.y, delta.x);
    updatePupilElements(eye, mousePosition, delta);
}
void redrawFrame(sf::RenderWindow &window, Eye &eye)
{
    window.draw(eye.whiteOfEye);
    window.draw(eye.pupilOfTheEye);
}

int main()
{
    constexpr unsigned WINDOW_WIDTH = 900;
    constexpr unsigned WINDOW_HEIGHT = 600;
    sf::Vector2f POSITION_FIRST_EYE = {300, 300};
    sf::Vector2f POSITION_SECOND_EYE = {450, 300};

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(
        sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}),
        "Eye follows mouse", sf::Style::Default, settings);

    Eye eye1;
    Eye eye2;
    sf::Vector2f mousePosition;

    initEye(eye1, POSITION_FIRST_EYE);
    initEye(eye2, POSITION_SECOND_EYE);
    while (window.isOpen())
    {
        pollEvents(window, mousePosition);
        window.clear();
        update(mousePosition, eye1);
        update(mousePosition, eye2);
        redrawFrame(window, eye1);
        redrawFrame(window, eye2);
        window.display();
    }
}