#include "../include/entity.h"
#include <iostream>

Entity::Entity(ResourceManager::Texture textureID, Type type, World& world, unsigned int healthMax, unsigned int currentHealth)
    : type(type), world(world), toRemove(false), hited(false), toExplode(false), hitIterator(0), explosionIterator(0), currentHealth(currentHealth), toPlayExplodeSound(false), ExplodeSoundIsNotPlaying(true)
{
    const sf::Texture& texture = ResourceManager::textures.at(textureID);
    const sf::Texture& hitTexture = ResourceManager::textures.at(ResourceManager::Texture::Hit);
    const sf::Texture& explosionTexture = ResourceManager::textures.at(ResourceManager::Texture::Explosion);

    sprite.setTexture(texture);
    sprite.setScale({ 1.75f, 1.75f });
    sprite.setOrigin(texture.getSize().x / 2.0f, texture.getSize().y / 2.0f);
    spriteSize = texture.getSize();

    hit.setTexture(hitTexture);
    hit.setScale(1.75f, 1.75f);
    hitAnimation = sf::IntRect(0, 0, 17, 16);
    hit.setTextureRect(hitAnimation);
    hit.setOrigin(hitAnimation.width / 2.f, hitAnimation.height / 2.f);

    explosion.setTexture(explosionTexture);
    explosion.setScale(1.75f, 1.75f);
    explosionAnimation = sf::IntRect(0, 0, 32, 32);
    explosion.setTextureRect(explosionAnimation);
    explosion.setOrigin(explosionAnimation.width / 2.f, explosionAnimation.height / 2.f);

    sf::SoundBuffer& hitSoundBuffer = ResourceManager::sounds.at(ResourceManager::Sound::Hit);
    hitSound.setBuffer(hitSoundBuffer);
    hitSound.setVolume(1);

    sf::SoundBuffer& explosionSoundBuffer = ResourceManager::sounds.at(ResourceManager::Sound::Explosion);
    explosionSound.setBuffer(explosionSoundBuffer);
    explosionSound.setVolume(100);
}

Entity::Type Entity::getType() const
{
    return type;
}

const sf::Sprite& Entity::getSprite() const
{
    return sprite;
}

Entity::~Entity() {}

const sf::Vector2f& Entity::getPosition() const
{
    return sprite.getPosition();
}

const sf::Vector2u Entity::getSpriteSize() const
{
    return spriteSize;
}

void Entity::onDestroy()
{
    if (toExplode)
    {
        
        if (ExplodeSoundIsNotPlaying)
        {
            toPlayExplodeSound = true;
            ExplodeSoundIsNotPlaying = false;
        }

        if (toFlipExplode.getElapsedTime().asSeconds() > 0.07f)
        {
            animateDestroy();
            explosion.setPosition(getPosition().x, getPosition().y);
            explosionIterator++;

            if (explosionIterator >= 4)
            {
                explosionIterator = 0;
                toRemove = true;
                toExplode = false;
            }

            toFlipExplode.restart();
        }
    }
    if (!toExplode)
    {
    }
}

void Entity::animateDestroy()
{
    explosionAnimation.left = explosionIterator * explosionAnimation.width;

    if (explosionAnimation.left >= 190)
        explosionAnimation.left = 0;

    explosion.setTextureRect(explosionAnimation);

    if (type == Type::BigAsteroid)
        explosion.setScale(3.f, 3.f);

    explosion.setOrigin(explosionAnimation.width / 2.f, explosionAnimation.height / 2.f);
}

void Entity::animateHit()
{
    hitAnimation.left = hitIterator * hitAnimation.width;

    if (hitAnimation.left >= 80)
        hitAnimation.left = 0;

    hit.setTextureRect(hitAnimation);

    hit.setOrigin(hitAnimation.width / 2.f, hitAnimation.height / 2.f);
}

void Entity::whenHit()
{
    if (hited)
    {
        hitSound.play();

        if (toFlipHit.getElapsedTime().asSeconds() > 0.04f)
        {
            animateHit();
            hit.setPosition(getPosition().x, getPosition().y);
            hitIterator++;

            if (hitIterator >= 4)
            {
                hitIterator = 0;
                currentHealth--;
                hited = false;
            }

            toFlipHit.restart();
        }
    }
    if (!hited)
    {
        hitSound.stop();

        if (currentHealth <= 0)
        {
            if (type == Type::SmallAsteroid)
                toRemove = true;
            else
                toExplode = true;
        }
    }
}

void Entity::setScaleFactor(float factorX, float factorY)
{
    sprite.setScale(factorX, factorY);
}

void Entity::changeTexture(ResourceManager::Texture textureID)
{
    const sf::Texture& texture = ResourceManager::textures.at(textureID);
 
    sprite.setTexture(texture, true);
    sprite.setOrigin(texture.getSize().x / 2.0f, texture.getSize().y / 2.0f);
    spriteSize = texture.getSize();
}

void Entity::changeType(Type type)
{
    this->type = type;
}

void Entity::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(sprite);

    if (hited)
    {
        target.draw(hit, states);
    }

    if (toExplode)
    {
        target.draw(explosion, states);
    }
}
