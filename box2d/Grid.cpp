#include "Grid.h"

void Grid::loadGrid(const std::string& path, std::vector<std::vector<char>>& grid, std::vector<Position>& emptyPositions) {
    std::ifstream myfile(path, std::ios_base::in);

    if (!myfile.is_open()) {
        std::cerr << "Error: Could not open file." << std::endl;
        return;
    }

    std::string line;
    while (std::getline(myfile, line)) {
        std::vector<char> row(line.begin(), line.end());
        grid.push_back(row);
    }

    myfile.close();
}

bool Grid::loadTextures(const std::vector<std::string>& texturePaths) {
    if (texturePaths.size() != 24) {
        std::cerr << "Error: Expected 24 texture paths, got " << texturePaths.size() << std::endl;
        return false;
    }

    for (size_t i = 0; i < texturePaths.size(); ++i) {
        if (!textures[i].loadFromFile(texturePaths[i])) {
            std::cerr << "Error: Could not load texture file: " << texturePaths[i] << std::endl;
            return false;
        }
    }

    charToTextureIndex = {
        {'X', 0}, {'Z', 1}, {'C', 2}, {'S', 3}, // Wall, Ground, Slide, Bouncer
        {'1', 4}, {'2', 5}, {'3', 6}, {'4', 7}, // Sky, Flag, Hole, Plank
        {'5', 8}, {'6', 9}, {'7', 10}, {'8', 11}, // Ring, Size Up, Spike, Star
        {'9', 12}, {'E', 13}, {'Y', 14}, {'T', 15}, // Heart, Coin, Ball
        {'W', 16}, {'U', 17}, {'V', 18}, {'G', 19}, {'H', 20}, // Additional tiles
        {'P', 21 }, {'I', 22}, {'O', 23}
    };

    return true;
}

sf::Texture& Grid::getTexture(int index) {
    return textures[index];
}

void Grid::initializePhysics(b2World& world, const std::vector<std::vector<char>>& grid, int cellSizeX, int cellSizeY) {
    for (size_t row = 0; row < grid.size(); ++row) {
        for (size_t col = 0; col < grid[row].size(); ++col) {
            char gridChar = grid[row][col];
            if (gridChar == 'X' || gridChar == 'C') {
                // Create static Box2D body for ground/walls
                b2BodyDef bodyDef;
                bodyDef.position.Set(col * (float)cellSizeX / SCALE, row * (float)cellSizeY / SCALE);
                bodyDef.type = b2_staticBody;

                b2PolygonShape groundShape;
                groundShape.SetAsBox(0.5f * cellSizeX / SCALE, 0.5f * cellSizeY / SCALE);

                b2Body* groundBody = world.CreateBody(&bodyDef);
                groundBody->CreateFixture(&groundShape, 0.0f);
            }
        }
    }
}

