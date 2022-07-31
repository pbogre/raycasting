#include <SFML/Graphics.hpp>

struct obstacle{
    sf::CircleShape shape;
    int type = 0;
};

class obstacleMap{
    public:
        std::vector<std::vector<sf::Vertex>> obstacles_points;
        std::vector<obstacle> obstacles;
        int obstacles_count;

        obstacleMap(){
            sf::CircleShape shape;

            create_obstacle(100, 5, sf::Vector2f(500, 300), 60);
            create_obstacle(160, 3, sf::Vector2f(900, 200), -45, 1);
            create_obstacle(60, 7, sf::Vector2f(450, 600), 30);

            create_points();
        }

        void create_obstacle(int radius = 100, int point_count = 4, sf::Vector2f position = sf::Vector2f(100, 100), int rotation = 0, int type = 0){
            sf::CircleShape shape;
                shape.setRadius(radius);
                shape.setOrigin(radius, radius);
                shape.setPointCount(point_count);
                shape.setPosition(position);
                shape.setRotation(rotation);
            obstacle new_obstacle = {shape, type};
            obstacles.push_back(new_obstacle);
        }

        void create_points(){
            std::vector<std::vector<sf::Vertex>> new_obstacles_points;
            obstacles_count = obstacles.size();
            
            for(int oi = 0; oi < obstacles_count; oi++){
                std::vector<sf::Vertex> current_obstacle_points;
                int current_obstacle_sides = obstacles[oi].shape.getPointCount();

                for(int si = 0; si < current_obstacle_sides; si++){
                    sf::Vertex current_vertex;
                    current_vertex.position = obstacles[oi].shape.getTransform().transformPoint(obstacles[oi].shape.getPoint(si));
                    current_obstacle_points.push_back(current_vertex);
                };
                current_obstacle_points.push_back(current_obstacle_points[0]);

                new_obstacles_points.push_back(current_obstacle_points);
            }
            obstacles_points = new_obstacles_points;
        }
};