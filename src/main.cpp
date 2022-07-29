#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>

bool debug = false;

sf::Vector2f intersection(sf::Vector2f line1_point1, sf::Vector2f line1_point2, sf::Vector2f line2_point1, sf::Vector2f line2_point2);
bool is_between(sf::Vector2f line_point1, sf::Vector2f line_point2, sf::Vector2f point);

sf::CircleShape mark_point(sf::Vector2f point, sf::Color color=sf::Color::Blue);
sf::VertexArray mark_line(sf::Vector2f point1, sf::Vector2f point2, int extend=0);

int main(int argc, char *argv[])
{
    // Arguments
    double frequency = 0.036;
    if (argc > 1 && strtol(argv[1], NULL, 10) > 0) frequency = strtol(argv[1], NULL, 10);
    int rays_number = (360/frequency);
    
    // Window Setup
    sf::ContextSettings settings;
        settings.antialiasingLevel = 4;
    sf::RenderWindow window(sf::VideoMode(1400, 950), "Raycasting", sf::Style::Default, settings);
    window.setFramerateLimit(60);

    // Shape, Hitbox, Ray Setup
    sf::VertexArray ray(sf::Lines, 2);
    
    sf::CircleShape shape(100, 5);
        shape.setPosition(500, 300);
        shape.setRotation(60);
        shape.setFillColor(sf::Color::Blue);
    int shape_sides = shape.getPointCount();

    sf::VertexArray obstacle(sf::Lines, shape_sides+1);
    for (int i = 0; i < shape_sides; i++){
        obstacle[i].position = shape.getTransform().transformPoint(shape.getPoint(i));
        obstacle[i].color = sf::Color::Yellow;
    }
    obstacle[shape_sides].position = obstacle[0].position;


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

        double mouse_x = sf::Mouse::getPosition(window).x;
        double mouse_y = sf::Mouse::getPosition(window).y;

        for(int li = 0; li < rays_number; li++){
            double rotation = (li*frequency) * M_PI / 180;
            if(rotation == 0 || rotation == M_PI) rotation -= 0.0001; // avoid infinite gradients

            // Set ray endpoints
            ray[0].position = sf::Vector2f(mouse_x, mouse_y);
            double radius = 10000;
            double ray_x = mouse_x - radius * sin(rotation);
            double ray_y = mouse_y + radius * cos(rotation);
            ray[1].position = sf::Vector2f(ray_x, ray_y);

            // Wall collision
            for(int i = 0; i < shape_sides; i++){
                sf::Vector2f intersection_point = intersection(ray[0].position, ray[1].position, obstacle[i].position, obstacle[i+1].position);
                //window.draw(mark_point(intersection_point));
                if( is_between(ray[0].position, ray[1].position, intersection_point) &&
                    is_between(obstacle[i].position, obstacle[i+1].position, intersection_point)) {
                        ray[1].position = intersection_point;
                }
            }

            // Render current ray
            window.draw(ray);
        }

        // Render shapes
        window.draw(shape);
        if(debug){
            for(int i = 0; i < shape_sides; i++){
                window.draw(mark_line(obstacle[i].position, obstacle[i+1].position, 10000));
                window.draw(mark_point(obstacle[i].position, sf::Color::Green));
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
    //std::cout << "y=" << gradient1 << "x + " << intercept1 << "(" << intersection_x << ", " << intersection_y << ")" << std::endl;

    return sf::Vector2f(intersection_x, intersection_y);
}

sf::CircleShape mark_point(sf::Vector2f point, sf::Color color){
    int size = 5;
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
    line[1].color = sf::Color(100, 100, 100, 255);
    return line;
}
