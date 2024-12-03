#include "box2d/box2d.h"
#include "SFML/Graphics.hpp"
#include <vector>
#include "Ball.h"
#include "Collectible.h"
#include "Water.h"
#include "Flag.h"
#include "MainMenu.h"
using namespace sf;
float ballGravity;
class MyContactListener : public b2ContactListener {
public:
    MyContactListener(Ball& ball, std::vector<Collectible*>& collectibles, std::vector<Collectible*>& toRemove, bool& jumpFlag, Water& water, Flag& flag)
        : ball(ball), collectibles(collectibles), toRemove(toRemove), isJumping(jumpFlag), water(water), flag(flag) {
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

        // Ball collides with water
        if ((bodyA == ball.getBody() && bodyB == water.getBody()) ||
            (bodyB == ball.getBody() && bodyA == water.getBody())) {
            printf("Ball collided with water!\n");
            if (ball.isMaximized) {
                ball.startWaveEffect(); // Trigger wave effect
                ballGravity = ball.getBody()->GetGravityScale();
                ball.getBody()->SetGravityScale(0.0);

            }
            water.startWaveEffect(); // Trigger wave effect
        }

        // Ball collides with flag
        if ((bodyA == ball.getBody() && bodyB == flag.getBody()) ||
            (bodyB == ball.getBody() && bodyA == flag.getBody())) {
            printf("Winner!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
            printf("Winner!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
            printf("Winner!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
            printf("Winner!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
            printf("Winner!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
            flag.onCollision();

        }

        // Check for collectible collision
        for (auto& collectible : collectibles) {
            if (bodyA == collectible->getBody() || bodyB == collectible->getBody()) {
                toRemove.push_back(collectible); // Mark collectible for removal
            }
        }
    }

    void EndContact(b2Contact* contact) override {
        b2Body* bodyA = contact->GetFixtureA()->GetBody();
        b2Body* bodyB = contact->GetFixtureB()->GetBody();

        // Ball exits water
        if ((bodyA == ball.getBody() && bodyB == water.getBody()) ||
            (bodyB == ball.getBody() && bodyA == water.getBody())) {
            printf("Ball exited the water!\n");
            ball.getBody()->SetGravityScale(ballGravity);
        }
    }

private:
    Ball& ball;
    std::vector<Collectible*>& collectibles;
    std::vector<Collectible*>& toRemove;
    bool& isJumping;
    Water& water;
    Flag& flag;
};
MenuOption mainMenu() {
    const float windowWidth = 800.0f;
    const float windowHeight = 600.0f;

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Main Menu");

    MainMenu menu(windowWidth, windowHeight);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // Handle menu interactions
        MenuOption selectedOption = menu.handleInput(window);
        if (selectedOption == MenuOption::Play) {
            return MenuOption::Play;
            std::cout << "Play selected!" << std::endl;
            // Transition to the play state
        }
        else if (selectedOption == MenuOption::Levels) {
            return MenuOption::Levels;
            std::cout << "Levels selected!" << std::endl;
            // Transition to the levels selection state
        }
        else if (selectedOption == MenuOption::Exit) {
            return MenuOption::Exit;
            std::cout << "Exit selected!" << std::endl;
            window.close();
        }

        // Render the menu
        window.clear();
        menu.draw(window);
        window.display();
    }
    return  MenuOption::None;
}
void Level1() {
    RenderWindow window(VideoMode(800, 600), "Bounce");
    window.setFramerateLimit(60);

    // Box2D world setup
    b2Vec2 gravity(0.0f, 9.8f);  // Gravity vector
    b2World world(gravity);

    // Create a ball object
    Ball ball(&world, 680, 100);
    Flag flag(&world, 850.0f, 630.0f, 100.0f, 100.0f, "C:/Users/merot/OneDrive/Documents/GitHub/Bounce/box2d/assets/Images/flag.png");

    // Create the water object before passing to MyContactListener
    Water water(&world, 400, 500, 300, 500);

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
    MyContactListener contactListener(ball, collectibles, toRemove, isJumping, water, flag);
    // Pass 'water' object here
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

                if (ball.isNormal)
                    ball.maximizeSize();
                else {
                    ball.maximizeSize();
                    ball.isNormal = true;
                    ball.isMaximized = false;
                    ball.isMinimized = false;
                }
            }
            if (event.type == Event::KeyReleased && event.key.code == Keyboard::N && !ball.isMinimized) {
                if (ball.isNormal)
                    ball.minimizeSize();
                else {
                    ball.minimizeSize();
                    ball.isNormal = true;
                    ball.isMinimized = false;
                    ball.isMaximized = false;
                }
            }
        }

        // Step the Box2D physics world
        world.Step(1.0f / 60.0f, 6, 2);

        // Update objects
        ball.update(deltaTime);
        water.update(deltaTime);
        flag.update();
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
        window.draw(groundRect);
        flag.draw(window);
        ball.draw(window);
        window.display();
    }

    // Cleanup collectibles
    for (auto& collectible : collectibles) {
        delete collectible;
    }
}
int main() {
    MenuOption option = mainMenu();
    if (option == MenuOption::Play)Level1();
    if (option == MenuOption::Levels)
        std::cout << "LEVEL";
    if (option == MenuOption::Exit)
        std::cout << "Exit";
    return 0;
}