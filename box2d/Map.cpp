#include "Map.h"
#include <iostream>

int Map::UploadMaps(sf::RenderWindow& window)
{
    // Load textures
    if (!Background.loadFromFile("Background.png") ||
        !Movingmap.loadFromFile("Movingmap.png")) {
        std::cerr << "Error: Failed to load textures!" << std::endl;
        return -1; // Return error if textures can't be loaded
    }

    // Set textures to repeat
    Background.setRepeated(true);
    Movingmap.setRepeated(true);

    // Set up FloatRects to control scrolling regions
    backRec = sf::FloatRect(0.f, 0.f, window.getSize().x * 2.f, window.getSize().y);
    mapRec = sf::FloatRect(0.f, 0.f, window.getSize().x * 2.f, window.getSize().y);

    // Create sprites for layers
    backLayer.setTexture(Background);
    mapLayer.setTexture(Movingmap);

    return 0; // Success
}

void Map::HandleInput()
{
    // Check for left or right arrow keys
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        movDirection = 1.f; // Move right
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        movDirection = -1.f; // Move left
    }
    else
    {
        movDirection = 0.f; // Stop moving
    }
}

void Map::MovingPlatform(float deltaTime)
{
    // Adjust FloatRects for scrolling based on direction
    backRec.left += backgroundSpeed * deltaTime * movDirection;
    mapRec.left += mapSpeed * deltaTime * movDirection;

    // Convert FloatRects to IntRects and apply to sprites
    backLayer.setTextureRect(sf::IntRect(static_cast<int>(backRec.left), 0, backRec.width, backRec.height));
    mapLayer.setTextureRect(sf::IntRect(static_cast<int>(mapRec.left), 0, mapRec.width, mapRec.height));
}

void Map::Draw(sf::RenderWindow& window)
{
    // Draw layers in correct order
    window.draw(backLayer);
    window.draw(mapLayer);
}
