#include "box2d/box2d.h"
#include "SFML/Graphics.hpp"
#include <vector>
using namespace sf;
using namespace std;

const float SCALE = 30.0f; // Box2D scale factor: 1 meter = 30 pixels
const float GravForce = 1;
bool flag = false;

int main()
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(1000, 800), "Bounce");

    // Initialize Box2D world with gravity
    b2Vec2 gravity(0.0f, 0.0f); // Gravity in the y-axis
    b2World world(gravity);

    // Main game loop
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Step the Box2D world
        world.Step(1.0f / 60.0f, 6, 2); // 60 Hz, 6 velocity iterations, 2 position iterations

        // Clear the window
        window.clear();

        // Display the rendered frame
        window.display();
    }

    return EXIT_SUCCESS;
}
