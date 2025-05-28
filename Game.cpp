#include "Game.hpp"
#include <algorithm>

class Player : public Entity {
private:
    sf::RectangleShape shape;
    sf::Vector2f position;
    bool active;

public:
    Player(float x, float y) : position(x, y), active(true) {
        shape.setSize(sf::Vector2f(20.f, 20.f));
        shape.setFillColor(sf::Color::Green);
        shape.setPosition(position);
    }

    void update(float deltaTime) override {}
    void render(sf::RenderWindow& window) const override {
        if (active) window.draw(shape);
    }
    sf::Vector2f getPosition() const override { return position; }
    void setPosition(const sf::Vector2f& pos) override { position = pos; shape.setPosition(pos); }
    void rotate() override {} // Player doesn't rotate
    bool isActive() const override { return active; }
};

class Mirror : public Entity {
private:
    sf::RectangleShape shape;
    sf::Vector2f position;
    int rotation; // 0: 45°, 1: 135°
    bool active;

public:
    Mirror(float x, float y) : position(x, y), rotation(0), active(true) {
        shape.setSize(sf::Vector2f(20.f, 5.f));
        shape.setFillColor(sf::Color::White);
        shape.setPosition(position);
        shape.setRotation(45.f);
    }

    void update(float deltaTime) override {
        shape.setRotation(rotation == 0 ? 45.f : 135.f);
    }

    void render(sf::RenderWindow& window) const override {
        if (active) window.draw(shape);
    }

    sf::Vector2f getPosition() const override { return position; }
    void setPosition(const sf::Vector2f& pos) override { position = pos; shape.setPosition(pos); }
    void rotate() override { rotation = (rotation + 1) % 2; }
    bool isActive() const override { return active; }
    int getRotation() const { return rotation; }
};

class Portal : public Entity {
private:
    sf::CircleShape shape;
    sf::Vector2f position;
    bool active;

public:
    Portal(float x, float y) : position(x, y), active(false) {
        shape.setRadius(10.f);
        shape.setFillColor(sf::Color::Red);
        shape.setPosition(position);
    }

    void update(float deltaTime) override {}
    void render(sf::RenderWindow& window) const override {
        window.draw(shape);
    }
    sf::Vector2f getPosition() const override { return position; }
    void setPosition(const sf::Vector2f& pos) override { position = pos; shape.setPosition(pos); }
    void rotate() override {} // Portal doesn't rotate
    bool isActive() const override { return active; }
    void setActive(bool state) { active = state; shape.setFillColor(state ? sf::Color::Green : sf::Color::Red); }
};

Game::Game(sf::RenderWindow& win) : window(win), playerPos(1, 1), cursorPos(1, 1), rng(std::random_device{}()), isPortalActive(false) {
    maze.resize(10, std::vector<char>(10, 'W'));
    generateMaze();
    entities.push_back(std::make_unique<Player>(150.f + playerPos.x * 50.f, 100.f + playerPos.y * 50.f));
    entities.push_back(std::make_unique<Portal>(150.f + 8 * 50.f, 100.f + 8 * 50.f));
}

void Game::generateMaze() {
    std::uniform_int_distribution<int> dist(1, 8);
    maze.assign(10, std::vector<char>(10, 'W'));
    maze[1][1] = 'P'; // Player start
    maze[8][8] = 'T'; // Portal
    // Simple recursive backtracking for maze
    std::vector<sf::Vector2i> stack = {{1, 1}};
    while (!stack.empty()) {
        auto [x, y] = stack.back();
        std::vector<sf::Vector2i> neighbors;
        if (x > 2) neighbors.push_back({x - 2, y});
        if (x < 7) neighbors.push_back({x + 2, y});
        if (y > 2) neighbors.push_back({x, y - 2});
        if (y < 7) neighbors.push_back({x, y + 2});
        if (!neighbors.empty()) {
            auto next = neighbors[dist(rng) % neighbors.size()];
            maze[next.y][next.x] = ' ';
            maze[(next.y + y) / 2][(next.x + x) / 2] = ' ';
            stack.push_back(next);
        } else {
            stack.pop_back();
        }
    }
}

void Game::traceLight(sf::Vector2f start, sf::Vector2f direction, std::vector<sf::Vertex>& lightPath) {
    sf::Vector2f pos = start;
    lightPath.push_back(sf::Vertex(pos, sf::Color::Yellow));
    for (int i = 0; i < 20; ++i) { // Limit ray length
        pos += direction * 10.f;
        int gridX = (pos.x - 150.f) / 50.f;
        int gridY = (pos.y - 100.f) / 50.f;
        if (gridX < 0 || gridX >= 10 || gridY < 0 || gridY >= 10 || maze[gridY][gridX] == 'W') break;
        lightPath.push_back(sf::Vertex(pos, sf::Color::Yellow));
        for (const auto& entity : entities) {
            if (dynamic_cast<Mirror*>(entity.get()) && entity->isActive()) {
                sf::Vector2f mirrorPos = entity->getPosition();
                if (std::abs(pos.x - mirrorPos.x) < 10.f && std::abs(pos.y - mirrorPos.y) < 10.f) {
                    int rot = dynamic_cast<Mirror&>(*entity).getRotation();
                    sf::Vector2f newDir = rot == 0 ? sf::Vector2f(-direction.y, -direction.x) : sf::Vector2f(direction.y, direction.x);
                    traceLight(pos, newDir, lightPath);
                    return;
                }
            }
        }
        if (maze[gridY][gridX] == 'T') {
            for (auto& entity : entities) {
                if (dynamic_cast<Portal*>(entity.get()) && std::abs(pos.x - entity->getPosition().x) < 10.f && std::abs(pos.y - entity->getPosition().y) < 10.f) {
                    dynamic_cast<Portal&>(*entity).setActive(true);
                    isPortalActive = true;
                }
            }
        }
    }
}

