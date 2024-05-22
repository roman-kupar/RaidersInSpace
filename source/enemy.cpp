#include<iostream>
#include"../include/enemy.h"
Enemy::Enemy(World& world, const sf::Vector2f& point) :
	Entity(ResourceManager::Texture::Enemy, Type::Enemy, world, 2, 2), enemyAnimation(0,0,29,29), teleportAnimation(0,0,34,34), animationON(false), isTeleporting(true), 
	toPlayTeleportSound(true), toPlayShotSound(true)

	//shotSoundBuffer1(ResourceManager::sounds.at(ResourceManager::Sound::EnemyShot1)),
	//shotSoundBuffer2(ResourceManager::sounds.at(ResourceManager::Sound::EnemyShot2))
{
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

	sprite.setTextureRect(enemyAnimation);

	sprite.setOrigin(enemyAnimation.width/2,enemyAnimation.height/2);
	
	sprite.setPosition(point);

	world.add(std::make_unique<Enemy>(*this));

	movementSpeed = 0.1f;


	//sf::SoundBuffer& shotSoundBuffer1 = ResourceManager::sounds.at(ResourceManager::Sound::EnemyShot1);
	//sf::SoundBuffer& shotSoundBuffer2 = ResourceManager::sounds.at(ResourceManager::Sound::EnemyShot2);

	//shotSound1.setBuffer(shotSoundBuffer1);
	//shotSound2.setBuffer(shotSoundBuffer2);

	//sf::SoundBuffer& shotSoundBuffer2 = ResourceManager::sounds.at(ResourceManager::Sound::Shot2);

	//shotSound1.setLoop(true);
	//shotSound1.setVolume(100);
	//shotSound2.setVolume(100);

	timeToFlip.restart();
	timeToAct.restart();
}

void Enemy::update(float deltaTime)
{
	switch(isTeleporting)
	{
	case true:
		animateTeleportation();
		break;
	case false:
		moveTowardsTarget(deltaTime);
		act();
		whenHit();
		onDestroy();
		break;
	}
	
}

void Enemy::act()
{
	if (timeToAct.getElapsedTime().asSeconds() >= 1.f && shotTimes < 2)
	{

		animate();
		shoot();
	}

	if (shotTimes >= 2)
	{
		//shotSound1.stop();
		//shotSound2.stop();
		shotTimes = 0;
		timeToAct.restart();
	}
}

void Enemy::moveTowardsTarget(float deltaTime)
{
	sf::Vector2f playerPosition = world.getPlayerPosition();
	sf::Vector2f direction = playerPosition - sprite.getPosition();
	float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

	float adjustedSpeed = movementSpeed*0.1f;

	if (length > 0)
	{
		direction.x /= length;
		direction.y /= length;

		angle = std::atan2(direction.y, direction.x) * (180 / M_PI); 
		angle += 180;
		sprite.setRotation(angle);

		sprite.move(direction * adjustedSpeed * deltaTime);
	}

}

void Enemy::shoot()
{
	if (flipCounter >= 5)
	{
		sf::Vector2f shootPosition = getPosition();

		float angleRadians = angle * (M_PI / 180);
		sf::Vector2f shootDirection = sf::Vector2f(cos(angleRadians), sin(angleRadians));

		shootPosition.y += shootFromWing;
		shootFromWing = -shootFromWing;
		int isBig = Random::GenerateInt(0, 1);

		std::unique_ptr<Shoot> shoot_ptr = std::make_unique<Shoot>(world, shootPosition, -shootDirection, angle, false, static_cast<bool>(isBig));

		world.add(std::move(shoot_ptr));

		int choice = Random::GenerateInt(0, 1);

		if (static_cast<bool>(choice))
			shotS1.play();
		else
			shotS2.play();


		flipCounter = 0;
		shotTimes++;
	}
}


void Enemy::animate()
{
	if (animationON)
	{
		if (timeToFlip.getElapsedTime().asSeconds() > 0.09f)
		{
			enemyAnimation.left += flipEnemy;

			if (enemyAnimation.left >= 145 || enemyAnimation.left < 0)
				flipEnemy = -flipEnemy;
			else {
				sprite.setTextureRect(enemyAnimation);
			}

			flipCounter++;
			timeToFlip.restart();
		}
	}
}

void Enemy::animateTeleportation()
{
	if (timeToFlip.getElapsedTime().asSeconds() > 0.1f)
	{

		if (toPlayTeleportSound)
		{
			teleportSound.play();
			std::cout << "playing";
			toPlayTeleportSound = false;
		}

		teleportAnimation.left += flipTeleport;
		teleport.setTextureRect(teleportAnimation);
		teleport.setPosition(getPosition().x, getPosition().y);

		if (teleportAnimation.left >= 204)
		{
			isTeleporting = false;
			animationON = true;
			//teleportSound.stop();
		}

		timeToFlip.restart();
	}
}

void Enemy::onCollide(Entity& other)
{
	switch(other.getType())
	{
	case Type::SmallAsteroid: case Type::BigAsteroid:
		hited = true;
		break;
	case Type::PlayerBullet:
		hited = true;
		world.updateScore(10);
	}
}

void Enemy::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (!animationON)
	{
		target.draw(teleport, states);
	}
	else 
	{
		Entity::draw(target, states);
	}

}
