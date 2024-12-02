#include "box2d/box2d.h"
#include "SFML/Graphics.hpp"
#include <vector>
#include "Ball.h"
#include "Collectible.h"
#include "Water.h"
using namespace sf;

class MyContactListener : public b2ContactListener {
public:
    MyContactListener(Ball& ball, std::vector<Collectible*>& collectibles, std::vector<Collectible*>& toRemove, bool& jumpFlag, Water& water)
        : ball(ball), collectibles(collectibles), toRemove(toRemove), isJumping(jumpFlag), water(water) {
    }

    void BeginContact(b2Contact* contact) override {
        b2Body* bodyA = contact->GetFixtureA()->GetBody();
        b2Body* bodyB = contact->GetFixtureB()->GetBody();

        // Check for ground contact
        if (bodyA == ball.getBody() || bodyB == ball.getBody()) {
            if (bodyA->GetType() == b2_staticBody || bodyB->GetType() == b2_staticBody) {
                isJumping = false;
            }
        }

        if ((bodyA == ball.getBody() && bodyB == water.getBody()) ||
            (bodyB == ball.getBody() && bodyA == water.getBody())) {
            printf("Ball collided with water!\n");
            water.startWaveEffect(); // Trigger wave effect
            ball.startWaveEffect(); // Trigger wave effect
            //ball.getBody()->SetGravityScale(0.0);
        }

        // Check for collectible collision
        for (auto& collectible : collectibles) {
            if (bodyA == collectible->getBody() || bodyB == collectible->getBody()) {
                toRemove.push_back(collectible); // Mark collectible for removal
            }
        }
    }

private:
    Ball& ball;
    std::vector<Collectible*>& collectibles;
    std::vector<Collectible*>& toRemove;
    bool& isJumping;
    Water& water;
};

int main() {
    // SFML window setup
    RenderWindow window(VideoMode(800, 600), "Box2D + SFML Jumping");
    window.setFramerateLimit(60);

    // Box2D world setup
    b2Vec2 gravity(0.0f, 9.8f);  // Gravity vector
    b2World world(gravity);

    // Create a ball object
    Ball ball(&world, 400, 300);

    // Create the water object before passing to MyContactListener
    Water water(&world, 400, 500, 300, 100);

    // Create a ground object
    b2BodyDef groundBodyDef;
    groundBodyDef.type = b2_staticBody;
    groundBodyDef.position.Set(0.0f / Ball::SCALE, 580.0f / Ball::SCALE);
    b2Body* groundBody = world.CreateBody(&groundBodyDef);

    b2PolygonShape groundBox;
    groundBox.SetAsBox(800.0f / Ball::SCALE, 10.0f / Ball::SCALE);
    groundBody->CreateFixture(&groundBox, 0.0f);

    RectangleShape groundRect(Vector2f(800.0f, 20.0f));
    groundRect.setFillColor(Color::Red);
    groundRect.setPosition(0.0f, 580.0f);

    // Create collectibles
    std::vector<Collectible*> collectibles;
    collectibles.push_back(new Collectible(&world, 200, 400));
    collectibles.push_back(new Collectible(&world, 600, 400));

    // List for removal
    std::vector<Collectible*> toRemove;

    // Contact listener to reset jump flag and handle collisions
    bool isJumping = false;
    MyContactListener contactListener(ball, collectibles, toRemove, isJumping, water); // Pass 'water' object here
    world.SetContactListener(&contactListener);

    sf::Clock clock;
    // Main game loop
    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        printf("%f\n", deltaTime);
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Space && !isJumping) {
                isJumping = true;
                ball.jump();
            }
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::D) {
                ball.isMoving = true;
                ball.move(1);
            }
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::A) {
                ball.isMoving = true;
                ball.move(-1);
            }
            if (event.type == Event::KeyReleased && (event.key.code == Keyboard::D || event.key.code == Keyboard::A)) {
                ball.isMoving = false;
            }
            if (event.type == Event::KeyReleased && event.key.code == Keyboard::X && !ball.isMaximized) {
                ball.maximizeSize();
            }
            if (event.type == Event::KeyReleased && event.key.code == Keyboard::N && !ball.isMinimized) {
                ball.minimizeSize();
            }
        }

        // Step the Box2D physics world
        world.Step(1.0f / 60.0f, 6, 2);

        // Update objects
        ball.update(deltaTime);
        water.update(deltaTime);
        for (auto& collectible : collectibles) {
            collectible->update();
        }

        // Remove collected collectibles
        for (auto& collectible : toRemove) {
            collectibles.erase(std::remove(collectibles.begin(), collectibles.end(), collectible), collectibles.end());
            delete collectible; // Free memory
            ball.setScore(ball.getScore() + 1);
        }
        toRemove.clear();

        // Render everything
        window.clear();
        for (auto& collectible : collectibles) {
            collectible->draw(window);
        }
        water.draw(window);
        ball.draw(window);
        window.draw(groundRect);
        window.display();
    }

    // Cleanup collectibles
    for (auto& collectible : collectibles) {
        delete collectible;
    }

    return 0;
}