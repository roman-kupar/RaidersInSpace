#include "../include/game.h"
#include<iostream>
#include<memory>

unsigned int MAX_TEXT_SPEED = 4;
const unsigned int EXPONENTIAL_FACTOR = 2;
const float INCREASE_RATE = 185.0f;

Game::Game() : width(Options::width), height(Options::height), world(Options::width, Options::height), buttonSize(Options::width/9.f,Options::height/9.f), configuration(world),
textMovementSpeed(1), introState(IntroState::PlayerAnimate)
{
    initWindow();
    initFonts();
    initMainMenu();
}

void Game::initMainMenu() {

    if (!mainTheme.openFromFile("resources/music/mainMenu.wav")) {
        std::cout << "Failed to load menu music!" << std::endl;
    }

    if (!backGroundMusic.openFromFile("resources/music/backgroundmusic.wav")) {
        std::cout << "Failed to load background music";
    }

    if (!starTexture.loadFromFile("resources/bg-stars.png") || !asteroidTexture.loadFromFile("resources/asteroid/asteroid-small.png")) {
        std::cout << "Unable to load picture";
    }

    starTexture.setRepeated(true);
    starTexture.setSmooth(true);

    float randomX, randomY;

    for (size_t i = 0;i<15;i++)
    {
        randomX = Random::GenerateFloat(0.f, float(width));
        randomY = Random::GenerateFloat(0.f, float(height));
        std::unique_ptr<sf::Sprite> star = std::make_unique<sf::Sprite>();
        star.get()->setTexture(starTexture);
        star.get()->setOrigin(starTexture.getSize().x / 2.f, starTexture.getSize().y / 2.f);
        star.get()->setPosition(randomX, randomY);
        stars.push_back(std::move(star));
    }


    mainTheme.setVolume(1);
    mainTheme.setLoop(true);
    mainTheme.play();

    if (!playTexture.loadFromFile("resources/buttons/2.png") || 
        !optionsTexture.loadFromFile("resources/buttons/1.png") ||
        !exitTexture.loadFromFile("resources/buttons/3.png")) {
        std::cout << "Can't load buttons";
    }

    playTexture.setSmooth(true);
    optionsTexture.setSmooth(true);
    exitTexture.setSmooth(true);

    playButton.setTexture(&playTexture);
    optionsButton.setTexture(&optionsTexture);
    exitButton.setTexture(&exitTexture);

    playButton.setSize(buttonSize);
    playButton.setOrigin(buttonSize.x/2.f, buttonSize.y/2.f);
    playButton.setPosition(width/2, height/2 - 100);

    optionsButton.setSize(buttonSize);
    optionsButton.setOrigin(buttonSize.x / 2.f, buttonSize.y / 2.f);
    optionsButton.setPosition(width/2, height/2);

    exitButton.setSize(buttonSize);
    exitButton.setOrigin(buttonSize.x / 2.f, buttonSize.y / 2.f);
    exitButton.setPosition(width/2, height/2 + 100);

    playText.setFont(font);
    playText.setCharacterSize(width/60);
    playText.setFillColor(sf::Color::White);
    playText.setLetterSpacing(1.5);
    playText.setOutlineColor(sf::Color::Black);
    playText.setOutlineThickness(2);
    playText.setString("PLAY");
    sf::FloatRect playTextRect = playText.getLocalBounds();
    playText.setOrigin(playTextRect.left + playTextRect.width / 2.0f, playTextRect.top + playTextRect.height / 2.0f / 2);
    playText.setPosition(width / 2, height / 2 - 100);

    optionsText.setFont(font);
    optionsText.setCharacterSize(width / 60);
    optionsText.setFillColor(sf::Color::White);
    optionsText.setLetterSpacing(1.5);
    optionsText.setOutlineColor(sf::Color::Black);
    optionsText.setOutlineThickness(2);
    optionsText.setString("OPTIONS"); 
    sf::FloatRect optionsTextRect = optionsText.getLocalBounds();
    optionsText.setOrigin(optionsTextRect.left + optionsTextRect.width / 2.0f, optionsTextRect.top + optionsTextRect.height / 2.0f / 2);
    optionsText.setPosition(width / 2, height / 2);

    exitText.setFont(font);
    exitText.setCharacterSize(width / 60);
    exitText.setFillColor(sf::Color::White);
    exitText.setLetterSpacing(1.5);
    exitText.setOutlineColor(sf::Color::Black);
    exitText.setOutlineThickness(2);
    exitText.setString("Exit");
    sf::FloatRect exitTextRect = exitText.getLocalBounds();
    exitText.setOrigin(exitTextRect.left + exitTextRect.width / 2.0f, exitTextRect.top + exitTextRect.height / 2.0f / 2);
    exitText.setPosition(width / 2, height / 2 + 100) ;

    title.setFont(font);
    title.setCharacterSize(width / 20);
    title.setFillColor(sf::Color::White);
    title.setLetterSpacing(1.5);
    title.setOutlineColor(sf::Color::Black);
    title.setOutlineThickness(10);
    title.setString("RAIDERS IN SPACE");
    sf::FloatRect titleTextRect = title.getLocalBounds();
    title.setOrigin(titleTextRect.left + titleTextRect.width / 2.0f, titleTextRect.top + titleTextRect.height / 2.0f / 2);
    title.setPosition(width/2, height/7);

    if (!backgroundTexture.loadFromFile("resources/Space_Background3.png")) { std::cout << "Can't load background"; }

    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setOrigin(backgroundTexture.getSize().x / 2, backgroundTexture.getSize().y / 2);
    backgroundSprite.setPosition(width / 2, height / 2);

    sf::SoundBuffer& hoverButtonSoundBuffer = ResourceManager::sounds.at(ResourceManager::Sound::Hover);
    sf::SoundBuffer& acceptButtonSoundBuffer = ResourceManager::sounds.at(ResourceManager::Sound::Accept);

    hoverButtonSound.setBuffer(hoverButtonSoundBuffer);
    acceptButtonSound.setBuffer(acceptButtonSoundBuffer);

    hoverButtonSound.setVolume(1);
    acceptButtonSound.setVolume(1);
    acceptButtonSound.setPitch(2);

    asteroidTimer.restart();
}

