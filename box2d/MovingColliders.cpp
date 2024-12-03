#include "MovingColliders.h"
#include "Ball.h"

MovingColliders::MovingColliders(b2World& world, sf::Vector2f startPosition, sf::Vector2f range, float speed, bool horizontal, bool vertical)
    : startPosition(startPosition),
    endPosition(startPosition + range),
    speed(speed),
    isMovingForward(horizontal),
    isMovingVertical(vertical) {
    // Define the body
    b2BodyDef bodyDef;
    bodyDef.type = b2_kinematicBody; // Kinematic for controlled movement
    bodyDef.position.Set(startPosition.x, startPosition.y);
    movObs = world.CreateBody(&bodyDef);

    // Define the shape
    b2PolygonShape boxShape;
    boxShape.SetAsBox(1.0f, 1.0f); // Collider dimensions

    // Define the fixture
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &boxShape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;

    movObs->CreateFixture(&fixtureDef);
}

void MovingColliders::updateColliderX(float deltaTime) {
    b2Vec2 position = movObs->GetPosition();

    if (isMovingForward) {
        position.x += speed * deltaTime;
        if (position.x >= endPosition.x) {
            position.x = endPosition.x;
            isMovingForward = false;
        }
    }
    else {
        position.x -= speed * deltaTime;
        if (position.x <= startPosition.x) {
            position.x = startPosition.x;
            isMovingForward = true;
        }
    }

    movObs->SetTransform(position, movObs->GetAngle());
}

void MovingColliders::updateColliderY(float deltaTime) {
    b2Vec2 position = movObs->GetPosition();

    if (isMovingForward) {
        position.y += speed * deltaTime;
        if (position.y >= endPosition.y) {
            position.y = endPosition.y;
            isMovingForward = false;
        }
    }
    else {
        position.y -= speed * deltaTime;
        if (position.y <= startPosition.y) {
            position.y = startPosition.y;
            isMovingForward = true;
        }
    }

    movObs->SetTransform(position, movObs->GetAngle());
}

void MovingColliders::renderCollider(sf::RenderWindow& window) {
    b2Vec2 position = movObs->GetPosition();
    float angle = movObs->GetAngle();

    sf::RectangleShape movObsRec(sf::Vector2f(20.0f, 20.0f));
    movObsRec.setOrigin(10.0f, 10.0f);
    movObsRec.setPosition(position.x * Ball::SCALE, position.y * Ball::SCALE);
    movObsRec.setRotation(angle * 180 / 3.14f);
    movObsRec.setFillColor(sf::Color::Magenta);

    window.draw(movObsRec);
}
