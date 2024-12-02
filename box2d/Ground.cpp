#include "Ground.h"

Ground::Ground(b2World& world, float x, float y, float width, float height)
{
    // SFML shape setup
    shape.setSize(sf::Vector2f(width, height));
    shape.setFillColor(sf::Color::Green);
    shape.setOrigin(width / 2.f, height / 2.f);
    shape.setPosition(x, y);

    // Box2D body setup
    b2BodyDef bodyDef;
    bodyDef.position.Set(x / 30.f, y / 30.f); // Convert position to Box2D scale
    bodyDef.type = b2_staticBody; // Static body for ground
    b2Body* groundBody = world.CreateBody(&bodyDef);

    b2PolygonShape groundBox;
    groundBox.SetAsBox((width / 2.f) / 30.f, (height / 2.f) / 30.f); // Convert dimensions to Box2D scale

    groundBody->CreateFixture(&groundBox, 0.0f);
}

void Ground::Draw(sf::RenderWindow& window)
{
    window.draw(shape);
}
