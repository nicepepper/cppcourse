#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <vector>

#define BALL_SIZE 50

struct ball
{
    sf::CircleShape shape;
    sf::Vector2f speed;
};

void initBalls(std::vector<ball> &balls)
{
    balls[0].shape.setPosition({50, 50});
    balls[0].speed = {50.f, 15.f};
    balls[0].shape.setFillColor(sf::Color(242, 0, 0));
    balls[0].shape.setRadius(BALL_SIZE);

    balls[1].shape.setPosition({350, 100});
    balls[1].speed = {50.f, 15.f};
    balls[1].shape.setFillColor(sf::Color(227, 255, 0));
    balls[1].shape.setRadius(BALL_SIZE);

    balls[2].shape.setPosition({600, 75});
    balls[2].speed = {50.f, 15.f};
    balls[2].shape.setFillColor(sf::Color(0, 141, 15));
    balls[2].shape.setRadius(BALL_SIZE);

    balls[3].shape.setPosition({200, 350});
    balls[3].speed = {50.f, 15.f};
    balls[3].shape.setFillColor(sf::Color(2, 78, 128));
    balls[3].shape.setRadius(BALL_SIZE);

    balls[4].shape.setPosition({500, 400});
    balls[4].speed = {50.f, 15.f};
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

void update(std::vector<ball> &balls, const float deltaTime, unsigned WINDOW_WIDTH, unsigned WINDOW_HEIGHT)
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
    sf::Clock clock;

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(
        sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}),
        "Bouncing balls", sf::Style::Default, settings);

    std::vector<ball> balls(5);
    initBalls(balls);
    while (window.isOpen())
    {
        const float deltaTime = clock.restart().asSeconds();
        pollEvents(window);
        update(balls, deltaTime, WINDOW_WIDTH, WINDOW_HEIGHT);
        redrawFrame(window, balls);
    }
}