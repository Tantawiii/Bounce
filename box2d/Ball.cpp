#include "Ball.h"

Ball::Ball(b2World& world, float x, float y, float radius)
    : isGrounded(true), jumpTime(0.0f), isJumping(false)
{
    // SFML shape setup
    shape.setRadius(radius);
    shape.setFillColor(sf::Color::Red);
    shape.setOrigin(radius, radius); // Center origin
    shape.setPosition(x, y);

    // Box2D body setup
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody; // Dynamic body for the ball
    bodyDef.position.Set(x / 30.f, y / 30.f); // Convert position to Box2D scale
    body = world.CreateBody(&bodyDef);

    b2CircleShape circleShape;
    circleShape.m_radius = radius / 30.f; // Convert radius to Box2D scale

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circleShape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    fixtureDef.restitution = 0.0f; // No bounce to simplify jump mechanics
    body->CreateFixture(&fixtureDef);
}

void Ball::Update(float deltaTime)
{
    // Synchronize SFML shape with Box2D body
    b2Vec2 pos = body->GetPosition();
    shape.setPosition(pos.x * 30.f, pos.y * 30.f);

    // Ground detection: Check if ball is at or near the ground
    if (pos.y >= 700.f / 30.f) // Assuming ground is at y = 700
    {
        isGrounded = true;
        isJumping = false; // Reset jumping state
        jumpTime = 0.0f;   // Reset jump timer
    }
    else
    {
        isGrounded = false;
    }

    // If jumping, apply a smooth jump curve
    if (isJumping)
    {
        jumpTime += deltaTime;
        float jumpForce = -50.0f + 200.0f * jumpTime - 300.0f * jumpTime * jumpTime; // Parabolic jump equation
        if (jumpForce > 0.0f)
        {
            body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x, jumpForce / 30.0f));
        }
        else
        {
            isJumping = false; // End jump when the force is too low
        }
    }
}

void Ball::Draw(sf::RenderWindow& window)
{
    window.draw(shape);
}

void Ball::Jump()
{
    if (isGrounded && !isJumping) // Only jump if on the ground and not already jumping
    {
        isGrounded = false;
        isJumping = true;
        jumpTime = 0.0f; // Reset jump timer
        body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x, -10.0f)); // Initial upward velocity
    }
}
