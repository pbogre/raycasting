#include <SFML/Graphics.hpp>

class obstacleMap{
    public:
        std::vector<sf::CircleShape> obstacles;
        std::vector<std::vector<sf::Vertex>> obstacles_points;
        int obstacles_count;
        int sides_count;

        void create_points(){
            for(auto obstacle: obstacles){
                sides_count += obstacle.getPointCount();
            }
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

                obstacles_points.push_back(current_obstacle_points);
            }
        }
};