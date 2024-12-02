#include "Flag.h"
#include <iostream>
#include "Ball.h"

Flag::Flag(b2World* world, float x, float y, float width, float height, const std::string& texturePath)
    : isColliding(false) {
    // Load the texture
    if (!texture.loadFromFile(texturePath)) {
        std::cerr << "Failed to load texture: " << texturePath << std::endl;
    }

    // Set up the sprite
    sprite.setTexture(texture);
    sprite.setOrigin(width / 2, height / 2);
    sprite.setPosition(x, y);

    // Scale the sprite to match the size
    sprite.setScale(width / texture.getSize().x, height / texture.getSize().y);

    // Create Box2D body
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.position.Set(x / Ball::SCALE, y / Ball::SCALE);
    // SCALE is your game scaling factor
    body = world->CreateBody(&bodyDef);
    
    b2PolygonShape boxShape;
    boxShape.SetAsBox(width / 2 / Ball::SCALE, height / 2 / Ball::SCALE);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &boxShape;
    fixtureDef.isSensor = true;  // Flag should act as a sensor
    body->CreateFixture(&fixtureDef);
}

void Flag::update() {
    // Scale the sprite in Y direction when colliding
    if (isColliding) {
        sprite.setScale(sprite.getScale().x, sprite.getScale().y * 0.9f);  // Scale down by 10%
        isColliding = false;  // Reset collision for one-time effect
    }
}

void Flag::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

void Flag::onCollision() {
    isColliding = true;  // Set the flag for collision
}

b2Body* Flag::getBody()
{
    return body;
}
