#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <cmath>

void initCat(sf::Sprite &cat, sf::Texture &catTexture)
{
    cat.setTexture(catTexture);
    cat.setPosition({400, 300});
    cat.setOrigin({19, 17});
}

void initPointer(sf::Sprite &pointer, sf::Texture &pointerTexture)
{
    pointer.setTexture(pointerTexture);
    pointer.setPosition({400, 300});
    pointer.setOrigin({16, 16});
}

float calculateDeltaDistance(float &deltaTime, float &SPEED_CAT)
{
    return deltaTime * SPEED_CAT;
}

float toDegrees(float radians)
{
    return float(double(radians) * 180.0 / M_PI);
}

void toCoordinateSystem(float &angle)
{
    if (angle < 0)
    {
        angle = 360 + angle;
    }
}

void onMouseLeftButton(const sf::Event::MouseButtonEvent &event, sf::Vector2f &mousePosition)
{
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
        case sf::Event::MouseButtonPressed:
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                onMouseLeftButton(event.mouseButton, mousePosition);
            }
            break;
        default:
            break;
        }
    }
}

void updatePointer(sf::Sprite &pointer, sf::Vector2f &mousePosition)
{
    pointer.setPosition({mousePosition.x, mousePosition.y});
}

sf::Vector2f toDirectionVector(sf::Vector2f &deltaVector, float &lengthVector)
{
    return {
        deltaVector.x / lengthVector,
        deltaVector.y / lengthVector};
}

void moveCat(sf::Sprite &cat, sf::Vector2f &deltaVector, float &lengthVector, float &deltaLength)
{
    if (deltaLength > lengthVector)
    {
        deltaLength = lengthVector;
    }
    cat.setPosition({cat.getPosition() + toDirectionVector(deltaVector, lengthVector) * deltaLength});
}

void toScale(sf::Sprite &cat, sf::Vector2f deltaVector)
{
    float angle = toDegrees(std::atan2(deltaVector.y, deltaVector.x));
    toCoordinateSystem(angle);
    ((angle > 90) && (angle < 270)) ? cat.setScale({-1, 1}) : cat.setScale({1, 1});
}

void updateCat(sf::Sprite &cat, sf::Vector2f &mousePosition, float &deltaLength)
{
    sf::Vector2f deltaVector = mousePosition - cat.getPosition();
    float lengthVector = std::sqrt(double(std::pow(deltaVector.x, 2)) + double(std::pow(deltaVector.y, 2)));
    if (lengthVector != 0)
    {
        moveCat(cat, deltaVector, lengthVector, deltaLength);
        toScale(cat, deltaVector);
    }
}

void update(sf::Sprite &cat, sf::Sprite &pointer, sf::Vector2f &mousePosition, float &deltaLength)
{
    updatePointer(pointer, mousePosition);
    updateCat(cat, mousePosition, deltaLength);
}

void redrawFrame(sf::RenderWindow &window, sf::Sprite &pointer, sf::Sprite &cat)
{
    window.clear(sf::Color(0xFF, 0xFF, 0xFF));
    window.draw(pointer);
    window.draw(cat);
    window.display();
}

int main()
{
    constexpr unsigned WINDOW_WIDTH = 800;
    constexpr unsigned WINDOW_HEIGHT = 600;
    float SPEED_CAT = 100;

    sf::Clock clock;
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(
        sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}),
        "Arrow follows mouse", sf::Style::Default, settings);

    sf::Vector2f mousePosition = {400, 300};

    sf::Texture catTexture;
    sf::Sprite cat;
    if (!catTexture.loadFromFile("cat.png"))
    {
        return 0;
    }
    initCat(cat, catTexture);
    sf::Texture pointerTexture;
    sf::Sprite pointer;
    if (!pointerTexture.loadFromFile("red_pointer.png"))
    {
        return 0;
    }
    initPointer(pointer, pointerTexture);

    while (window.isOpen())
    {
        float deltaTime = clock.restart().asSeconds();
        float deltaLength = calculateDeltaDistance(deltaTime, SPEED_CAT);
        pollEvents(window, mousePosition);
        update(cat, pointer, mousePosition, deltaLength);
        redrawFrame(window, pointer, cat);
    }
}