void Game::initFonts()
{
    if (!font.loadFromFile("resources/fonts/GalacticaGrid.ttf"))
    {
        std::cout << "Failed to load font";
    }
}

void Game::initGameHUD()
{
    scoreText.setFont(font);
    scoreText.setCharacterSize(width / 50);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setLetterSpacing(1.5);
    scoreText.setOutlineColor(sf::Color::Black);
    scoreText.setOutlineThickness(2);
    scoreText.setString("Score: " + std::to_string(world.getScore()));
    sf::FloatRect scoreTextRect = scoreText.getLocalBounds();
    scoreText.setOrigin(scoreTextRect.left + scoreTextRect.width / 2.0f, scoreTextRect.top + scoreTextRect.height / 2.0f);
    scoreText.setPosition(40.f,20.f);

    levelText.setFont(font);
    levelText.setCharacterSize(width / 50);
    levelText.setFillColor(sf::Color::White);
    levelText.setLetterSpacing(1.5);
    levelText.setOutlineColor(sf::Color::Black);
    levelText.setOutlineThickness(2);
    levelText.setString("Level: " + std::to_string(static_cast<int>(configuration.currentLevel)+1));
    sf::FloatRect levelTextRect = levelText.getLocalBounds();
    levelText.setOrigin(levelTextRect.left + levelTextRect.width / 2.0f, levelTextRect.top + levelTextRect.height / 2.0f);
    levelText.setPosition(world.getWidth() / 2, 20.f);
}

void Game::initMouse()
{
    window.setMouseCursorVisible(false);

    sf::Texture& crossText = ResourceManager::textures.at(ResourceManager::Texture::Cross);

    mouseSprite.setTexture(crossText);
    mouseSprite.setScale(0.09, 0.09);
    mouseSprite.setOrigin(crossText.getSize().x/2, crossText.getSize().y/2);
    mouseSprite.setPosition(Options::width / 2, Options::height / 2);
}

