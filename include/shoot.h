#ifndef SHOOT_H
#define SHOOT_H

#include"entity.h"
#include"world.h"

class Shoot : public Entity
{
private:
	sf::Vector2f direction;
public:
	Shoot(World& world, sf::Vector2f position, sf::Vector2f shootDirection, float angle, bool isPlayer, bool isBoss, bool isBigShoot);

	void update(float deltaTime) override;

	virtual void onCollide(Entity& other) override;

	//virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

};
#endif