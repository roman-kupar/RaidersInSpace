#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include"player.h"
#include"enemy.h"
#include"asteroid.h"
#include"boss.h"
#include"options.h"
#include"random.h"
#include"powerUp.h"

class World;

class Configuration
{
private:
	World &world;

	sf::Clock timeLevel, timeToSpawnAsteroids, timeToStartSpawningEnemies, timeToSpawnEnemy, timeToSpawnPowerUP;

	unsigned int timeBetweenSpawningEnemies;

public:
	Configuration(World& world);

	enum class Level
	{
		Level1,
		Level2,
		Level3,
		Level4,
		Level5
	};

	Level currentLevel;

	void spawnAsteroids(int amount);
	void spawnEnemy(const sf::Vector2f& point);
	void spawnPowerUP();
	void spawnEnemies();

	void clearLevel();
	void reset();
	void loadPlayer(sf::RenderWindow& window);
	void updateLevel();
	void loadLevel1();
	void loadLevel2();
	void loadLevel3();
	void loadLevel4();
	void loadLevel5();

	void proceed();

};
#endif