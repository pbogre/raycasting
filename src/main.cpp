#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Config.hpp>
#include <cmath>

float to_rad(float deg);
float to_deg(float rad);

int main(int argc, char *argv[])
{
        int frequency = 6;
        if (argc > 1 && strtol(argv[1], NULL, 10) > 0) frequency = strtol(argv[1], NULL, 10);
        int lines_number = (360/frequency);

        sf::ContextSettings settings;
            settings.antialiasingLevel = 4;
        sf::RenderWindow window(sf::VideoMode(900, 600), "Raycasting", sf::Style::Fullscreen, settings);
        window.setFramerateLimit(60);

        sf::RectangleShape line(sf::Vector2f(1, 300));
        sf::RectangleShape wall(sf::Vector2f(25, 300));

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


                int mouse_x = sf::Mouse::getPosition(window).x;
                int mouse_y = sf::Mouse::getPosition(window).y;

                float vert = (window.getSize().y - mouse_y);

                wall.setPosition(850, 300);
                wall.setRotation(60);
                wall.setFillColor(sf::Color::Transparent);
                wall.setOutlineThickness(2);
                wall.setOutlineColor(sf::Color::Red);
                window.draw(wall);

                line.setPosition(mouse_x, mouse_y);
                for(int i = 0; i < lines_number; i++){
                    float rotation = i*frequency;

                    float hypothenuse = ( vert / cos(to_rad(rotation)) );
                    if (rotation >= 90 && rotation < 270) {
                        hypothenuse = -( mouse_y / cos(to_rad(rotation)) );
                    }

                    line.setRotation(rotation);
                    line.setSize(sf::Vector2f(line.getSize().x, hypothenuse));

                    window.draw(line);
                }
                
                sf::RectangleShape v_component(sf::Vector2f(1, vert));
                    v_component.setPosition(0, window.getSize().y - vert);
                    v_component.setFillColor(sf::Color::Green);
                sf::RectangleShape h_component(sf::Vector2f(mouse_x, 1));
                    h_component.setPosition(0, mouse_y);
                    h_component.setFillColor(sf::Color::Red);
                window.draw(v_component);
                window.draw(h_component);

                window.display();
    }

    return 0;
}

float to_rad(float deg){
    return deg * M_PI / 180;
}

float to_deg(float rad){
    return rad * 180 / M_PI;
}