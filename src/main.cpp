#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>

#include "obstacle.hpp"

bool debug = false;

sf::CircleShape mark_point(sf::Vector2f point, sf::Color color=sf::Color::Blue);
sf::VertexArray mark_line(sf::Vector2f point1, sf::Vector2f point2, int extend=0);

int main(int argc, char *argv[])
{
    // Arguments
    double frequency = 0.036;
    if(debug) frequency = 12;
    if (argc > 1 && strtol(argv[1], NULL, 10) > 0) frequency = strtol(argv[1], NULL, 10);
    int rays_number = (360/frequency);
    
    // Window Setup
    sf::ContextSettings settings;
    sf::RenderWindow window(sf::VideoMode(1400, 950), "Raycasting", sf::Style::Default, settings);
    window.setFramerateLimit(60);

    // Obstacle & Ray Setup
    sf::VertexArray ray(sf::Lines, 2);
    obstacleMap obstacleMap;

    int selected_obstacle_index = -1;

    while (window.isOpen()){

        double mouse_x = sf::Mouse::getPosition(window).x;
        double mouse_y = sf::Mouse::getPosition(window).y;

        sf::Event event;
        while (window.pollEvent(event))
        {
            if(event.type == sf::Event::KeyPressed && (event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::Right )){
                for(int oi = 0; oi < obstacleMap.obstacles_count; oi++){
                    if(obstacleMap.obstacles[oi].getGlobalBounds().contains(mouse_x, mouse_y)){
                        if(event.key.code == sf::Keyboard::Left) obstacleMap.obstacles[oi].rotate(5);
                        else obstacleMap.obstacles[oi].rotate(-5);
                        obstacleMap.create_points();

                        break;
                    }
                }
            }
            else if (event.type == sf::Event::Closed)
                window.close();   
            else if (event.type == sf::Event::Resized){
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
            }
        }

        // Obstacle Drag
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
            if(selected_obstacle_index == -1){
                for(int oi = 0; oi < obstacleMap.obstacles_count; oi++){
                    if(obstacleMap.obstacles[oi].getGlobalBounds().contains(mouse_x, mouse_y)){
                        selected_obstacle_index = oi;
                        break;
                    }
                }
            }
            else{
                obstacleMap.obstacles[selected_obstacle_index].setPosition(mouse_x, mouse_y);
                obstacleMap.create_points();
            }
        }
        else{
            selected_obstacle_index = -1;
        }

        window.clear();

        // Rays
        for(int ri = 0; ri < rays_number; ri++){
            double rotation = (ri*frequency) * M_PI / 180;
            if(rotation == 0 || rotation == M_PI) rotation -= 0.00001; // avoid infinite gradients

            // Set ray endpoints
            ray[0].position = sf::Vector2f(mouse_x, mouse_y);
            double radius = 10000;
            double ray_x = mouse_x - radius * sin(rotation);
            double ray_y = mouse_y + radius * cos(rotation);
            ray[1].position = sf::Vector2f(ray_x, ray_y);

            // Obstacle collision
            sf::Vector2f collision_point = obstacleMap.get_collision(ray);
            ray[1].position = collision_point;

            if(debug) window.draw(mark_point(collision_point, sf::Color(255, 180, 10, 255)));
            window.draw(ray);
        }

        // Render obstacles
        for(int oi = 0; oi < obstacleMap.obstacles_count; oi++){
            if(debug){
                for(int si = 0; si < obstacleMap.obstacles[oi].getPointCount(); si++){
                    window.draw(mark_line(obstacleMap.obstacles_points[oi][si].position, obstacleMap.obstacles_points[oi][si+1].position, 10000));
                    window.draw(mark_point(obstacleMap.obstacles_points[oi][si].position, sf::Color::Green));
                }
            }
            window.draw(&obstacleMap.obstacles_points[oi][0], obstacleMap.obstacles_points[oi].size(), sf::LineStrip);
        }

        window.display();
    }

    return 0;
}


sf::CircleShape mark_point(sf::Vector2f point, sf::Color color){
    int size = 3;
    sf::CircleShape marker(size);
    marker.setFillColor(color);
    marker.setPosition(sf::Vector2f(point.x - size, point.y - size));
    return marker;
}

sf::VertexArray mark_line(sf::Vector2f point1, sf::Vector2f point2, int extend){
    sf::VertexArray line(sf::Lines, 2);
    double gradient = (point2.y - point1.y) / (point2.x - point1.x);
    
    double x1 = point1.x - extend;
    double y1 = point1.y - (gradient*extend);
    double x2 = point2.x + extend;
    double y2 = point2.y + (gradient*extend);
    line[0].position = sf::Vector2f(x1, y1);
    line[1].position = sf::Vector2f(x2, y2);

    line[0].color = sf::Color(100, 100, 100, 100);
    line[1].color = sf::Color(100, 100, 100, 100);
    return line;
}