void Grid::drawWalls(sf::RenderWindow& window, const std::vector<std::vector<char>>& grid, int cellSizeX, int cellSizeY, sf::View& view) {
    sf::Vector2f center = view.getCenter();
    sf::Vector2f size = view.getSize();

    int startRow = std::max(0, static_cast<int>((center.y - size.y / 2) / cellSizeY));
    int endRow = std::min(static_cast<int>(grid.size()), static_cast<int>((center.y + size.y / 2) / cellSizeY) + 1);
    int startCol = std::max(0, static_cast<int>((center.x - size.x / 2) / cellSizeX));
    int endCol = std::min(static_cast<int>(grid[0].size()), static_cast<int>((center.x + size.x / 2) / cellSizeX) + 1);

    for (int row = startRow; row < endRow; ++row) {
        for (int col = startCol; col < endCol; ++col) {
            char gridChar = grid[row][col];
            if (charToTextureIndex.find(gridChar) != charToTextureIndex.end()) {
                int textureIndex = charToTextureIndex[gridChar];
                //std::cout << "Rendering char '" << gridChar << "' at (" << row << ", " << col << ") with texture index " << textureIndex << std::endl;

                if (textureIndex >= 0 && textureIndex < 24) {
                    sf::RectangleShape cell(sf::Vector2f(cellSizeX, cellSizeY));
                    cell.setPosition(col * cellSizeX, row * cellSizeY);

                    if (gridChar == 'O') {
                        cell.setTexture(&textures[textureIndex]);
                        //std::cout << "Rendering 'O' texture.\n";
                        window.draw(cell);
                    }
                    else if (gridChar == '1') {
                        cell.setTexture(&textures[textureIndex]);
                        cell.setFillColor(sf::Color(255, 255, 255, 128)); // Semi-transparent sky
                        //std::cout << "Rendering '1' (sky) texture.\n";
                        window.draw(cell);
                    }
                    else {
                        //cell.setTexture(&textures[textureIndex]);
                        //cell.setTextureRect(sf::IntRect(0, 0, cellSizeX, cellSizeY));
                        float scaleFactor = std::min(
                            static_cast<float>(cellSizeX) / textures[textureIndex].getSize().x,
                            static_cast<float>(cellSizeY) / textures[textureIndex].getSize().y
                        );
                        cell.setTexture(&textures[textureIndex]);
                        cell.setScale(scaleFactor, scaleFactor);
                        window.draw(cell);
                    }
                }
                //else {
                //    std::cerr << "Invalid texture index " << textureIndex << " for char '" << gridChar << "'\n";
                //}
            }
            else {
                //std::cerr << "Unhandled gridChar '" << gridChar << "' at (" << row << ", " << col << ")\n";
            }
        }
    }
}

void Grid::switchView(sf::RenderWindow& window, sf::View& view, int scene, const b2Vec2& playerCircle) {
    sf::Vector2f playerPosition = Vector2f(playerCircle.x , playerCircle.y);

    switch (scene) {
    case 0: // Full game view
        view.setCenter(WINDOW_WIDTH, WINDOW_HEIGHT);
        view.setSize(WINDOW_WIDTH, WINDOW_HEIGHT);
        break;
    
    case 1: // Vertical split
        if (playerPosition.x < WINDOW_WIDTH / 2) {
            view.setCenter(WINDOW_WIDTH / 4, WINDOW_HEIGHT / 2);
        }
        else {
            view.setCenter(3 * WINDOW_WIDTH / 4, WINDOW_HEIGHT / 2);
        }
        view.setSize(WINDOW_WIDTH / 2, WINDOW_HEIGHT);
        break;

    case 2: // Four quadrants
        if (playerPosition.x < WINDOW_WIDTH / 2) {
            if (playerPosition.y < WINDOW_HEIGHT / 2) {
                view.setCenter(WINDOW_WIDTH / 4, WINDOW_HEIGHT / 4); // Top-left quadrant
            }
            else {
                view.setCenter(WINDOW_WIDTH / 4, 3 * WINDOW_HEIGHT / 4); // Bottom-left quadrant
            }
        }
        else {
            if (playerPosition.y < WINDOW_HEIGHT / 2) {
                view.setCenter(3 * WINDOW_WIDTH / 4, WINDOW_HEIGHT / 4); // Top-right quadrant
            }
            else {
                view.setCenter(3 * WINDOW_WIDTH / 4, 3 * WINDOW_HEIGHT / 4); // Bottom-right quadrant
            }
        }
        view.setSize(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
        break;

    case 3: // Player-focused view
        view.setCenter(playerPosition.x, playerPosition.y);
        view.setSize(player_Scene_WIDTH * SCALE, player_Scene_HEIGHT * SCALE);
        break;

    case 4:
        view.setCenter(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
        view.setSize(WINDOW_WIDTH, WINDOW_HEIGHT);
        break;
    default:
        std::cerr << "Invalid scene index." << std::endl;
        break;
    }

    window.setView(view);
}