void Game::initGameStart()
{
    readyText.setFont(font);
    readyText.setCharacterSize(width / 90);
    readyText.setFillColor(sf::Color::Yellow);
    readyText.setLetterSpacing(1.5);
    readyText.setOutlineColor(sf::Color::Black);
    readyText.setOutlineThickness(10);
    readyText.setString("READY");
    sf::FloatRect readyTextRect = readyText.getLocalBounds();
    readyText.setOrigin(readyTextRect.left + readyTextRect.width / 2.0f, readyTextRect.top + readyTextRect.height / 2.0f);
    readyText.setPosition(width / 2, height / 2);

    setText.setFont(font);
    setText.setCharacterSize(width / 90);
    setText.setFillColor(sf::Color::Yellow);
    setText.setLetterSpacing(1.5);
    setText.setOutlineColor(sf::Color::Black);
    setText.setOutlineThickness(10);
    setText.setString("SET");
    sf::FloatRect setTextRect = setText.getLocalBounds();
    setText.setOrigin(setTextRect.left + setTextRect.width / 2.0f, setTextRect.top + setTextRect.height / 2.0f);
    setText.setPosition(width / 2, height / 2);

    goText.setFont(font);
    goText.setCharacterSize(width / 90);
    goText.setFillColor(sf::Color::Yellow);
    goText.setLetterSpacing(1.5);
    goText.setOutlineColor(sf::Color::Black);
    goText.setOutlineThickness(10);
    goText.setString("GO");
    sf::FloatRect goTextRect = goText.getLocalBounds();
    goText.setOrigin(goTextRect.left + goTextRect.width / 2.0f, goTextRect.top + goTextRect.height / 2.0f);
    goText.setPosition(width / 2, height / 2);

    sf::SoundBuffer& readySoundBuffer = ResourceManager::sounds.at(ResourceManager::Sound::Ready);
    sf::SoundBuffer& setSoundBuffer = ResourceManager::sounds.at(ResourceManager::Sound::Set);
    sf::SoundBuffer& goSoundBuffer = ResourceManager::sounds.at(ResourceManager::Sound::Go);

    readySound.setBuffer(readySoundBuffer);
    setSound.setBuffer(setSoundBuffer);
    goSound.setBuffer(goSoundBuffer);
}

void Game::initGameOver()
{
    const sf::Vector2f windowSize = { float(width), float(height) };
    screenFading.setSize(windowSize);
    screenFading.setOrigin(0.f, 0.f);
    screenFading.setPosition(0.f, 0.f);
    screenFading.setFillColor(sf::Color(0, 0, 0, 0));

    gameOverText.setFont(font);
    gameOverText.setCharacterSize(width / 20);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setLetterSpacing(2);
    gameOverText.setOutlineColor(sf::Color::Yellow);
    gameOverText.setOutlineThickness(4);
    gameOverText.setString("GAME OVER");

    sf::FloatRect gameOverTextRect = gameOverText.getGlobalBounds();

    gameOverText.setOrigin(gameOverTextRect.left + gameOverTextRect.width / 2.0f,
        gameOverTextRect.top + gameOverTextRect.height / 2.0f);

    gameOverText.setPosition(width / 2, height / 2);
    std::cout << " " << gameOverText.getPosition().x << " " << gameOverText.getPosition().y;
    fadingTime.restart();
}

void Game::updateMouse()
{
    sf::Vector2i currentMousePosition = sf::Mouse::getPosition(window);
    sf::Vector2f mousePositionFloat = static_cast<sf::Vector2f>(currentMousePosition);

    mouseSprite.setPosition(mousePositionFloat);
}

void Game::returnToStartScreen()
{
    gameState = GameState::StartScreen;
    configuration.reset();
    backGroundMusic.stop();
    mainTheme.play();
    mainTheme.setVolume(1);
    mainTheme.setLoop(true);
    window.setMouseCursorVisible(true);
}

void Game::pollEvents()
{
    while (window.pollEvent(sfmlEvent))
    {
        switch (sfmlEvent.type)
        {
        case sf::Event::Closed:
            window.close();
            break;
        case sf::Event::KeyPressed:
            if (sfmlEvent.key.code == sf::Keyboard::Escape)
            {
                returnToStartScreen();
            }
            break;
        default:
            break;
        }
    }

}

const bool Game::running() const
{
    return window.isOpen();
}

void Game::initWindow()
{
    videoMode = sf::VideoMode(width, height);
    window.create(videoMode, "Raiders in Space", Options::fullscreenOn ? sf::Style::Fullscreen : sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);
    sf::Image icon;
    icon.loadFromFile("resources/player/player2.png");
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
}

