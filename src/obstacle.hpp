#pragma once

#include <SFML/Graphics.hpp>
#include "core.hpp"

class obstacleMap{
    public:
        std::vector<std::vector<sf::Vertex>> obstacles_points;
        std::vector<sf::CircleShape> obstacles;
        int obstacles_count;

        obstacleMap(){
            sf::CircleShape shape;

            create_obstacle(200, sf::Vector2f(300, 300), 4, 45);
            create_obstacle(80, sf::Vector2f(600, 600), 5);
            create_obstacle(80, sf::Vector2f(800, 600), 6);
            create_obstacle(80, sf::Vector2f(1000, 600), 7);
            create_obstacle(300, sf::Vector2f(800, 140), 2, 90);
            create_obstacle(300, sf::Vector2f(800, 150), 2, 90);
            create_obstacle(300, sf::Vector2f(800, 160), 2, 90);
            create_obstacle(300, sf::Vector2f(800, 170), 2, 90);

            create_points();
        }

        void create_obstacle(int radius = 100, sf::Vector2f position = sf::Vector2f(100, 100), int point_count = 4, int rotation = 0){ 
            sf::CircleShape shape;
                shape.setRadius(radius);
                shape.setOrigin(radius, radius);
                shape.setPointCount(point_count);
                shape.setPosition(position);
                shape.setRotation(rotation);
            obstacles.push_back(shape);
        }

        void create_points(){
            std::vector<std::vector<sf::Vertex>> new_obstacles_points;
            obstacles_count = obstacles.size();
            
            for(int oi = 0; oi < obstacles_count; oi++){
                std::vector<sf::Vertex> current_obstacle_points;

                // Create points vector
                for(int si = 0; si < obstacles[oi].getPointCount(); si++){
                    sf::Vertex current_vertex;
                        current_vertex.color = sf::Color::Red;
                        current_vertex.position = obstacles[oi].getTransform().transformPoint(obstacles[oi].getPoint(si));
                    current_obstacle_points.push_back(current_vertex);
                };
                current_obstacle_points.push_back(current_obstacle_points[0]);

                // Check for infinite gradients (change in x = 0)
                for(int si = 0; si < obstacles[oi].getPointCount(); si++){
                    double dx = current_obstacle_points[si].position.x - current_obstacle_points[si+1].position.x;
                    if (dx == 0) {
                        current_obstacle_points[si].position.x += 0.0001;
                        current_obstacle_points[si+1].position.x -= 0.0001;
                    }
                }

                new_obstacles_points.push_back(current_obstacle_points);
            }
            obstacles_points = new_obstacles_points;
        }

        sf::Vector2f get_collision(sf::VertexArray ray){
            sf::Vector2f nearest_intersection = ray[1].position;
            for(int oi = 0; oi < obstacles_count; oi++){
                for(int si = 0; si < obstacles[oi].getPointCount(); si++){

                    sf::Vector2f intersection_point = intersection(ray[0].position, ray[1].position, obstacles_points[oi][si].position, obstacles_points[oi][si+1].position);
                    if( is_between(ray[0].position, ray[1].position, intersection_point) &&
                        is_between(obstacles_points[oi][si].position, obstacles_points[oi][si+1].position, intersection_point) &&
                        distance(ray[0].position, intersection_point) < distance(ray[0].position, nearest_intersection)){
                            nearest_intersection = intersection_point;
                    }
                }
            }

            return nearest_intersection;
        }
};