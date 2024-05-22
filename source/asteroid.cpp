#include "../include/asteroid.h"
#include <iostream>

Asteroid::Asteroid(World& world) :
    Entity(ResourceManager::Texture::AsteroidSmall, Type::SmallAsteroid, world, 1, 1),
    isInsideWindow(false),
    isNotInsideWindowAnymore(false)
{
    bool isBig = Random::GenerateInt(0, 10) == 0;

    if (isBig)
    {
        healthMax = 3;
        currentHealth = 3;
        changeTexture(ResourceManager::Texture::AsteroidBig);
        changeType(Type::BigAsteroid);
    }

    setPosition(randomOffscreenPosition());

    setVelocity();

    world.add(std::make_unique<Asteroid>(*this));
}

void Asteroid::setVelocity()
{
    float velocityMagnitude = randomInitialVelocityMagnitude();
    sf::Vector2f screenCenter(world.getWidth() / 2.0f, world.getHeight() / 2.0f);
    sf::Vector2f direction = screenCenter - getPosition();
    float directionMagnitude = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    velocity = direction * (velocityMagnitude / directionMagnitude);
}

sf::Vector2f Asteroid::randomOffscreenPosition()
{
    sf::Vector2i screenSize(world.getWidth(), world.getHeight());
    float buffer = 10.0f;

    int side = Random::GenerateInt(0, 3); 

    float randomX, randomY;

    switch (side) {
    case 0: 
        randomX = Random::GenerateFloat(0, screenSize.x);
        randomY = -buffer;
        break;
    case 1: 
        randomX = screenSize.x + buffer;
        randomY = Random::GenerateFloat(0, screenSize.y);
        break;
    case 2:
        randomX = Random::GenerateFloat(0, screenSize.x);
        randomY = screenSize.y + buffer;
        break;
    case 3: 
        randomX = -buffer;
        randomY = Random::GenerateFloat(0, screenSize.y);
        break;
    default:
        randomX = Random::GenerateFloat(0, screenSize.x);
        randomY = -buffer;
        break;
    }

    return sf::Vector2f(randomX, randomY);
}


float Asteroid::randomInitialVelocityMagnitude()
{
    constexpr float minMagnitude = 50.0f; 
    constexpr float maxMagnitude = 100.0f; 
    return Random::GenerateFloat(minMagnitude, maxMagnitude);
}

void Asteroid::update(float deltaTime)
{
    sprite.move(velocity * deltaTime);

    const sf::Vector2f& position = sprite.getPosition();

    if ((position.x - getSpriteSize().x / 2 > world.getWidth() || position.x + getSpriteSize().x / 2 < 0 ||
        position.y - getSpriteSize().y / 2 > world.getHeight() || position.y + getSpriteSize().y < 0) && isInsideWindow)
    {
        toRemove = true;
        //std::cout << "removed ";
    }
    else if ((position.x - 300.f > world.getWidth() || position.x + 300.f < 0) && (position.y - 300.f > world.getHeight() ||
        position.y + 300.f < 0))
        setVelocity();

    isInsideWindow = (position.x + getSpriteSize().x / 2  >= 0 && position.x - getSpriteSize().x / 2 <= world.getWidth() &&
        position.y + getSpriteSize().y / 2 >= 0 && position.y - getSpriteSize().y / 2 <= world.getHeight());

    whenHit();

    onDestroy();
}

void Asteroid::onCollide(Entity& other)
{
    switch (other.getType())
    {
    case Type::PlayerBullet:
        hited = true;
        world.updateScore(10);
        break;
    case Type::EnemyBullet:
        hited = true;
        break;
    case Type::SmallAsteroid:
    case Type::BigAsteroid:
    case Type::Enemy:
        collisionNormal = other.getPosition() - getPosition();
        collisionNormal = collisionNormal / std::sqrt(collisionNormal.x * collisionNormal.x + collisionNormal.y * collisionNormal.y);
        velocity = velocity - 2.f * dotProduct(velocity, collisionNormal) * collisionNormal;
        break;
    case Type::Player:
        toExplode = true;
    default:
        break;
    }
}

float Asteroid::dotProduct(const sf::Vector2f& v1, const sf::Vector2f& v2)
{
    return v1.x * v2.x + v1.y * v2.y;
}
