#include "box2d/box2d.h"
#include "SFML/Graphics.hpp"
#include "Map.h"
#include "Ball.h"
#include "Ground.h"

int main()
{
    Map map;

    // Create the main window
    sf::RenderWindow window(sf::VideoMode(1000, 800), "Bounce with Ball and Jump");

    // Load the map
    if (map.UploadMaps(window) == -1) {
        return EXIT_FAILURE; // Exit if map textures fail to load
    }

    // Initialize Box2D world with gravity
    b2Vec2 gravity(0.0f, 9.8f); // Gravity pointing downward
    b2World world(gravity);

    // Create the ball
    Ball ball(world, 500.f, 400.f, 20.f); // Ball at (500, 400) with radius 20

    // Create the ground
    Ground ground(world, 500.f, 700.f, 1000.f, 20.f); // Ground at (500, 700) with width 1000, height 20

    sf::Clock clock;

    // Main game loop
    while (window.isOpen())
    {

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Handle input for jump
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            ball.Jump();
        }

        

        // Handle input and update the map
        map.HandleInput();
    // Calculate delta time
        float deltaTime = clock.restart().asSeconds();
        ball.Update(deltaTime);
        map.MovingPlatform(deltaTime);

        // Step the Box2D world
        world.Step(1.0f / 60.0f, 6, 2); // 60 Hz, 6 velocity iterations, 2 position iterations

        // Update the ball position

        // Render everything
        window.clear();
        map.Draw(window);
        ball.Draw(window);
        ground.Draw(window);
        window.display();
    }
    return EXIT_SUCCESS;
}
