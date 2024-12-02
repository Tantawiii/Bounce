#include "Ball.h"
Ball::Ball(b2World* world, float x, float y)
{
    isNormal = true;
    // Create a Box2D body for the ball
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(x / SCALE, y / SCALE);  // Convert from SFML units to Box2D units
    body = world->CreateBody(&bodyDef);

    // Create a circular shape for the ball
    b2CircleShape circleShape;
    circleShape.m_radius = 20.0f / SCALE;  // Convert radius to Box2D units

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circleShape;
    fixtureDef.density = 1.0f;   // Mass of the ball
    fixtureDef.friction = 0.3f;  // Friction coefficient
    fixtureDef.restitution = 0.6f;  // Bounciness
    body->CreateFixture(&fixtureDef);

    // Set SFML shape properties
    shape.setRadius(20.0f);
    shape.setFillColor(sf::Color::Blue);
    shape.setOrigin(20.0f, 20.0f);// Center origin

    waveEffectActive = false;
    waveTime = 0.0f;
}
const float Ball::SCALE = 30.0f;
void Ball::update(float deltaTime) {
    sf::Vector2f position(body->GetPosition().x * SCALE, body->GetPosition().y * SCALE);
    shape.setPosition(position);

    //printf("body %d%d\n", body->GetPosition().x, body->GetPosition().y);
    //printf("shape %d%d\n", shape.getPosition().x, shape.getPosition().y);
    printf("score %d\n", score);
    if (waveEffectActive) {
        waveTime += deltaTime;



        if (waveTime >= 4.0f) {  // Effect lasts for 2 seconds
            waveEffectActive = false;
            shape.setScale(1.0f, 1.0f);  // Reset scale
            body->SetLinearVelocity(b2Vec2(0.0, 0));
        }
        else if (waveTime >= .5f) {  // Effect lasts for 2 seconds
            // Apply some "wave" animation to the ball (e.g., scaling effect)
            float scale = 1.0f + .1f * sin(waveTime * 5); // Wave effect scale
            //shape.setScale(scale, scale);
            body->SetLinearVelocity(b2Vec2(0.0, -scale));
        }
    }

}
void Ball::startWaveEffect() {
    // Trigger the wave effect when ball collides with water
    waveEffectActive = true;
    waveTime = 0.0f; // Reset the wave effect time
}
void Ball::draw(sf::RenderWindow& window) {
    window.draw(shape); // Directly draw the shape
}

void Ball::jump()
{

    b2Vec2 impulse;

    if (isMaximized) {
        impulse.Set(0.0f, -45.0f);
    }
    else if (isNormal) {
        impulse.Set(0.0f, -10.0f);
    }
    else if (isMinimized) {
        impulse.Set(0.0f, -2.0f);
    }


    body->ApplyLinearImpulseToCenter(impulse, true);
}

b2Body* Ball::getBody()
{
    return body;
}

void Ball::maximizeSize()
{
    shape.setRadius(shape.getRadius() * 2.0f);
    shape.setOrigin(shape.getRadius(), shape.getRadius());

    body->DestroyFixture(body->GetFixtureList());
    b2CircleShape newCircleShape;
    newCircleShape.m_radius = (shape.getRadius()) / SCALE;
    b2FixtureDef newFixtureDef;
    newFixtureDef.shape = &newCircleShape;
    newFixtureDef.density = 1.0f;
    newFixtureDef.friction = 0.3f;
    newFixtureDef.restitution = 0.6f;
    body->CreateFixture(&newFixtureDef);
    isMaximized = true;
    isMinimized = false;
    isNormal = false;
}

void Ball::minimizeSize()
{
    shape.setRadius(shape.getRadius() * 0.5f);
    shape.setOrigin(shape.getRadius(), shape.getRadius());

    body->DestroyFixture(body->GetFixtureList());
    b2CircleShape newCircleShape;
    newCircleShape.m_radius = (shape.getRadius()) / SCALE;
    b2FixtureDef newFixtureDef;
    newFixtureDef.shape = &newCircleShape;
    newFixtureDef.density = 1.0f;
    newFixtureDef.friction = 0.3f;
    newFixtureDef.restitution = 0.6f;
    body->CreateFixture(&newFixtureDef);
    isMinimized = true;
    isMaximized = false;
    isNormal = false;
}

void Ball::move(float direction)
{
    if (isMoving) {
        b2Vec2 force(10.0f * direction, 0.0f); // Force vector: positive x-direction
        body->ApplyForceToCenter(force, true);
    }
    else {
        body->ApplyForceToCenter(b2Vec2(0.0, 0.0), true);
    }

}

void Ball::setScore(int s)
{
    if (s < 0)s = 0;
    score = s;
}

int Ball::getScore()
{
    return score;
}
