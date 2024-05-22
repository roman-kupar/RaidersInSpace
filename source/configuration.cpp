#include "../include/configuration.h"

Configuration::Configuration(World& world) : world(world), currentLevel(Level::Level1), timeBetweenSpawningEnemies(0)
{}

void Configuration::spawnAsteroids(int amount)
{
    for (size_t i = 0;i<amount;i++)
    {
        Asteroid asteroid(world);
    }
}

void Configuration::clearLevel()
{
    world.clearEntities();
}

void Configuration::reset()
{
    world.clearAllEntites();
}

void Configuration::loadPlayer(sf::RenderWindow& window)
{
    Player player(world, window);
}

void Configuration::proceed() {
    switch (currentLevel) {
    case Level::Level1:
        currentLevel = Level::Level2;
        break;
    case Level::Level2:
        currentLevel = Level::Level3;
        break;
    case Level::Level3:
        currentLevel = Level::Level4;
        break;
    case Level::Level4:
        currentLevel = Level::Level5;
        break;
    case Level::Level5:
        break;
    }
}

void Configuration::spawnEnemy(const sf::Vector2f& point)
{
    Enemy(world, point);
}

void Configuration::updateLevel()
{
    switch (currentLevel)
    {
    case Level::Level1:
        if (timeToSpawnAsteroids.getElapsedTime().asSeconds() > 5.f && timeLevel.getElapsedTime().asSeconds() < 60.f)
        {
            spawnAsteroids(5);
            timeToSpawnAsteroids.restart();
        }

        break;
    }
}

//if (timeToStartSpawningEnemies.getElapsedTime().asSeconds() > 15.f)
//{
//    if (timeToSpawnEnemy.getElapsedTime().asSeconds() >= timeBetweenSpawningEnemies)
//    {
//        timeBetweenSpawningEnemies = Random::GenerateInt(3, 5);
//        spawnEnemy(sf::Vector2f(Random::GenerateDouble(0,world.getWidth()), Random::GenerateDouble(0,world.getHeight())));
//        timeToSpawnEnemy.restart();
//    }
//}

void Configuration::loadLevel1()
{
    spawnAsteroids(5);
    timeLevel.restart();
}

void Configuration::loadLevel2()
{


}
