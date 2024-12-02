#pragma once

#include <vector>
#include "box2d/box2d.h"
#include "SFML/Graphics.hpp"
using namespace sf;
using namespace std;

class Map
{
public:
    sf::Texture Background, Movingmap;
    sf::Sprite backLayer, mapLayer;

    float backgroundSpeed = 60.f;
    float mapSpeed = 80.f;
    float movDirection = 0.f; // No movement initially

    sf::FloatRect backRec, mapRec; // FloatRects to handle scrolling

    int UploadMaps(sf::RenderWindow& window);
    void HandleInput();
    void MovingPlatform(float deltaTime);
    void Draw(sf::RenderWindow& window);
};
