#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>

bool debug = false;

float to_rad(float deg);
float to_deg(float rad);

sf::Vector2f intersection(sf::Vector2f line1[], sf::Vector2f line2[]);
bool is_between(sf::Vector2f line[], sf::Vector2f point);

sf::CircleShape mark_point(sf::Vector2f point, sf::Color color=sf::Color::Blue);
sf::VertexArray draw_line(sf::Vector2f points[], int extend=0);

int main(int argc, char *argv[])
{
        int frequency = 6;
        if (argc > 1 && strtol(argv[1], NULL, 10) > 0) frequency = strtol(argv[1], NULL, 10);
        int lines_number = (360/frequency);

        sf::ContextSettings settings;
            settings.antialiasingLevel = 4;
        sf::RenderWindow window(sf::VideoMode(1400, 950), "Raycasting", sf::Style::Default, settings);
        window.setFramerateLimit(60);

        sf::RectangleShape line(sf::Vector2f(1, 300));
        sf::RectangleShape wall(sf::Vector2f(25, 300));
            wall.setPosition(850, 300);
            wall.setRotation(60);
            wall.setFillColor(sf::Color::Red);
        sf::Vector2f wall_points[wall.getPointCount()][wall.getPointCount() / 2];
        for (int i = 0; i < wall.getPointCount(); i++){
            wall_points[i][0] = wall.getTransform().transformPoint(wall.getPoint(i));
            wall_points[i][1] = wall.getTransform().transformPoint(wall.getPoint(i+1));
        }

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

                line.setPosition(mouse_x, mouse_y);
                for(int i = 0; i < lines_number; i++){
                    float rotation = i*frequency;
                    line.setRotation(rotation);
                    line.setSize(sf::Vector2f(line.getSize().x, 500));

                    sf::Vector2f line_points[line.getPointCount() / 2];
                        line_points[0] = line.getTransform().transformPoint(line.getPoint(0));
                        line_points[1] = line.getTransform().transformPoint(line.getPoint(3));

                    line.setFillColor(sf::Color::White);
                    for(int i = 0; i < wall.getPointCount(); i++){
                        if(debug) window.draw(draw_line(wall_points[i], 1000));

                        sf::Vector2f intersection_point = intersection(line_points, wall_points[i]);
                        // this is unreadable but its just pythagoras
                        float distance = sqrt(pow(line_points[0].x - intersection_point.x, 2) + pow(line_points[0].y - intersection_point.y, 2));
                        if(distance <= line.getSize().y){
                            if( is_between(line_points, intersection_point) &&
                                is_between(wall_points[i], intersection_point)) {
                                    line.setSize(sf::Vector2f(line.getSize().x, distance));
                            }
                        }

                        if(debug) window.draw(mark_point(intersection_point));
                    }

                    window.draw(line);
                    window.draw(wall);
                    if(debug){
                        for(int i = 0; i < wall.getPointCount(); i++){
                            window.draw(mark_point(wall_points[i][0], sf::Color::Green));
                            window.draw(mark_point(wall_points[i][1], sf::Color::Green));
                        }
                    }
                }

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

bool is_between(sf::Vector2f line[], sf::Vector2f point){
    if( point.x <= std::max(line[0].x, line[1].x) &&
        point.x >= std::min(line[0].x, line[1].x) &&
        point.y <= std::max(line[0].y, line[1].y) &&
        point.y >= std::min(line[0].y, line[1].y)
        ) return true;
    return false;
}

sf::Vector2f intersection(sf::Vector2f line1[], sf::Vector2f line2[]) {
   double a = line1[1].y - line1[0].y;
   double b = line1[0].x - line1[1].x;
   double c = a*(line1[0].x) + b*(line1[0].y);

   double a1 = line2[1].y - line2[0].y;
   double b1 = line2[0].x - line2[1].x;
   double c1 = a1*(line2[0].x)+ b1*(line2[0].y);
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

sf::VertexArray draw_line(sf::Vector2f points[], int extend){
    sf::VertexArray line(sf::Lines, 2);
    float gradient = (points[1].y - points[0].y) / (points[1].x - points[0].x);
    
    float x1 = points[0].x - extend;
    float y1 = points[0].y - (gradient*extend);
    float x2 = points[1].x + extend;
    float y2 = points[1].y + (gradient*extend);
    line[0].position = sf::Vector2f(x1, y1);
    line[1].position = sf::Vector2f(x2, y2);

    line[0].color = sf::Color(100, 100, 100, 100);
    line[1].color = sf::Color(100, 100, 100, 255);
    return line;
}