#include "Inclusion.h"
#include "Grid.h"

// Constants
const int SCENE_WIDTH = 21;  // Number of grid cells wide per scene
const int SCENE_HEIGHT = 15; // Number of grid cells tall per scene

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Dynamic Scene Loading with Box2D");
    sf::View sceneView(sf::FloatRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT)); // View for the current scene
    window.setView(sceneView);

    int cellSizeX = WINDOW_WIDTH / SCENE_WIDTH;  // Grid cell width
    int cellSizeY = WINDOW_HEIGHT / SCENE_HEIGHT; // Grid cell height

    // Initialize Box2D world with gravity
    b2Vec2 gravity(0.0f, 9.8f);
    b2World world(gravity);

    // Load the grid and textures
    Grid gridMap;
    std::vector<Position> emptyPositions;
    std::vector<std::vector<char>> levelGrid;
    gridMap.loadGrid("gridTest.txt", levelGrid, emptyPositions);

    std::vector<std::string> texturePaths = {
        "Textures/TILE BRICK.png", "Textures/TILE BRICK wo ground.png",
        "Textures/TILE BRICK ground tilt.png", "Textures/TILE BRICK jump.png",
        "Textures/sky bb.png", "Textures/flag.png", "Textures/Hole.png",
        "Textures/Pillar.png", "Textures/gate.png", "Textures/size up.png",
        "Textures/Spike.png", "Textures/star.png", "Textures/heart-HP.png",
        "Textures/coin.png", "Textures/TILE BRICK wo ground tilt fill.png",
        "Textures/ball final.png", "Textures/cracked ball FINAL.png",
        "Textures/TILEBRICKwogroundtiltfill.png", "Textures/TILEBRICKwogroundtiltupside.png",
        "Textures/TILE BRICK ground tilt update.png", "Textures/TILEBRICKwogroundtiltfill.png"
    };

    if (!gridMap.loadTextures(texturePaths)) {
        return EXIT_FAILURE;
    }

    gridMap.initializePhysics(world, levelGrid, cellSizeX, cellSizeY);

    // Use texture 15 for the player
    sf::Texture* playerTexture = &gridMap.getTexture(15);
    sf::CircleShape playerCircle(15.0f); // Radius in pixels
    playerCircle.setTexture(playerTexture);
    playerCircle.setOrigin(15.0f, 15.0f); // Center origin

    // Create the player's dynamic circle body in Box2D
    b2BodyDef playerBodyDef;
    playerBodyDef.type = b2_dynamicBody;
    playerBodyDef.position.Set(1.0f, 1.0f); // Initial position in meters
    b2Body* playerBody = world.CreateBody(&playerBodyDef);

    b2CircleShape playerShape;
    playerShape.m_radius = 0.5f; // Radius in meters

    b2FixtureDef playerFixtureDef;
    playerFixtureDef.shape = &playerShape;
    playerFixtureDef.density = 1.0f;
    playerFixtureDef.friction = 0.3f;
    playerFixtureDef.restitution = 0.2f; // Slight bounce
    playerBody->CreateFixture(&playerFixtureDef);

    int currentSceneX = 0; // Current scene X index
    int currentSceneY = 0; // Current scene Y index

    // Main game loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Step the Box2D world
        world.Step(1.0f / 60.0f, 6, 2);

        // Update player's position in SFML
        b2Vec2 playerPosition = playerBody->GetPosition();
        playerCircle.setPosition(playerPosition.x * SCALE, playerPosition.y * SCALE);

        // Check if the player has moved to a new scene
        int playerSceneX = playerCircle.getPosition().x / WINDOW_WIDTH;
        int playerSceneY = playerCircle.getPosition().y / WINDOW_HEIGHT;

        if (playerSceneX != currentSceneX || playerSceneY != currentSceneY) {
            currentSceneX = playerSceneX;
            currentSceneY = playerSceneY;

            // Update the view to the new scene
            sceneView.setCenter(currentSceneX * WINDOW_WIDTH + WINDOW_WIDTH / 2,
                currentSceneY * WINDOW_HEIGHT + WINDOW_HEIGHT / 2);
            window.setView(sceneView);
        }

        window.clear();

        // Render walls and objects for the current scene
        gridMap.drawWalls(window, levelGrid, cellSizeX, cellSizeY, sceneView);

        // Render the player
        window.draw(playerCircle);
        window.display();
    }

    return EXIT_SUCCESS;
}