#include "../include/boss.h"
#include<iostream>

Boss::Boss(World& world, const sf::Vector2f& point) : 
	Entity(ResourceManager::Texture::Enemy, Type::Boss, world, 99, 99)
{
	sf::IntRect rect(0, 0, 29, 29);

	explosion.setScale(10, 10);

	hit.setScale(5, 5);

	sprite.setTextureRect(rect);

	sprite.setPosition(point);

	sprite.setScale(7,7);

	sprite.setOrigin(sprite.getTextureRect().width/2, sprite.getTextureRect().height/2);

	offset_x = 0.1f;
	offset_y = 0.1f;

	angle = 0.f;

	currentStage = Stage::firstStage;

	timeToShootBig = false;

	const sf::Texture& teleportTexture = ResourceManager::textures.at(ResourceManager::Texture::Teleport);

	sf::SoundBuffer& shot1SBuffer = ResourceManager::sounds.at(ResourceManager::Sound::EnemyShot1);
	sf::SoundBuffer& shot2SBuffer = ResourceManager::sounds.at(ResourceManager::Sound::EnemyShot2);
	sf::SoundBuffer& teleportSoundBuffer = ResourceManager::sounds.at(ResourceManager::Sound::Teleport);
	teleportSound.setBuffer(teleportSoundBuffer);
	teleportSound.setVolume(1);
	teleportSound.setPitch(3.f);
	shotS1.setBuffer(shot1SBuffer);
	shotS1.setVolume(1);
	shotS2.setBuffer(shot2SBuffer);
	shotS2.setVolume(1);

	teleport.setTexture(teleportTexture);
	teleport.setScale({ 3.f,3.f });
	teleport.setTextureRect(teleportAnimation);
	teleport.setOrigin(teleportAnimation.width / 2.f, teleportAnimation.height / 2.f);
	teleport.setPosition(point);

	world.add(std::make_unique<Boss>(*this));

	movementSpeed = 0.1f;

	timeToAct.restart();
}

void Boss::update(float deltaTime)
{
	move(deltaTime);
	act();
	whenHit();
	checkHealth();

	if (toExplode)
		world.bossDies();

	onDestroy();
}

void Boss::onCollide(Entity& other)
{
	switch (other.getType())
	{
	case Entity::Type::PlayerBullet:
	{
		hited = true;
		world.updateScore(30);
		break;
	}

	}
}

void Boss::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	Entity::draw(target, states);
}

void Boss::move(float deltaTime)
{
    if ((sprite.getPosition().y >= Options::height - 90.f) || (sprite.getPosition().y <= 100.f))
    {
        offset_y = -offset_y; 
    }


	if ((sprite.getPosition().x >= Options::width - 80.f) || (sprite.getPosition().x <= Options::width - 200.f))
	{
		
		offset_x = -offset_x;
	}
    sprite.move(offset_x*deltaTime*1000.f, offset_y*deltaTime*1000.f);
}

void Boss::act()
{
	if (timeToAct.getElapsedTime().asSeconds() >= 2.f)
	{
		switch (currentStage)
		{
		case Stage::firstStage:
			shoot();
			shoot();
			break;
		case Stage::secondStage:
			shoot();
			shoot();
			shoot();
			shoot();
			break;
		case Stage::finalStage:
			if (timeToShootBig) {
				createEnemy();
				timeToShootBig = false;
			}
			else {
				shoot();
				shoot();
				shoot();
				shoot();
				timeToShootBig = true;
			}

			break;
		}
		timeToAct.restart();
	}
	//if (shotTimes >= 2)
	//{
	//	//shotSound1.stop();
	//	//shotSound2.stop();
	//	shotTimes = 0;
	//	timeToAct.restart();
	//}
}

void Boss::checkHealth()
{
	if (currentHealth <= 67 && currentStage == Stage::firstStage)
	{
		currentStage = Stage::secondStage;
		offset_y = 0.2f;
		offset_x = 0.2f;
		//std::cout << "Second stage";
	}
	else if (currentHealth <= 40 && currentStage == Stage::secondStage) {
		currentStage = Stage::finalStage;
		shootFromWing = 75.f;
		//std::cout << "Final stage";
	}

}

void Boss::bigShoot()
{
	sf::Vector2f shootPosition = getPosition();
	shootPosition.x -= 50.f;
	float angleRadians = angle * (M_PI / 180);
	sf::Vector2f shootDirection = sf::Vector2f(cos(angleRadians), sin(angleRadians));
}

void Boss::createEnemy()
{
	sf::Vector2f bossPosition = getPosition();

	float wingOffset = 75.f;
	sf::Vector2f leftWingPosition = bossPosition;
	sf::Vector2f rightWingPosition = bossPosition;

	leftWingPosition.y -= wingOffset;
	rightWingPosition.y += wingOffset;

	Enemy leftEnemy(world, leftWingPosition, true);
	Enemy rightEnemy(world, rightWingPosition, true);

}



void Boss::shoot()
{

	sf::Vector2f shootPosition = getPosition();

	shootPosition.x -= 50.f;
	float angleRadians = angle * (M_PI / 180);
	sf::Vector2f shootDirection = sf::Vector2f(cos(angleRadians), sin(angleRadians));

	if (currentStage == Stage::firstStage) {
		shootPosition.y += shootFromWing;
	}
	else if (currentStage == Stage::secondStage || currentStage == Stage::finalStage)
	{
		if (shootFromWing > 35)
		{
			shootFromWing -= 15.f;
			shootPosition.y += shootFromWing;
		}
		else if (shootFromWing < -35)
		{
			shootFromWing += 15.f;
			shootPosition.y += shootFromWing;
		}
		else if (shootFromWing <= 35 && shootFromWing >= -35)
		{
			shootFromWing = 75.f;
			shootPosition.y += shootFromWing;
		}
	}

	shootFromWing = -shootFromWing;

	int isBig = Random::GenerateInt(0, 1);

	std::unique_ptr<Shoot> shoot_ptr = std::make_unique<Shoot>(world, shootPosition, -shootDirection, angle, false, true, static_cast<bool>(isBig));

	world.add(std::move(shoot_ptr));

	int choice = Random::GenerateInt(0, 1);

	if (static_cast<bool>(choice))
		shotS1.play();
	else
		shotS2.play();

	shotTimes++;

}