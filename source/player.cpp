#include "../include/player.h"
#include<iostream>

Player::Player(World& world, sf::RenderWindow& window) :
	Entity(ResourceManager::Texture::PlayerShip, Type::Player, world, 5, 5), frictionFactor(0.3f), showFlash(false), shotsInRowLeft(5), healthAnimation(0,0,17,17), previousHealth(5),
	point(Options::width / 6, Options::height / 2), window(window)
{
	acceleration = 400.f; 
	maxVelocity = {100.f, 100.f};
	friction = { 0.f,0.f };
	angle = 0;

	sf::Texture& flashTexture = ResourceManager::textures.at(ResourceManager::Texture::Flash);
	sf::Texture& healthTexture = ResourceManager::textures.at(ResourceManager::Texture::Health);

	flash.setTexture(flashTexture);
	flash.setOrigin(flashTexture.getSize().x/2, flashTexture.getSize().y/2);
	flash.setScale(0.7, 0.7);

	health.setTexture(healthTexture);
	health.setScale(2.f, 2.f);
	health.setTextureRect(healthAnimation);
	health.setOrigin(healthAnimation.getSize().x / 2, healthAnimation.getSize().y);
	health.setPosition(world.getWidth() - 25.f, 50.f);

	sf::SoundBuffer& shotSoundBuffer1 = ResourceManager::sounds.at(ResourceManager::Sound::Shot1);
	sf::SoundBuffer& shotSoundBuffer2 = ResourceManager::sounds.at(ResourceManager::Sound::Shot2);

	shotSound1.setBuffer(shotSoundBuffer1);
	shotSound2.setBuffer(shotSoundBuffer2);
	shotSound1.setVolume(1);
	shotSound2.setVolume(1);

	setPosition(-100.f, Options::height/2);

	world.add(std::make_unique<Player>(*this));


}

void Player::shoot()
{
	sf::Vector2f shootPosition = getPosition();

	sf::Vector2f shootDirection = sf::Vector2f(cos(angle * (M_PI / 180)), sin(angle * (M_PI / 180)));


	float x = shootPosition.x + radius * cos(a);
	float y = shootPosition.y + radius * sin(a);

	flash.setPosition(x,y);
	flash.setRotation(angle);
	
	shootPosition += shootDirection * 10.f;

	showFlash = true;

	int isBig = Random::GenerateInt(0, 1);
	std::unique_ptr<Shoot> shoot_ptr = std::make_unique<Shoot>(world, shootPosition, shootDirection, angle, true, false, static_cast<bool>(isBig));
	world.add(std::move(shoot_ptr));

	flashTimer.restart();

}

void Player::updateInput(float deltaTime)
{
	sf::Vector2f accelerationVec(0.f, 0.f);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		accelerationVec.y += acceleration;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		accelerationVec.y -= acceleration;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		accelerationVec.x -= acceleration;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		accelerationVec.x += acceleration;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) 
	{
		angle-=3.f;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
	{
		angle+=3.f;
	}

	accelerationVec *= frictionFactor;

	velocity += (accelerationVec * deltaTime) - friction;

	friction = velocity * 0.01f;

	sprite.move(velocity * deltaTime);

	sprite.setRotation(angle);

	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		shootButtonPressed = false;
		shotsInRowLeft = 4;
	}

	if ((!shootButtonPressed || shotsInRowLeft>0) && (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || sf::Mouse::isButtonPressed(sf::Mouse::Left)) && shootCooldown.getElapsedTime().asSeconds() >= 0.2)
	{
		shotSound1.stop();
		shotSound2.stop();

		shoot();

		int choice = Random::GenerateInt(0, 1);

		if (static_cast<bool>(choice))
			shotSound1.play();
		else
			shotSound2.play();

		shootCooldown.restart();

		shootButtonPressed = true;

		shotsInRowLeft--;
	}


	if (showFlash && flashTimer.getElapsedTime().asSeconds() >= 0.05)
	{
		showFlash = false;
	}

	sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));
	sf::Vector2f direction = mousePosition - getPosition();

	float targetAngle = atan2(direction.y, direction.x) * 180 / M_PI;

	angle = targetAngle;

	sprite.setRotation(angle);


}

void Player::changeHealthPicture()
{
	if (previousHealth!=currentHealth)
	{
		//healthAnimation.left += healthAnimation.width;

		switch (currentHealth)
		{
		case 5:
			health.setTextureRect({0,0,17,17});
			break;
		case 4:
			health.setTextureRect({ 17,0,17,17 });
			break;
		case 3:
			health.setTextureRect({ 34,0,17,17 });
			break;
		case 2:
			health.setTextureRect({ 51,0,17,17 });
			break;
		case 1:
			health.setTextureRect({ 68,0,17,17 });
			break;
		case 0:
			health.setTextureRect({ 85,0,17,17 });
			break;
		default:
			break;
		}

		previousHealth = currentHealth;
	}
}

void Player::onCollide(Entity& other)
{
	switch (other.getType())
	{
	case Type::SmallAsteroid: case Type::EnemyBullet: case Type::BigAsteroid: case Type::Boss:
		//toRemove = true;
		//std::cout << currentHealth;
		hited = true;
		break;
	case Type::PowerUP:
		if (this->currentHealth < 5) {
			this->currentHealth++;
			changeHealthPicture();
		}
		break;
	default:
		break;
	}
}

void Player::update(float deltaTime)
{
	switch (isAppearing)
	{
	case false:
		updateInput(deltaTime);

		whenHit();

		changeHealthPicture();

		onDestroy();

		break;
	case true:
		appear(deltaTime);
		break;
	}

}

void Player::appear(float deltaTime)
{
	sf::Vector2f direction = point - getPosition();
	float distance = sqrt(direction.x * direction.x + direction.y * direction.y);

	if (distance > 0) 
	{ 
		direction /= distance; 
		sf::Vector2f velocity = direction * introSpeed;
		introSpeed -= 2;

		sprite.move(velocity * deltaTime);
		sprite.setRotation(atan2(direction.y, direction.x) * 180 / M_PI);
	}

	if (distance < 5.f) {
		isAppearing = false;
		world.playerReady();
	}
}



void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	//target.draw(sprite, states);
	Entity::draw(target, states);


	if (showFlash)
	{
		target.draw(flash, states);

	}

	if (currentHealth >= 1 && !isAppearing)
	{
		target.draw(health, states);
	}
}
	
