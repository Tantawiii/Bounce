#include "box2d/box2d.h"
#include "SFML/Graphics.hpp"
#include <vector>
#include "Map.h"
using namespace sf;
using namespace std;
#include "box2d/box2d.h"
#include "SFML/Graphics.hpp"
#include <vector>
#include "Map.h"

const float SCALE = 30.0f; // Box2D scale factor: 1 meter = 30 pixels
const float GravForce = 1;
bool flag = false;

int main()
{
    Map map;

    // Create the main window
    sf::RenderWindow window(sf::VideoMode(1000, 800), "Bounce");

    // Load the map
    if (map.UploadMaps(window) == -1) {
        return EXIT_FAILURE; // Exit if map textures fail to load
    }

    // Initialize Box2D world with gravity
    b2Vec2 gravity(0.0f, 0.0f); // Gravity in the y-axis
    b2World world(gravity);

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

        // Calculate delta time
        float deltaTime = clock.restart().asSeconds();

        // Handle input and update the map
        map.HandleInput();
        map.MovingPlatform(deltaTime);

        // Step the Box2D world
        world.Step(1.0f / 60.0f, 6, 2); // 60 Hz, 6 velocity iterations, 2 position iterations

        // Render everything
        window.clear();
        map.Draw(window);
        window.display();
    }
    return EXIT_SUCCESS;
}
