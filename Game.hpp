#ifndef GAME_HPP
#define GAME_HPP
#include "Entity.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <random>

class Game {
private:
    sf::RenderWindow& window;
    std::vector<std::vector<char>> maze; // 'W'=wall, ' '=path, 'P'=player, 'T'=portal
    std::vector<std::unique_ptr<Entity>> entities; // Mirrors, player, portal
    sf::Vector2i playerPos;
    sf::Vector2i cursorPos;
    std::mt19937 rng;
    bool isPortalActive;

    void generateMaze();
    void traceLight(sf::Vector2f start, sf::Vector2f direction, std::vector<sf::Vertex>& lightPath);

public:
    Game(sf::RenderWindow& win);
    void handleEvents();
    void update(float deltaTime);
    void render();
};

#endif // GAME_HPP
