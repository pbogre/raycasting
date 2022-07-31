#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>

#include "obstacle.hpp"

bool debug = false;

sf::Vector2f intersection(sf::Vector2f line1_point1, sf::Vector2f line1_point2, sf::Vector2f line2_point1, sf::Vector2f line2_point2);
bool is_between(sf::Vector2f line_point1, sf::Vector2f line_point2, sf::Vector2f point);

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
    sf::CircleShape shape;
    std::vector<obstacle> obstacles;
    obstacleMap obstacleMap;
    
    shape.setRadius(100);
    shape.setPointCount(5);
    shape.setOrigin(shape.getRadius(), shape.getRadius());
    shape.setPosition(500, 300);
    shape.setRotation(60);
    shape.setFillColor(sf::Color::Blue);
    obstacle obstacle1;
        obstacle1.shape = shape;
        obstacle1.type  = 0;
    obstacles.push_back(obstacle1);

    shape.setRadius(160);
    shape.setPointCount(3);
    shape.setOrigin(shape.getRadius(), shape.getRadius());
    shape.setPosition(900, 200);
    shape.setRotation(-45);
    shape.setFillColor(sf::Color::Yellow);
    obstacle obstacle2;
        obstacle2.shape = shape;
        obstacle2.type  = 1;
    obstacles.push_back(obstacle2);

    shape.setRadius(60);
    shape.setPointCount(5);
    shape.setOrigin(shape.getRadius(), shape.getRadius());
    shape.setPosition(450, 600);
    shape.setRotation(30);
    shape.setFillColor(sf::Color::Red);
    obstacle obstacle3;
        obstacle3.shape = shape;
        obstacle3.type  = 0;
    obstacles.push_back(obstacle3);

    obstacleMap.obstacles = obstacles;
    obstacleMap.create_points();

    int selected_obstacle_index = -1;

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

        double mouse_x = sf::Mouse::getPosition(window).x;
        double mouse_y = sf::Mouse::getPosition(window).y;

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
            if(selected_obstacle_index == -1){
                for(int oi = 0; oi < obstacleMap.obstacles_count; oi++){
                    if(obstacles[oi].shape.getGlobalBounds().contains(mouse_x, mouse_y)){
                        selected_obstacle_index = oi;
                        break;
                    }
                }
            }
            else{
                obstacles[selected_obstacle_index].shape.setPosition(mouse_x, mouse_y);
                obstacleMap.obstacles[selected_obstacle_index].shape.setPosition(obstacles[selected_obstacle_index].shape.getPosition());
                obstacleMap.create_points();
            }
        }
        else{
            selected_obstacle_index = -1;
        }

        window.clear();

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
            for(int oi = 0; oi < obstacleMap.obstacles_count; oi++){
                std::vector<sf::Vertex> obstacle_points = obstacleMap.obstacles_points[oi];
                for(int si = 0; si < obstacles[oi].shape.getPointCount(); si++){

                    sf::Vector2f intersection_point = intersection(ray[0].position, ray[1].position, obstacle_points[si].position, obstacle_points[si+1].position);
                    if( is_between(ray[0].position, ray[1].position, intersection_point) &&
                        is_between(obstacle_points[si].position, obstacle_points[si+1].position, intersection_point)) {
                            ray[1].position = intersection_point;
                            if(debug)window.draw(mark_point(intersection_point));
                    }
                }
            }

            // Render current ray
            window.draw(ray);
        }

        // Render obstacles
        if(debug){
            for(int oi = 0; oi < obstacleMap.obstacles_count; oi++){
                for(int si = 0; si < obstacles[oi].shape.getPointCount(); si++){
                    window.draw(mark_line(obstacleMap.obstacles_points[oi][si].position, obstacleMap.obstacles_points[oi][si+1].position, 10000));
                    window.draw(mark_point(obstacleMap.obstacles_points[oi][si].position, sf::Color::Green));
                }
            }
        }
        else{
            for(auto obstacle: obstacles){
                if(obstacle.type == 0) obstacle.shape.setFillColor(sf::Color::Red);
                else if(obstacle.type == 1) obstacle.shape.setFillColor(sf::Color::Blue);
                else if(obstacle.type == 2) obstacle.shape.setFillColor(sf::Color::Yellow);
                window.draw(obstacle.shape);
            }
        }

        window.display();
    }

    return 0;
}

bool is_between(sf::Vector2f line_point1, sf::Vector2f line_point2, sf::Vector2f point){
    if( point.x <= std::max(line_point1.x, line_point2.x) &&
        point.x >= std::min(line_point1.x, line_point2.x) &&
        point.y <= std::max(line_point1.y, line_point2.y) &&
        point.y >= std::min(line_point1.y, line_point2.y)
        ) return true;
    return false;
}

// for an explanation of this function,
// have a look at this desmos i made
// in order to illustrate what's going on
// https://www.desmos.com/calculator/vcnvqt5i3p 
sf::Vector2f intersection(sf::Vector2f line1_point1, sf::Vector2f line1_point2, sf::Vector2f line2_point1, sf::Vector2f line2_point2){
    // y1 = gradient1 * x + intercept1
    double gradient1  = (line1_point2.y - line1_point1.y) / (line1_point2.x - line1_point1.x);
    double intercept1 = line1_point1.y - gradient1 * line1_point1.x;
    // y2 = gradient2 * x + intercept2
    double gradient2  = (line2_point2.y - line2_point1.y) / (line2_point2.x - line2_point1.x);
    double intercept2 = line2_point1.y - gradient2 * line2_point1.x;
    // gradient2 * x + intercept2 = gradient1 * x + intercept2
    // (gradient2 - gradient1) * x = (intercept2 - intercept1)
    // x = (intercept2 - intercept1) / (gradient2 - gradient1)
    double intersection_x = -((intercept2 - intercept1) / (gradient2 - gradient1));
    double intersection_y = gradient2 * intersection_x + intercept2;

    return sf::Vector2f(intersection_x, intersection_y);
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