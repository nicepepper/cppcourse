#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <cmath>

int main()
{
    const sf::Clock clock;
    constexpr int pointCount = 200;
    constexpr float circleRadius = 25;

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(
        sf::VideoMode({800, 600}), "Polar Rosa",
        sf::Style::Default, settings);

    sf::ConvexShape rosa;
    rosa.setPosition({400, 300});
    rosa.setFillColor(sf::Color(0xFF, 0x09, 0x80));
    rosa.setPointCount(pointCount);
    for (int pointNo = 0; pointNo < pointCount; ++pointNo)
    {
        float angle = float(2 * M_PI * pointNo) / float(pointCount);
        float rosaRadius = pointCount * sin(6 * angle);
        sf::Vector2f point = sf::Vector2f{
            rosaRadius * std::sin(angle),
            rosaRadius * std::cos(angle)};
        rosa.setPoint(pointNo, point);
    }

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            {
                if ((event.type == sf::Event::Closed) || ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)))
                    window.close();
            }
        }

        const float time = clock.getElapsedTime().asSeconds();
        const float ugol = time * float(M_PI / 10.f) * 4;
        sf::Vector2f circleTreck = sf::Vector2f{
            circleRadius * std::cos(ugol),
            circleRadius * std::sin(ugol)};

        rosa.setPosition(403 + circleTreck.x, 300 + circleTreck.y);

        window.clear();
        window.draw(rosa);
        window.display();
    }
}