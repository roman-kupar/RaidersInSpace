#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "resourceManager.h"
#include "random.h"
#define _USE_MATH_DEFINES
#include <math.h>

class World;

class Entity : public sf::Drawable
{
public:
	enum class Type
	{
		Player,
		PlayerBullet,
		EnemyBullet,
		SmallAsteroid,
		BigAsteroid,
		Enemy
	};

	Entity(ResourceManager::Texture textureID, Type type, World& world, unsigned int healthMax, unsigned int currentHealth);
	virtual ~Entity();

	template <typename... Args>
	void setPosition(Args&&... args)
	{
		sprite.setPosition(std::forward<Args>(args)...);
	}

	const sf::Vector2f& getPosition() const;

	const sf::Vector2u getSpriteSize() const;

	virtual void update(float deltaTime) = 0;
	virtual void onCollide(Entity& other) = 0;

	
	void onDestroy();
	void animateDestroy();
	void animateHit();
	void whenHit();

	void setScaleFactor(float factorX, float factorY);

	template <typename... Args>
	void setHitPosition(Args&&... args)
	{
		sprite.setPosition(std::forward<Args>(args)...);
	}

	void changeTexture(ResourceManager::Texture textureID);
	void changeType(Type);

	Type getType() const;

	const sf::Sprite& getSprite() const;

	bool toRemove, hited, toExplode, toPlayExplodeSound, ExplodeSoundIsNotPlaying;
	unsigned int hitIterator, explosionIterator;

	void move(const sf::Vector2f& offset) {
		sprite.move(offset);
	}

protected:
	friend class Player;
	friend class Asteroid;
	friend class Bullet;
	friend class Enemy;
	friend class PowerUP;

	float acceleration = 0.f, movementSpeed = 500.f, angle = 0;
	sf::Vector2f velocity = { 0.f,0.f }, friction = { 0.f,0.f };

	sf::IntRect hitAnimation, explosionAnimation;
	sf::Clock toFlipHit, toFlipExplode;
	sf::Sprite sprite, hit, explosion;
    sf::Vector2u spriteSize;
	sf::Sound hitSound ,explosionSound;
	World& world;

	Type type;

	int healthMax,currentHealth;

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif

/*
	

	const sf::Sprite& GetSprite() const;

	virtual bool IsAlive() const;
	virtual bool IsCollideWith(const Entity& other) const = 0;


	protected
    friend class Meteor;
	friend class Player;
	friend class Saucer;
	friend class PlayerShot;
	friend class EnemyShot;

*/