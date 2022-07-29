#include <SFML/Graphics.hpp>

class obstacleMap{
    public:
        std::vector<std::vector<sf::Vertex>> obstacles_points;
        std::vector<sf::CircleShape> obstacles;
        int obstacles_count;

        void create_points(){
            std::vector<std::vector<sf::Vertex>> new_obstacles_points;
            obstacles_count = obstacles.size();
            
            for(int oi = 0; oi < obstacles_count; oi++){
                std::vector<sf::Vertex> current_obstacle_points;
                int current_obstacle_sides = obstacles[oi].getPointCount();

                for(int si = 0; si < current_obstacle_sides; si++){
                    sf::Vertex current_vertex;
                    current_vertex.position = obstacles[oi].getTransform().transformPoint(obstacles[oi].getPoint(si));
                    current_obstacle_points.push_back(current_vertex);
                };
                current_obstacle_points.push_back(current_obstacle_points[0]);

                new_obstacles_points.push_back(current_obstacle_points);
            }
            obstacles_points = new_obstacles_points;
        }
};