#ifndef ASTEROID_H
#define ASTEROID_H

#include"entity.h"
#include"world.h"
#include"collision.h"


class Asteroid : public Entity
{
private:
	static constexpr float MASS = 100.0f;
	sf::Vector2f collisionNormal, reflectionDirection, currentVelocity, startPosition;
	bool isInsideWindow, isNotInsideWindowAnymore;
	float length, product;
	const float minimumDistance = 100.0f;
public:
	Asteroid(World& world);

	void setVelocity();

	sf::Vector2f randomOffscreenPosition();

	float randomInitialVelocityMagnitude();

	sf::Vector2f randomDirectionTowardsScreen();

	virtual void update(float deltaTime);
	virtual void onCollide(Entity& other) override;

	float dotProduct(const sf::Vector2f& v1, const sf::Vector2f& v2);

	bool checkOverlap(const sf::Vector2f& position);
	float distance(const sf::Vector2f& p1, const sf::Vector2f& p2);


};
#endif