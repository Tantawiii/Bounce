#pragma once
#include "box2d/box2d.h"
#include "SFML/Graphics.hpp"

class Ball
{
private:
    sf::CircleShape shape;
    b2Body* body;
    bool isGrounded;
    bool isJumping;
    float jumpTime; // Timer to track jump progression

public:
    Ball(b2World& world, float x, float y, float radius);

    void Update(float deltaTime);
    void Draw(sf::RenderWindow& window);
    void Jump();
};
