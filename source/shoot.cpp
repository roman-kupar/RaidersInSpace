#include"../include/shoot.h"

Shoot::Shoot(World& world, sf::Vector2f position, sf::Vector2f shootDirection, float angle, bool isPlayer, bool isBigShoot) : 
    Entity(ResourceManager::Texture::ShootSmall, Type::PlayerBullet, world, 1,1), direction(shootDirection)
{
    if (!isPlayer)
    {
        if (isBigShoot)
            changeTexture(ResourceManager::Texture::EnemyShotBig);
        else
            changeTexture(ResourceManager::Texture::EnemyShotSmall);

        changeType(Type::EnemyBullet);
    }
    else if (isBigShoot)
        changeTexture(ResourceManager::Texture::ShootBig);

    

    movementSpeed = 400.f;

    sprite.setPosition(position);
    sprite.setRotation(angle);
}

void Shoot::update(float deltaTime)
{
    sf::Vector2f movement = direction * movementSpeed * deltaTime;
    sprite.move(movement);

    movementSpeed += 50.f * deltaTime;

    if (sprite.getPosition().x >= world.getWidth() || sprite.getPosition().x <= 0 ||
        sprite.getPosition().y >= world.getHeight() || sprite.getPosition().y <= 0)
    {
        toRemove = true;
    }

}

void Shoot::onCollide(Entity& other)
{
    switch (other.getType())
    {
    case Type::SmallAsteroid: case Type::PlayerBullet: case Type::EnemyBullet: case Type::BigAsteroid:
        toRemove = true;
        break;
    case Type::Enemy:
        if (type != Type::EnemyBullet)
            toRemove = true;
        break;
    case Type::Player:
        if (type != Type::PlayerBullet)
            toRemove = true;
        break;
    default:
        break;
    }
    
}

//void Shoot::draw(sf::RenderTarget& target, sf::RenderStates states) const
//{
//    target.draw(sprite, states);
//}
