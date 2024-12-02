#pragma once
#include <SFML/Graphics.hpp>
#include "box2d/box2d.h"

class Ground
{
public:
    sf::RectangleShape shape; // Visual representation of the ground
    Ground(b2World& world, float x, float y, float width, float height);
    void Draw(sf::RenderWindow& window);
};
