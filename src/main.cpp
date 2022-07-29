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
    float frequency = 0.036;
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

        float mouse_x = sf::Mouse::getPosition(window).x;
        float mouse_y = sf::Mouse::getPosition(window).y;

        for(int li = 0; li < rays_number; li++){
            float rotation = (li*frequency) * M_PI / 180;

            // Set ray endpoints
            ray[0].position = sf::Vector2f(mouse_x, mouse_y);
            float radius = 10000;
            float ray_x = mouse_x - radius * sin(rotation);
            float ray_y = mouse_y + radius * cos(rotation);
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

sf::Vector2f intersection(sf::Vector2f line1_point1, sf::Vector2f line1_point2, 
                          sf::Vector2f line2_point1, sf::Vector2f line2_point2) {
   double a = line1_point2.y - line1_point1.y;
   double b = line1_point1.x - line1_point2.x;
   double c = a*(line1_point1.x) + b*(line1_point1.y);

   double a1 = line2_point2.y - line2_point1.y;
   double b1 = line2_point1.x - line2_point2.x;
   double c1 = a1*(line2_point1.x)+ b1*(line2_point1.y);
   double det = a*b1 - a1*b;
   if (det == 0) {
      return sf::Vector2f(9999999, 9999999);
   } else {
      double x = (b1*c - b*c1)/det;
      double y = (a*c1 - a1*c)/det;
      return sf::Vector2f(x, y);
   }
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
    float gradient = (point2.y - point1.y) / (point2.x - point1.x);
    
    float x1 = point1.x - extend;
    float y1 = point1.y - (gradient*extend);
    float x2 = point2.x + extend;
    float y2 = point2.y + (gradient*extend);
    line[0].position = sf::Vector2f(x1, y1);
    line[1].position = sf::Vector2f(x2, y2);

    line[0].color = sf::Color(100, 100, 100, 100);
    line[1].color = sf::Color(100, 100, 100, 255);
    return line;
}