void Game::update(float deltaTime)
{
    scoreText.setString("Score: " + std::to_string(world.getScore()));
    levelText.setString("Level: " + std::to_string(static_cast<int>(configuration.currentLevel) + 1));
    updateMouse();
    world.update(deltaTime);
    if (world.isPlayerDead())
        gameState = GameState::GameOver;
}

void Game::updateOverScreen(float deltaTime)
{
    if (backGroundMusic.getStatus() == sf::Music::Playing)
        backGroundMusic.stop();

    if (fadingTime.getElapsedTime().asSeconds() > 0.01f && screenFading.getFillColor().a < 255 && gameOverText.getPosition().x < width + gameOverText.getGlobalBounds().width)
    {
        int i = screenFading.getFillColor().a;
        screenFading.setFillColor(sf::Color(0, 0, 0, i + 5));
        fadingTime.restart();
    }
    else if (screenFading.getFillColor().a >= 255 && fadingTime.getElapsedTime().asSeconds() > 1.5f && gameOverText.getPosition().y >= 50.f)
    {
        gameOverText.move({ 0.f,-4.f });
        //std::cout << " " << gameOverText.getPosition().x << " " << gameOverText.getPosition().y << "\n ";
    }
    else if (fadingTime.getElapsedTime().asSeconds() > 5.f && gameOverText.getPosition().y <= 50.f && gameOverText.getPosition().x < width + gameOverText.getGlobalBounds().width)
    {
        gameOverText.move({ 8.f,0.f });
    }
    else if (fadingTime.getElapsedTime().asSeconds() > 0.01f && screenFading.getFillColor().a > 0 && gameOverText.getPosition().x > width + gameOverText.getGlobalBounds().width)
    {
        updateBack(deltaTime);
        int i = screenFading.getFillColor().a;
        i-=5;
        screenFading.setFillColor(sf::Color(0, 0, 0, i));
        fadingTime.restart();
    }
    else if (screenFading.getFillColor().a == 0 && gameOverText.getPosition().x > width + gameOverText.getGlobalBounds().width)
    {
        returnToStartScreen();
    }
}

void Game::updateBack(float deltaTime)
{
    float randomX;
    for (auto& starPtr : stars)
    {
        sf::Sprite& star = *starPtr;
        star.move(-100.f * deltaTime, 100.f * deltaTime);

        if (star.getPosition().y >= height || star.getPosition().x < 0)
        {
            randomX = Random::GenerateFloat(200.f, float(width) + 500.f);
            star.setPosition(randomX, 0);
        }
    }


    if (asteroidTimer.getElapsedTime().asSeconds() > 2)
    {
        randomX = Random::GenerateFloat(200.f, float(width) + 500.f);
        std::unique_ptr<sf::Sprite> asteroid = std::make_unique<sf::Sprite>();
        asteroid.get()->setTexture(asteroidTexture);
        asteroid.get()->setOrigin(asteroidTexture.getSize().x / 2.f, asteroidTexture.getSize().y / 2.f);
        asteroid.get()->setPosition(randomX, 0);

        asteroids.push_back(std::move(asteroid));

        asteroidTimer.restart();
    }

    for (auto asteroidIter = asteroids.begin(); asteroidIter != asteroids.end();)
    {
        sf::Sprite& asteroid = **asteroidIter;

        asteroid.move(-100.f * deltaTime, 50.f * deltaTime);

        if (asteroid.getPosition().y >= height)
        {
            asteroidIter = asteroids.erase(asteroidIter);
        }
        else
        {
            ++asteroidIter;
        }
    }
}