void Game::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        } else if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::R) {
                maze.clear();
                entities.clear();
                generateMaze();
                entities.push_back(std::make_unique<Player>(150.f + playerPos.x * 50.f, 100.f + playerPos.y * 50.f));
                entities.push_back(std::make_unique<Portal>(150.f + 8 * 50.f, 100.f + 8 * 50.f));
                playerPos = {1, 1};
                cursorPos = {1, 1};
                isPortalActive = false;
            } else if (event.key.code == sf::Keyboard::Escape) {
                window.close();
            } else if (event.key.code == sf::Keyboard::W && playerPos.y > 0 && maze[playerPos.y - 1][playerPos.x] != 'W') {
                playerPos.y--;
                entities[0]->setPosition(sf::Vector2f(150.f + playerPos.x * 50.f, 100.f + playerPos.y * 50.f));
            } else if (event.key.code == sf::Keyboard::S && playerPos.y < 9 && maze[playerPos.y + 1][playerPos.x] != 'W') {
                playerPos.y++;
                entities[0]->setPosition(sf::Vector2f(150.f + playerPos.x * 50.f, 100.f + playerPos.y * 50.f));
            } else if (event.key.code == sf::Keyboard::A && playerPos.x > 0 && maze[playerPos.y][playerPos.x - 1] != 'W') {
                playerPos.x--;
                entities[0]->setPosition(sf::Vector2f(150.f + playerPos.x * 50.f, 100.f + playerPos.y * 50.f));
            } else if (event.key.code == sf::Keyboard::D && playerPos.x < 9 && maze[playerPos.y][playerPos.x + 1] != 'W') {
                playerPos.x++;
                entities[0]->setPosition(sf::Vector2f(150.f + playerPos.x * 50.f, 100.f + playerPos.y * 50.f));
            } else if (event.key.code == sf::Keyboard::Up && cursorPos.y > 0) {
                cursorPos.y--;
            } else if (event.key.code == sf::Keyboard::Down && cursorPos.y < 9) {
                cursorPos.y++;
            } else if (event.key.code == sf::Keyboard::Left && cursorPos.x > 0) {
                cursorPos.x--;
            } else if (event.key.code == sf::Keyboard::Right && cursorPos.x < 9) {
                cursorPos.x++;
            } else if (event.key.code == sf::Keyboard::Space) {
                bool exists = false;
                for (const auto& entity : entities) {
                    if (entity->getPosition() == sf::Vector2f(150.f + cursorPos.x * 50.f, 100.f + cursorPos.y * 50.f)) {
                        entity->rotate();
                        exists = true;
                        break;
                    }
                }
                if (!exists && maze[cursorPos.y][cursorPos.x] != 'W') {
                    entities.push_back(std::make_unique<Mirror>(150.f + cursorPos.x * 50.f, 100.f + cursorPos.y * 50.f));
                }
            }
        }
    }
}

void Game::update(float deltaTime) {
    for (auto& entity : entities) {
        entity->update(deltaTime);
    }
}

void Game::render() {
    window.clear(sf::Color::Black);

    // Draw maze (visible only where light touches)
    std::vector<sf::Vertex> lightPath;
    traceLight(entities[0]->getPosition(), sf::Vector2f(1.f, 0.f), lightPath); // Start light from player, rightward
    std::vector<std::vector<bool>> visible(10, std::vector<bool>(10, false));
    for (const auto& vertex : lightPath) {
        int gridX = (vertex.position.x - 150.f) / 50.f;
        int gridY = (vertex.position.y - 100.f) / 50.f;
        if (gridX >= 0 && gridX < 10 && gridY >= 0 && gridY < 10) {
            visible[gridY][gridX] = true;
        }
    }

    for (int y = 0; y < 10; ++y) {
        for (int x = 0; x < 10; ++x) {
            if (visible[y][x] || (std::abs(x - playerPos.x) <= 1 && std::abs(y - playerPos.y) <= 1)) {
                sf::RectangleShape cell(sf::Vector2f(50.f, 50.f));
                cell.setPosition(150.f + x * 50.f, 100.f + y * 50.f);
                if (maze[y][x] == 'W') {
                    cell.setFillColor(sf::Color::Gray);
                } else {
                    cell.setFillColor(sf::Color(50, 50, 50));
                }
                window.draw(cell);
            }
        }
    }

    // Draw light path
    if (!lightPath.empty()) {
        window.draw(lightPath.data(), lightPath.size(), sf::LinesStrip);
    }

    // Draw cursor
    sf::RectangleShape cursor(sf::Vector2f(50.f, 50.f));
    cursor.setFillColor(sf::Color::Transparent);
    cursor.setOutlineColor(sf::Color::Yellow);
    cursor.setOutlineThickness(2.f);
    cursor.setPosition(150.f + cursorPos.x * 50.f, 100.f + cursorPos.y * 50.f);
    window.draw(cursor);

    // Render entities
    for (const auto& entity : entities) {
        entity->render(window);
    }

    window.display();
}
