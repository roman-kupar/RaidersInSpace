#include"../include/powerUp.h"

PowerUP::PowerUP(World& world) : Entity(ResourceManager::Texture::PowerUP, Type::PowerUP, world, 1, 1)
{
    float screenWidth = Options::width;
    float screenHeight = Options::height;

    int edge = Random::GenerateInt(0, 3);
    sf::Vector2f position;

    switch (edge)
    {
    case 0: 
        position.x = Random::GenerateFloat(0.f, screenWidth);
        position.y = -50.f; 
        break;
    case 1: 
        position.x = Random::GenerateFloat(0.f, screenWidth);
        position.y = screenHeight + 50.f; 
        break;
    case 2:
        position.x = -50.f; 
        position.y = Random::GenerateFloat(0.f, screenHeight);
        break;
    case 3: 
        position.x = screenWidth + 50.f; 
        position.y = Random::GenerateFloat(0.f, screenHeight);
        break;
    }

    sprite.setPosition(position);

    sf::Vector2f screenCenter(screenWidth / 2.f, screenHeight / 2.f);
    sf::Vector2f direction = screenCenter - position;

    float magnitude = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    direction /= magnitude;
    
    movementSpeed = 0.1f;
    velocity = direction * movementSpeed;

    world.add(std::make_unique<PowerUP>(*this));
}

void PowerUP::update(float deltaTime)
{
    sprite.move(velocity * deltaTime * 1000.f);
}


void PowerUP::onCollide(Entity& other)
{
    switch (other.getType())
    {
    case Type::Player:
        toRemove = true;
        break;
    default:
        break;
    }

}