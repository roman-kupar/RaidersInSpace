#include "../include/configuration.h"

Configuration::Configuration(World& world) : world(world), currentLevel(Level::Level1), timeBetweenSpawningEnemies(5)
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
    timeLevel.restart();
    currentLevel = Level::Level1;
}

void Configuration::loadPlayer(sf::RenderWindow& window)
{
    Player player(world, window);
}

void Configuration::proceed() {

    timeLevel.restart();

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
    Enemy enemy(world, point, false);
}

void Configuration::spawnPowerUP()
{
    PowerUP powerUP(world);
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

        if (timeToSpawnPowerUP.getElapsedTime().asSeconds() > 25.f)
        {
            spawnPowerUP();
            timeToSpawnPowerUP.restart();
        }

        break;
    case Level::Level2:
        if (timeToSpawnEnemy.getElapsedTime().asSeconds() > timeBetweenSpawningEnemies && timeLevel.getElapsedTime().asSeconds() < 60.f)
        {
            spawnEnemy(sf::Vector2f(Random::GenerateFloat(0, Options::width), Random::GenerateFloat(0, Options::height)));
            timeToSpawnEnemy.restart();
        }

        if (timeToSpawnAsteroids.getElapsedTime().asSeconds() > 5.f && timeLevel.getElapsedTime().asSeconds() < 60.f)
        {
            spawnAsteroids(5);
            timeToSpawnAsteroids.restart();
        }

        if (timeToSpawnPowerUP.getElapsedTime().asSeconds() > 20.f)
        {
            spawnPowerUP();
            timeToSpawnPowerUP.restart();
        }
        break;
    case Level::Level3:
        if (timeToSpawnEnemy.getElapsedTime().asSeconds() > timeBetweenSpawningEnemies && timeLevel.getElapsedTime().asSeconds() < 60.f)
        {
            spawnEnemy(sf::Vector2f(Random::GenerateFloat(0, Options::width), Random::GenerateFloat(0, Options::height)));
            timeToSpawnEnemy.restart();
        }
        if (timeToSpawnAsteroids.getElapsedTime().asSeconds() > 7.f && timeLevel.getElapsedTime().asSeconds() < 60.f)
        {
            spawnAsteroids(10);
            timeToSpawnAsteroids.restart();
        }

        if (timeToSpawnPowerUP.getElapsedTime().asSeconds() > 15.f)
        {
            spawnPowerUP();
            timeToSpawnPowerUP.restart();
        }

        break;
    case Level::Level4:
        if (timeToSpawnEnemy.getElapsedTime().asSeconds() > timeBetweenSpawningEnemies && timeLevel.getElapsedTime().asSeconds() < 30.f)
        {
            spawnEnemy(sf::Vector2f(Random::GenerateFloat(0, Options::width), Random::GenerateFloat(0, Options::height)));
            spawnEnemy(sf::Vector2f(Random::GenerateFloat(0, Options::width), Random::GenerateFloat(0, Options::height)));

            timeToSpawnEnemy.restart();
        }
        if (timeToSpawnAsteroids.getElapsedTime().asSeconds() > 6.f && timeLevel.getElapsedTime().asSeconds() < 30.f)
        {
            spawnAsteroids(10);
            timeToSpawnAsteroids.restart();
        }

        if (timeToSpawnPowerUP.getElapsedTime().asSeconds() > 10.f)
        {
            spawnPowerUP();
            timeToSpawnPowerUP.restart();
        }
        break;
    case Level::Level5:
        if (timeToSpawnPowerUP.getElapsedTime().asSeconds() > 10.f)
        {
            spawnPowerUP();
            timeToSpawnPowerUP.restart();
        }
        break;
    }
}



void Configuration::loadLevel1()
{
    spawnAsteroids(5);
    timeLevel.restart();
}

void Configuration::loadLevel2()
{
    spawnAsteroids(5);
    spawnEnemy(sf::Vector2f(Random::GenerateFloat(0, Options::width), Random::GenerateFloat(0, Options::height)));
    timeToSpawnEnemy.restart();
    timeToSpawnAsteroids.restart();
}

void Configuration::loadLevel3()
{
    spawnAsteroids(5);
    spawnEnemy(sf::Vector2f(Random::GenerateFloat(0, Options::width), Random::GenerateFloat(0, Options::height)));
    spawnEnemy(sf::Vector2f(Random::GenerateFloat(0, Options::width), Random::GenerateFloat(0, Options::height)));
    spawnEnemy(sf::Vector2f(Random::GenerateFloat(0, Options::width), Random::GenerateFloat(0, Options::height)));

    timeBetweenSpawningEnemies--;

    timeToSpawnEnemy.restart();
    timeToSpawnAsteroids.restart();

}

void Configuration::loadLevel4()
{
    spawnAsteroids(5);
    spawnEnemy(sf::Vector2f(Random::GenerateFloat(0, Options::width), Random::GenerateFloat(0, Options::height)));
    spawnEnemy(sf::Vector2f(Random::GenerateFloat(0, Options::width), Random::GenerateFloat(0, Options::height)));
    spawnEnemy(sf::Vector2f(Random::GenerateFloat(0, Options::width), Random::GenerateFloat(0, Options::height)));
    spawnEnemy(sf::Vector2f(Random::GenerateFloat(0, Options::width), Random::GenerateFloat(0, Options::height)));
    spawnEnemy(sf::Vector2f(Random::GenerateFloat(0, Options::width), Random::GenerateFloat(0, Options::height)));

    //timeBetweenSpawningEnemies--;

    timeToSpawnEnemy.restart();
    timeToSpawnAsteroids.restart();
}

void Configuration::loadLevel5()
{
    Boss boss(world, sf::Vector2f(Options::width-200, Options::height/2));
}
