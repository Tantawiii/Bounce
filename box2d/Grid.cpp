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
    if (texturePaths.size() != 21) {
        std::cerr << "Error: Expected 21 texture paths, got " << texturePaths.size() << std::endl;
        return false;
    }

    for (size_t i = 0; i < texturePaths.size(); ++i) {
        if (!textures[i].loadFromFile(texturePaths[i])) {
            std::cerr << "Error: Could not load texture file: " << texturePaths[i] << std::endl;
            return false;
        }
    }

    charToTextureIndex = {
        {'Z', 0}, {'X', 1}, {'C', 2}, {'S', 3}, // Wall, Ground, Slide, Bouncer
        {'1', 4}, {'2', 5}, {'3', 6}, {'4', 7}, // Sky, Flag, Hole, Plank
        {'5', 8}, {'6', 9}, {'7', 10}, {'8', 11}, // Ring, Size Up, Spike, Star
        {'9', 12}, {'E', 13}, {'Y', 14}, {'T', 15}, // Heart, Coin, Ball
        {'W', 16}, {'U', 17}, {'V', 18}, {'G', 19}, {'H', 20} // Additional tiles
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
            if (gridChar == 'X') {
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

    int startRow = std::max(0, (int)((center.y - size.y / 2) / cellSizeY));
    int endRow = std::min((int)grid.size(), (int)((center.y + size.y / 2) / cellSizeY) + 1);

    int startCol = std::max(0, (int)((center.x - size.x / 2) / cellSizeX));
    int endCol = std::min((int)grid[0].size(), (int)((center.x + size.x / 2) / cellSizeX) + 1);

    for (int row = startRow; row < endRow; ++row) {
        if (row < 0 || row >= grid.size()) continue; // Ensure row index is valid

        for (int col = startCol; col < endCol; ++col) {
            if (col < 0 || col >= grid[row].size()) continue; // Ensure col index is valid

            sf::RectangleShape cell(sf::Vector2f(cellSizeX, cellSizeY)); // Define the size of each cell
            cell.setPosition(col * cellSizeX, row * cellSizeY); // Position the cell on the grid

            char gridChar = grid[row][col];
            if (charToTextureIndex.find(gridChar) != charToTextureIndex.end()) {
                int textureIndex = charToTextureIndex[gridChar];
                if (textureIndex < 0 || textureIndex >= 21) { // Ensure texture index is valid
                    std::cerr << "Error: Invalid texture index " << textureIndex << " for gridChar " << gridChar << std::endl;
                    continue;
                }
                cell.setTexture(&textures[textureIndex]);

                // Correctly scale the texture to fit the grid cell
                sf::Vector2u textureSize = textures[textureIndex].getSize();
                float scaleX = static_cast<float>(cellSizeX) / textureSize.x;
                float scaleY = static_cast<float>(cellSizeY) / textureSize.y;
                cell.setScale(scaleX, scaleY);
            }
            else {
                std::cerr << "Warning: No texture mapping found for gridChar " << gridChar << std::endl;
            }

            window.draw(cell);
        }
    }
}

