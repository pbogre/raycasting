#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>

bool is_between(sf::Vector2f line_point1, sf::Vector2f line_point2, sf::Vector2f point){
    if( point.x <= std::max(line_point1.x, line_point2.x) &&
        point.x >= std::min(line_point1.x, line_point2.x) &&
        point.y <= std::max(line_point1.y, line_point2.y) &&
        point.y >= std::min(line_point1.y, line_point2.y)
        ) return true;
    return false;
}

float distance(sf::Vector2f point1, sf::Vector2f point2){
    return sqrt(pow(point2.y - point1.y, 2) + pow(point2.x - point1.x, 2));
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

// This function finds the gradient
// between a ray and the normal of 
// the obstacle it hits. This is called
// the angle of incidence and allows me 
// to simulate reflections & refractions
// https://www.desmos.com/calculator/5s5asfykfh

// PS: Decided not to use this as I won't be
// implementing reflections or refractions, but
// I will keep this here because it works and it 
// is an interesting function.
double incidence_angle(sf::Vector2f line1_point1, sf::Vector2f line1_point2, sf::Vector2f line2_point1, sf::Vector2f line2_point2){
    // Line gradients
    double gradient1  = (line1_point2.y - line1_point1.y) / (line1_point2.x - line1_point1.x);
    double gradient2  = (line2_point2.y - line2_point1.y) / (line2_point2.x - line2_point1.x);
    double normal_gradient = -1 / gradient1;
    // Angle between (normal of line1) and (line2)
    double incidence_angle = atan((normal_gradient - gradient2) / (1 + normal_gradient * gradient2));
    return incidence_angle;
}

