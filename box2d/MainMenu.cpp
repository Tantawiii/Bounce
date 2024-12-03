#include "MainMenu.h"

MainMenu::MainMenu(float windowWidth, float windowHeight)
{
    // Load textures
    if (!playTexture.loadFromFile("C:/Users/merot/OneDrive/Documents/GitHub/Bounce/box2d/assets/Images/PLAY.png") ||
        !levelsTexture.loadFromFile("C:/Users/merot/OneDrive/Documents/GitHub/Bounce/box2d/assets/Images/LEVELBLUE.png") ||
        !exitTexture.loadFromFile("C:/Users/merot/OneDrive/Documents/GitHub/Bounce/box2d/assets/Images/EXIT.png")) {
        std::cout << "Failed to load menu images!" << std::endl;
        return;
    }

    // Set up buttons
    setupButton(playButton, playTexture, windowWidth / 2, windowHeight / 2 - 60, sf::Vector2f(6.0f, 6.0f));  // Slightly higher
    setupButton(levelsButton, levelsTexture, windowWidth / 2, windowHeight / 2, sf::Vector2f(6.0f, 6.0f));     // Centered vertically
    setupButton(exitButton, exitTexture, windowWidth / 2, windowHeight / 2 + 60, sf::Vector2f(6.0f, 6.0f));   // Slightly lower


    // Load font
    if (!font.loadFromFile("C:/Users/merot/OneDrive/Documents/GitHub/Bounce/box2d/assets/Fonts/pixelfont.ttf")) {
        std::cerr << "Failed to load font\n";
        return;
    }

    // Set up title text
    titleText.setFont(font);
    titleText.setString("Bounce");
    titleText.setCharacterSize(100);
    titleText.setFillColor(sf::Color::White);
    titleText.setStyle(sf::Text::Bold);
    titleText.setPosition(windowWidth / 2 - titleText.getGlobalBounds().width / 2, 50);
}

MenuOption MainMenu::handleInput(const sf::RenderWindow& window)
{
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        if (playButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            return MenuOption::Play;
        }
        if (levelsButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            return MenuOption::Levels;
        }
        if (exitButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            return MenuOption::Exit;
        }
    }
    return MenuOption::None;
}

void MainMenu::draw(sf::RenderWindow& window)
{
    window.draw(titleText);
    window.draw(playButton);
    window.draw(levelsButton);
    window.draw(exitButton);
}

void MainMenu::setupButton(sf::Sprite& button, sf::Texture& texture, float x, float y, sf::Vector2f scale)
{
    button.setTexture(texture);
    button.setOrigin(texture.getSize().x / 2, texture.getSize().y / 2);
    button.setPosition(x, y);
    button.setScale(scale);
}
