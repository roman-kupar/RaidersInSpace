#ifndef GAME_H
#define GAME_H

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"

#include "world.h"
#include "configuration.h"

class Game {
private:
	enum class GameState
	{
		StartScreen,
		Load,
		Intro,
		Playing,
		GameOver,
		Exiting
	};

	enum class IntroState 
	{
		PlayerAnimate,
		Ready,
		Set,
		Go
	};

	GameState gameState;
	IntroState introState;

	sf::VideoMode videoMode;
	sf::RenderWindow window;
	sf::Event sfmlEvent;
	
	World world;
	Configuration configuration;

	sf::Sprite cursor;

	const unsigned int width, height;
	unsigned int textMovementSpeed/*, score*/;
	bool soundIsPlayed = false;
	
	sf::Texture playTexture, optionsTexture, exitTexture, backgroundTexture, starTexture, asteroidTexture;
	sf::Sprite	backgroundSprite, mouseSprite;
	
	sf::Font font;
	sf::Text title, playText, optionsText, exitText, readyText, setText, goText, scoreText, levelText, gameOverText;

	sf::RectangleShape playButton;
	sf::RectangleShape optionsButton;
	sf::RectangleShape exitButton;
	sf::RectangleShape screenFading;

	sf::Vector2f buttonSize;

	sf::Music mainTheme, backGroundMusic;

	sf::Sound hoverButtonSound, acceptButtonSound, readySound, setSound, goSound;

	sf::Clock asteroidTimer, playerAnimating;
	sf::Clock fadingTime;
	sf::Clock Level1Time, Level2Time, Level3Time;

	std::list<std::unique_ptr<sf::Sprite>> stars;
	std::list<std::unique_ptr<sf::Sprite>> asteroids;

public:
	Game();
	~Game() = default;

	void initWindow();
	void initMainMenu();
	void initFonts();
	void initGameStart();
	void initGameHUD();
	void initGameOver();	
	void initMouse();
	void handleEvents();
	void handleStartScreen(float deltaTime);
	void handleLoading();
	void handleIntro(float deltaTime);
	void handlePlaying(float deltaTime);
	void handleGameOverScreen(float deltaTime);
	void updateIntro(float deltaTime);
	void updateOverScreen(float deltaTime);
	void updateBack(float deltaTime);
	void updateMouse();
	void update(float deltaTime);
	void render();
	void renderIntro();
	void renderMainMenu();
	void renderGameOver();
	void returnToStartScreen();
	void pollEvents();
	void isLevelCompleted();
	bool checkEnemiesAlive();
	const bool running() const;

	void run();

};

#endif 