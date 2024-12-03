#include "Inclusion.h"
#include "Grid.h"

// Constants
const int SCENE_WIDTH = 15;  // Number of grid cells wide per scene
const int SCENE_HEIGHT = 8; // Number of grid cells tall per scene

//// Function to load and play background music
//void playBackgroundMusic(SoundBuffer& buffer, Sound& sound) {
//    if (buffer.loadFromFile("C:/Users/omart/OneDrive/Documents/GitHub/SnakeGame/SfmlTests/music.mp3")) {
//        sound.setBuffer(buffer);
//        sound.setLoop(true); // Optional: loops the sound continuously
//        sound.setVolume(10); // Set volume (adjust as needed)
//        sound.play();
//    }
//    else {
//        cerr << "Error: Could not load background music." << endl;
//    }
//}

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Bounce");
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
    //SoundBuffer buffer;
    //Sound sound;
    //playBackgroundMusic(buffer, sound);
    gridMap.loadGrid("gridTest.txt", levelGrid, emptyPositions);

    std::vector<std::string> texturePaths = {
        "Textures/TILE BRICK ground.png", "Textures/TILE BRICK wo ground.png",
        "Textures/TILE BRICK ground tilt.png", "Textures/TILE BRICK jump.png",
        "Textures/sky bb.png", "Textures/flag.png", "Textures/Hole.png",
        "Textures/Pillar.png", "Textures/gate.png", "Textures/size up.png",
        "Textures/Spike.png", "Textures/star.png", "Textures/heart-HP.png",
        "Textures/coin.png", "Textures/TILE BRICK wo ground tilt fill.png",
        "Textures/ball final.png", "Textures/cracked ball FINAL.png",
        "Textures/TILE BRICK wo ground tilt fill.png", "Textures/TILEBRICKwogroundtiltupside.png",
        "Textures/monster1.png", "Textures/TILEBRICKwogroundtiltfill.png",
        "Textures/TILE BRICK wo ground wo grass.png", "Textures/TILEBRICKwogroundtiltupsidegrass.png",
        "Textures/TILE BRICK ground tilt opposite.png"
    };

    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("Textures/sky bb.png")) {
        std::cerr << "Error: Could not load background texture." << std::endl;
        return EXIT_FAILURE;
    }
    sf::Sprite backgroundSprite;
    backgroundSprite.setTexture(backgroundTexture);

    // Scale the background to fit the entire window
    backgroundSprite.setScale(
        static_cast<float>(WINDOW_WIDTH) / backgroundTexture.getSize().x,
        static_cast<float>(WINDOW_HEIGHT) / backgroundTexture.getSize().y
    );


    if (!gridMap.loadTextures(texturePaths)) {
        return EXIT_FAILURE;
    }

    gridMap.initializePhysics(world, levelGrid, cellSizeX, cellSizeY);

    //sf::Texture& backgroundTexture = gridMap.getTexture(4);
    //std::cout << "Texture size: "
    //    << backgroundTexture.getSize().x << " x "
    //    << backgroundTexture.getSize().y << std::endl;

    //sf::Sprite background;
    //background.setTexture(backgroundTexture);
    //background.setScale(
    //    static_cast<float>(WINDOW_WIDTH) / backgroundTexture.getSize().x,
    //    static_cast<float>(WINDOW_HEIGHT) / backgroundTexture.getSize().y
    //);

    // Use texture 15 for the player
    sf::Texture* playerTexture = &gridMap.getTexture(15);
    sf::CircleShape playerCircle(15.0f); // Radius in pixels
    playerCircle.setTexture(playerTexture);
    playerCircle.setOrigin(15.0f, -15.0f); // Center origin

    // Create the player's dynamic circle body in Box2D
    b2BodyDef playerBodyDef;
    playerBodyDef.type = b2_dynamicBody;
    playerBodyDef.position.Set(3.0f, 1.0f); // Initial position in meters
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

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed) {
                b2Vec2 currentPosition = playerBody->GetPosition();
                switch (event.key.code) {
                case sf::Keyboard::D:
                    playerBody->SetTransform(b2Vec2(currentPosition.x + 0.5f, currentPosition.y), playerBody->GetAngle());
                    break;

                case sf::Keyboard::A:
                    playerBody->SetTransform(b2Vec2(currentPosition.x - 0.5f, currentPosition.y), playerBody->GetAngle());
                    break;

                case sf::Keyboard::S:
                    playerBody->SetTransform(b2Vec2(currentPosition.x, currentPosition.y + 0.5f), playerBody->GetAngle());
                    break;
                default:
                    break;
                }
            }
            if (event.type == sf::Event::Resized) {
                float newWidth = static_cast<float>(event.size.width);
                float newHeight = static_cast<float>(event.size.height);

                float aspectRatio = newWidth / newHeight;

                if (aspectRatio > WINDOW_WIDTH / WINDOW_HEIGHT) {
                    float newViewWidth = WINDOW_HEIGHT * aspectRatio;
                    sceneView.setSize(newViewWidth, WINDOW_HEIGHT);
                }
                else {
                    float newViewHeight = WINDOW_HEIGHT / aspectRatio;
                    sceneView.setSize(WINDOW_HEIGHT, newViewHeight);
                }

                window.setView(sceneView);
            }
        }

        world.Step(1.0f / 60.0f, 6, 2);

        b2Vec2 playerPosition = playerBody->GetPosition();
        playerCircle.setPosition(playerPosition.x * SCALE, playerPosition.y * SCALE);

        int playerSceneX = playerCircle.getPosition().x / (SCENE_WIDTH * cellSizeX);
        int playerSceneY = playerCircle.getPosition().y / (SCENE_HEIGHT * cellSizeY);

        if (playerSceneX >= 0 && playerSceneX < (levelGrid[0].size() / SCENE_WIDTH) &&
            playerSceneY >= 0 && playerSceneY < (levelGrid.size() / SCENE_HEIGHT)) {
            // Update current scene and view
            currentSceneX = playerSceneX;
            currentSceneY = playerSceneY;
            sceneView.setCenter(currentSceneX * WINDOW_WIDTH + WINDOW_WIDTH / 2,
                currentSceneY * WINDOW_HEIGHT + WINDOW_HEIGHT / 2);
            window.setView(sceneView);
        }

        window.clear();
        window.draw(backgroundSprite);
        gridMap.drawWalls(window, levelGrid, cellSizeX, cellSizeY, sceneView);
        window.draw(playerCircle);
        window.display();
    }

    return EXIT_SUCCESS;
}