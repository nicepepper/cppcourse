#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <vector>

struct ball
{
    sf::CircleShape shape;
    sf::Vector2f speed;
};

void initBalls(std::vector<ball> &balls, unsigned BALL_SIZE)
{
    balls[0].shape.setPosition({50, 50});
    balls[0].speed = {50.f, 15.f};
    balls[0].shape.setFillColor(sf::Color(242, 0, 0));
    balls[0].shape.setRadius(BALL_SIZE);

    balls[1].shape.setPosition({350, 100});
    balls[1].speed = {60.f, -30.f};
    balls[1].shape.setFillColor(sf::Color(227, 255, 0));
    balls[1].shape.setRadius(BALL_SIZE);

    balls[2].shape.setPosition({600, 75});
    balls[2].speed = {100.f, 100.f};
    balls[2].shape.setFillColor(sf::Color(0, 141, 15));
    balls[2].shape.setRadius(BALL_SIZE);

    balls[3].shape.setPosition({200, 350});
    balls[3].speed = {-60.f, -35.f};
    balls[3].shape.setFillColor(sf::Color(2, 78, 128));
    balls[3].shape.setRadius(BALL_SIZE);

    balls[4].shape.setPosition({500, 400});
    balls[4].speed = {20.f, 45.f};
    balls[4].shape.setFillColor(sf::Color(120, 140, 0));
    balls[4].shape.setRadius(BALL_SIZE);
}

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
float dotOperation(ball ballI, ball ballJ, sf::Vector2f positionI, sf::Vector2f positionJ)
{
    sf::Vector2f overallSpeed = ballI.speed - ballJ.speed;
    sf::Vector2f overallPosition = positionI - positionJ;
    return overallSpeed.x * overallPosition.x + overallSpeed.y * overallPosition.y;
}

float centerDifferenceModule2(sf::Vector2f positionI, sf::Vector2f positionJ)
{
    sf::Vector2f center = positionI - positionJ;
    return std::sqrt(center.x * center.x + center.y * center.y);
}

sf::Vector2f speedCorrection(ball ball1, ball ball2, sf::Vector2f positionI, sf::Vector2f positionJ)
{
    float dot = dotOperation(ball1, ball2, positionI, positionJ);
    float modul2 = centerDifferenceModule2(positionI, positionJ);
    sf::Vector2f vectorPoint = positionI - positionJ;
    return ((dot / modul2) * vectorPoint);
}

void update(std::vector<ball> &balls, const float deltaTime, unsigned WINDOW_WIDTH, unsigned WINDOW_HEIGHT, unsigned BALL_SIZE)
{
    sf::Vector2f position = {0.f, 0.f};
    for (int i = 0; i < balls.size(); i++)
    {
        position = balls[i].shape.getPosition();
        position += balls[i].speed * deltaTime;
        if ((position.x + 2 * BALL_SIZE >= WINDOW_WIDTH) && (balls[i].speed.x > 0))
        {
            balls[i].speed.x = -balls[i].speed.x;
        }
        if ((position.x < 0) && (balls[i].speed.x < 0))
        {
            balls[i].speed.x = -balls[i].speed.x;
        }
        if ((position.y + 2 * BALL_SIZE >= WINDOW_HEIGHT) && (balls[i].speed.y > 0))
        {
            balls[i].speed.y = -balls[i].speed.y;
        }
        if ((position.y < 0) && (balls[i].speed.y < 0))
        {
            balls[i].speed.y = -balls[i].speed.y;
        }
        balls[i].shape.setPosition(position);
    }
    for (int i = 0; i < balls.size(); i++)
    {
        for (int j = i + 1; j < balls.size(); j++)
        {
            sf::Vector2f vectorPoint = balls[i].shape.getPosition() - balls[j].shape.getPosition();
            float vectorLenght = std::sqrt(vectorPoint.x * vectorPoint.x + vectorPoint.y * vectorPoint.y);
            if (vectorLenght < 2 * BALL_SIZE)
            {
                sf::Vector2f speedI = balls[i].speed;
                sf::Vector2f speedJ = balls[j].speed;
                sf::Vector2f positionI = balls[i].shape.getPosition();
                sf::Vector2f positionJ = balls[j].shape.getPosition();
                float lengthI1 = std::sqrt(speedI.x * speedI.x + speedI.y * speedI.y);
                float lengthJ1 = std::sqrt(speedJ.x * speedJ.x + speedJ.y * speedJ.y);
                speedI = balls[i].speed - speedCorrection(balls[i], balls[j], positionI, positionJ);
                speedJ = balls[j].speed - speedCorrection(balls[j], balls[i], positionJ, positionI);
                float lengthI2 = std::sqrt(speedI.x * speedI.x + speedI.y * speedI.y);
                float lengthJ2 = std::sqrt(speedJ.x * speedJ.x + speedJ.y * speedJ.y);
                sf::Vector2f normalizationI = {speedI.x / lengthI2, speedI.y / lengthI2};
                balls[i].speed = lengthJ1 * normalizationI;
                sf::Vector2f normalizationJ = {speedJ.x / lengthJ2, speedJ.y / lengthJ2};
                balls[j].speed = lengthI1 * normalizationJ;
            }
        }
    }
}

void redrawFrame(sf::RenderWindow &window, std::vector<ball> &balls)
{
    window.clear(sf::Color(0, 0, 0));
    window.draw(balls[0].shape);
    window.draw(balls[1].shape);
    window.draw(balls[2].shape);
    window.draw(balls[3].shape);
    window.draw(balls[4].shape);
    window.display();
}

int main()
{
    unsigned WINDOW_WIDTH = 800;
    unsigned WINDOW_HEIGHT = 600;
    unsigned BALL_SIZE = 50;
    sf::Clock clock;

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(
        sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}),
        "Bouncing balls", sf::Style::Default, settings);

    std::vector<ball> balls(5);
    initBalls(balls, BALL_SIZE);
    while (window.isOpen())
    {
        const float deltaTime = clock.restart().asSeconds();
        pollEvents(window);
        update(balls, deltaTime, WINDOW_WIDTH, WINDOW_HEIGHT, BALL_SIZE);
        redrawFrame(window, balls);
    }
}