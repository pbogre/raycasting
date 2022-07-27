#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Config.hpp>
#include <cmath>

//#include <player.h>

int main(int argc, char *argv[])
{
        int frequency = 6;
        if (argc > 1) frequency = strtol(argv[1], NULL, 10);

        sf::RenderWindow window(sf::VideoMode(900, 600), "Raycasting");
        window.setFramerateLimit(60);

        sf::RectangleShape line(sf::Vector2f(1, 300));

        int frame = 0;
        while (window.isOpen()){

                sf::Event event;
                while (window.pollEvent(event))
                {
                        if (event.type == sf::Event::Closed)
                                window.close();   
                        if (event.type == sf::Event::Resized){
                                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                                window.setView(sf::View(visibleArea));
                        }
                }
                window.clear();

                line.setPosition(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
                

                for(int i = 0; i < 360/frequency; i++){

                        sf::RectangleShape v_component(sf::Vector2f(1, line.getPosition().y));
                                v_component.setFillColor(sf::Color::Green);
                        sf::RectangleShape h_component(sf::Vector2f(line.getPosition().x, 1));
                                h_component.setPosition(0, line.getPosition().y);
                                h_component.setFillColor(sf::Color::Red);

                        float hypothenuse = sqrt(pow(line.getPosition().x,2)+pow(line.getPosition().y,2));
                        line.setSize(sf::Vector2f(line.getSize().x, hypothenuse));

                        line.setRotation((frequency)+frame);
                        //std::cout << "Rotation: " << line.getRotation() << " | Position: " << line.getPosition().x << ", " << line.getPosition().y << std::endl;

                        window.draw(line);
                        window.draw(v_component);
                        window.draw(h_component);
                }

                window.display();
                frame++;
    }

    return 0;
}