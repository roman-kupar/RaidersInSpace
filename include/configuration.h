#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include"player.h"
#include"enemy.h"
#include"asteroid.h"
#include"options.h"
#include"random.h"

class World;

class Configuration
{
private:
	World &world;

	sf::Clock timeLevel, timeToSpawnAsteroids, timeToStartSpawningEnemies, timeToSpawnEnemy;

	int timeBetweenSpawningEnemies;

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
	void spawnEnemies();

	void clearLevel();
	void reset();
	void loadPlayer(sf::RenderWindow& window);
	void updateLevel();
	void loadLevel1();
	void loadLevel2();
	void loadLevel3();

	void proceed();

};
#endif