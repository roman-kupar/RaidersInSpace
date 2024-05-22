#ifndef PLAYER_H
#define PLAYER_H

#include"entity.h"
#include"shoot.h"
#include"world.h"

class Player : public Entity
{
private:
	sf::Clock shootCooldown;
	sf::Vector2f maxVelocity; 
	const sf::Vector2f point;
	const float frictionFactor;

	sf::Sprite flash, health;
	bool showFlash, shootButtonPressed = false, isAppearing = true;
	sf::Clock flashTimer;


	float radius = 5.f;
	float angularSpeed = 0.05;
	float a = 0;
	float introSpeed = 250.f;

	sf::Sound shotSound1, shotSound2;

	unsigned int shotsInRowLeft, previousHealth;

	sf::IntRect healthAnimation;

	sf::RenderWindow& window;

public:
	Player(World& world, sf::RenderWindow& window);

	void shoot();

	void updateInput(float deltaTime);
	void appear(float deltaTime);

	virtual void update(float deltaTime);
	virtual void onCollide(Entity& other) override;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void changeHealthPicture();
};

#endif	
