#pragma once
#include "Inclusion.h"

class Grid
{
private:
    sf::Texture textures[21];
    std::map<char, int> charToTextureIndex;

public:
    void loadGrid(const std::string& path, std::vector<std::vector<char>>& grid, std::vector<Position>& emptyPositions);
    void drawWalls(sf::RenderWindow& window, const std::vector<std::vector<char>>& grid, int cellSizeX, int cellSizeY, sf::View& view);
    bool loadTextures(const std::vector<std::string>& texturePaths);
    void initializePhysics(b2World& world, const std::vector<std::vector<char>>& grid, int cellSizeX, int cellSizeY);
    Texture& getTexture(int index);
};