void Game::updateIntro(float deltaTime)
{
    float sizeIncrease = INCREASE_RATE * deltaTime;

    switch(introState)
    {
    case IntroState::PlayerAnimate:
        if (playerAnimating.getElapsedTime().asSeconds() >= 2.f)
            introState = IntroState::Ready;
        break;
    case IntroState::Ready:
        if (readyText.getPosition().y > width)
        {
            readyText.setCharacterSize(0);
            introState = IntroState::Set;
            soundIsPlayed = false;
        }
        else if (readyText.getCharacterSize() > 120)
        {
            readyText.move(0, +50.f);
            textMovementSpeed = 1;
        }
        else
        {
            readyText.setCharacterSize(readyText.getCharacterSize() + sizeIncrease);
            readyText.setLetterSpacing(readyText.getLineSpacing() + (sizeIncrease + 3));
            sf::FloatRect readyTextRect = readyText.getLocalBounds();
            readyText.setOrigin(readyTextRect.left + readyTextRect.width / 2.0f, readyTextRect.top + readyTextRect.height / 2.0f);
            readyText.setPosition(width / 2, height / 2);

            if (!soundIsPlayed)
            {
                soundIsPlayed = true;
                readySound.play();
            }
        }
        break;
    case IntroState::Set:
        if (setText.getPosition().y > width)
        {
            setText.setCharacterSize(0);
            introState = IntroState::Go;
            soundIsPlayed = false;
        }
        else if (setText.getCharacterSize() > 110)
        {
            setText.move(0, +50.f);
            textMovementSpeed = 1;
        }
        else
        {
            setText.setCharacterSize(setText.getCharacterSize() + sizeIncrease);
            setText.setLetterSpacing(setText.getLineSpacing() + (sizeIncrease + 3));
            sf::FloatRect setTextRect = setText.getLocalBounds();
            setText.setOrigin(setTextRect.left + setTextRect.width / 2.0f, setTextRect.top + setTextRect.height / 2.0f);
            setText.setPosition(width / 2, height / 2);


            if (!soundIsPlayed)
            {
                soundIsPlayed = true;
                setSound.play();
            }
        }
        break;
    case IntroState::Go:
        if (goText.getPosition().y > width)
        {
            goText.setCharacterSize(0);
            gameState = GameState::Load;
            soundIsPlayed = false;
        }
        else if (goText.getCharacterSize() > 80)
        {
            goText.move(0, +90.f);
            textMovementSpeed = 1;
        }
        else
        {
            goText.setCharacterSize(goText.getCharacterSize() + sizeIncrease);
            goText.setLetterSpacing(goText.getLineSpacing() + (sizeIncrease + 3));
            sf::FloatRect goTextRect = goText.getLocalBounds();
            goText.setOrigin(goTextRect.left + goTextRect.width / 2.0f, goTextRect.top + goTextRect.height / 2.0f) ;
            goText.setPosition(width / 2, height / 2);


            if (!soundIsPlayed)
            {
                soundIsPlayed = true;
                goSound.play();
            }
        }
        break;
    }
}

void Game::render()
{
    window.clear(sf::Color::Black);
    window.draw(world);
    window.draw(scoreText);
    window.draw(levelText);
    window.draw(mouseSprite);
    window.display();
}

void Game::renderMainMenu() 
{
    window.clear(sf::Color::Black);
    window.draw(backgroundSprite);

    for (const auto& star : stars)
        window.draw(*star);
    for (const auto& asteroid : asteroids)
        window.draw(*asteroid);

    window.draw(title);
    window.draw(playButton);
    window.draw(optionsButton);
    window.draw(exitButton);
    window.draw(playText);
    window.draw(optionsText);
    window.draw(exitText);
    window.display();
}

void Game::renderIntro()
{
    window.clear();
    window.draw(world);
    switch(introState)
    {
    case IntroState::Ready:
        window.draw(readyText);
        break;
    case IntroState::Set:
        window.draw(setText);
        break;
    case IntroState::Go:
        window.draw(goText);
        break;
    }
    window.display();
}

void Game::renderGameOver()
{
    window.clear(sf::Color::White);

    if (gameOverText.getPosition().x < width+gameOverText.getGlobalBounds().width)
        window.draw(world);
    else
    {   
        window.draw(backgroundSprite);

        for (const auto& star : stars)
            window.draw(*star);
        for (const auto& asteroid : asteroids)
            window.draw(*asteroid);

        window.draw(title);
        window.draw(playButton);
        window.draw(optionsButton);
        window.draw(exitButton);
        window.draw(playText);
        window.draw(optionsText);
        window.draw(exitText);
    }

    window.draw(screenFading);

    if (screenFading.getFillColor().a>=255)
        window.draw(gameOverText);

    window.display();
}

