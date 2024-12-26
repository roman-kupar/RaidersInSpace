#ifndef BOSS_H
#define BOSS_H

#include "entity.h"
#include "enemy.h"
#include"shoot.h"
#include"world.h"

class Boss : public Entity
{
private:
	sf::Sprite health, health_bar, teleport;
	sf::Sound shotS1, shotS2, teleportSound;
	sf::IntRect teleportAnimation;
	sf::Clock timeToAct;
	
	bool timeToShootBig;
	float offset_y;
	float offset_x;
	float shootFromWing = 75.f;
	short int shotTimes = 0;
	enum class Stage
	{
		firstStage,
		secondStage,
		finalStage
	};

	Stage currentStage;

public:
	Boss(World& world, const sf::Vector2f& point);

	virtual void update(float deltaTime);
	virtual void onCollide(Entity& other) override;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	
	void move(float deltaTime);
	void shoot();
	void act();
	void checkHealth();
	void bigShoot();
	void createEnemy();
};

#endif BOSS_H
