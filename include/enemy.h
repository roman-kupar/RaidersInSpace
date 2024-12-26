#ifndef ENEMY_H
#define ENEMY_H

#include"entity.h"
#include"shoot.h"
#include"world.h"
#include<cmath>


class Enemy : public Entity
{
private:
	sf::IntRect enemyAnimation, teleportAnimation;
	sf::Clock timeToFlip, timeToAct;
	sf::Sound shotS1, shotS2, teleportSound;
	bool animationON, isTeleporting, toPlayTeleportSound, toPlayShotSound, createdByBoss;
	short int flipEnemy = 29, flipTeleport = 34, flipCounter = 0, shotTimes = 0;
	float radius = 5.f, a = 0, shootFromWing = 13.f;
	//sf::SoundBuffer& shotSoundBuffer1, shotSoundBuffer2/*, teleportSoundBuffer*/;
	sf::Sprite teleport;

public:

	Enemy(World& world, const sf::Vector2f& point, bool createdByBoss);
	virtual void update(float deltaTime);
	void act();
	void animate();
	void animateTeleportation();
	void moveTowardsTarget(float deltaTime);
	void moveTowardsPoint(const sf::Vector2f& targetPoint,float deltaTime);
	void shoot();
	virtual void onCollide(Entity& other) override;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};
#endif