void Game::handleEvents() {
    static bool playButtonHovered = false;
    static bool optionsButtonHovered = false;
    static bool exitButtonHovered = false;
    static bool isAccepted = false;

    while (window.pollEvent(sfmlEvent)) {
        if (sfmlEvent.type == sf::Event::Closed)
            window.close();

        if (sfmlEvent.type == sf::Event::MouseMoved) {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

            bool playHovered = playButton.getGlobalBounds().contains(mousePos);
            bool optionsHovered = optionsButton.getGlobalBounds().contains(mousePos);
            bool exitHovered = exitButton.getGlobalBounds().contains(mousePos);

            if (playHovered && !playButtonHovered) {
                hoverButtonSound.play();
                playButtonHovered = true;
            }
            else if (!playHovered && playButtonHovered) {
                playButtonHovered = false;
            }

            if (optionsHovered && !optionsButtonHovered) {
                hoverButtonSound.play();
                optionsButtonHovered = true;
            }
            else if (!optionsHovered && optionsButtonHovered) {
                optionsButtonHovered = false;
            }

            if (exitHovered && !exitButtonHovered) {
                hoverButtonSound.play();
                exitButtonHovered = true;
            }
            else if (!exitHovered && exitButtonHovered) {
                exitButtonHovered = false;
            }
        }

        if (sfmlEvent.type == sf::Event::MouseButtonPressed) {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            if (playButton.getGlobalBounds().contains(mousePos))
            {
                gameState = GameState::Intro;
                introState = IntroState::PlayerAnimate;

                mainTheme.stop();
                if (!isAccepted)
                    acceptButtonSound.play();

                initMouse();
                initGameStart();
                initGameHUD();
                initGameOver();

                configuration.loadPlayer(window);

                playerAnimating.restart();
            }
            else if (optionsButton.getGlobalBounds().contains(mousePos))
            {
                acceptButtonSound.play();
                if (!isAccepted)
                    acceptButtonSound.play();
                //TODO options
            }
            else if (exitButton.getGlobalBounds().contains(mousePos))
            {
                acceptButtonSound.play();
                if (!isAccepted)
                    acceptButtonSound.play();
                window.close();
            }
        }
    }
}

void Game::handleIntro(float deltaTime)
{
    pollEvents();
    update(deltaTime);
    updateIntro(deltaTime);
    renderIntro();
}

void Game::handleStartScreen(float deltaTime) {
    handleEvents();
    updateBack(deltaTime);
    renderMainMenu();
}

void Game::handleGameOverScreen(float deltaTime) 
{
    updateOverScreen(deltaTime);
    renderGameOver();
}

bool Game::checkEnemiesAlive()
{
    for (const auto& entity : world.getEntities()) {
        if (entity->getType() == Entity::Type::Enemy || entity->getType() == Entity::Type::SmallAsteroid || entity->getType() == Entity::Type::BigAsteroid) {
            return true;
        }
    }

    return false;
}

void Game::isLevelCompleted()
{
    switch (configuration.currentLevel)
    {
    case Configuration::Level::Level1:
        if (Level1Time.getElapsedTime().asSeconds() > 60)
        {
            if (!checkEnemiesAlive())
            {
                configuration.proceed();
                configuration.clearLevel();
                handleLoading();
            }
        }
        break;
    case Configuration::Level::Level2:
        if (Level2Time.getElapsedTime().asSeconds() > 15.f)
        {
            if (!checkEnemiesAlive())
            {
                configuration.proceed();
                configuration.clearLevel();
                handleLoading();
            }
        }
        break;
    }
}

void Game::handleLoading()
{
    switch (configuration.currentLevel)
    {
    case Configuration::Level::Level1:
        configuration.loadLevel1();
        Level1Time.restart();
        break;
    case Configuration::Level::Level2:
        configuration.loadLevel2();
        Level2Time.restart();
        break;
    }
    gameState = GameState::Playing;
    backGroundMusic.play();
    backGroundMusic.setLoop(true);
    backGroundMusic.setVolume(1);
    render();
}

void Game::handlePlaying(float deltaTime)
{
    update(deltaTime);
    configuration.updateLevel();
    isLevelCompleted();
    pollEvents();
    render();
}

void Game::run() {
    sf::Clock clock;

    while (running()) {
        float deltaTime = clock.restart().asSeconds();

        switch (gameState) {
        case GameState::StartScreen:
            handleStartScreen(deltaTime);
            break;
        case GameState::Intro:
            handleIntro(deltaTime);
            break;
        case GameState::Load:
            handleLoading();
            break;
        case GameState::Playing:
            handlePlaying(deltaTime);
            break;
        case GameState::GameOver:
            handleGameOverScreen(deltaTime);
            break;
        case GameState::Exiting:
            window.close();
            break;
        }
    }
}
