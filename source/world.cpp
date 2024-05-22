#include "../include/world.h"
#include<iostream>

World::World(unsigned int width, unsigned int height)
	: width(width), height(height), sparkAnimation(30,45,69, 69), sparkIterator(0),	explosionSoundBuffer(ResourceManager::sounds.at(ResourceManager::Sound::Explosion)),
	currentSoundIndex(0), playerIsAppearing(true), score(0)

{	
	explosionSound.setBuffer(explosionSoundBuffer);

	for (size_t i = 0; i < 5; ++i) {
		sf::Sound sound;
		sound.setBuffer(explosionSoundBuffer);
		sound.setVolume(5);
		explosionSounds.push_back(sound);
	}

    sf::Texture& textureBack = ResourceManager::textures.at(ResourceManager::Texture::BackgroundImage);
	textureBack.setSmooth(true);
	background.setTexture(textureBack);
	background.setOrigin(textureBack.getSize().x / 2.0f, textureBack.getSize().y / 2.0f);
	background.setPosition(width / 2.0f, height / 2.0f);

	sf::Texture& textureStars = ResourceManager::textures.at(ResourceManager::Texture::Stars);
	textureStars.setRepeated(true);
	textureStars.setSmooth(true);

	sf::Texture& textureSpark = ResourceManager::textures.at(ResourceManager::Texture::Spark);
	textureSpark.setSmooth(true);
	spark.setTexture(textureSpark);
	spark.setScale(1.25, 1.25);
	spark.setTextureRect(sparkAnimation);	
	spark.setOrigin(60,69);
	spark.setPosition(float(width)/2.f - 6.f, float(height)/2.f + 50.f);

	float randomX, randomY;
	
	for (size_t i = 0;i<30;i++)
	{
		randomX = Random::GenerateFloat(0.f, float(width));
		randomY = Random::GenerateFloat(0.f, float(height));
		std::unique_ptr<sf::Sprite> star = std::make_unique<sf::Sprite>();
		star.get()->setTexture(textureStars);
		star.get()->setOrigin(textureStars.getSize().x / 2.f, textureStars.getSize().y / 2.f);
		star.get()->setPosition(randomX, randomY);
		stars.push_back(std::move(star));
	}

	if (Options::fullscreenOn)
		background.setScale(float(width)/float(STANDART_WIDTH),float(height)/float(STANDART_HEIGHT));
}

World::~World()	
{
}

void World::sparkingAnimation()
{
	if (sparkWillHappen.getElapsedTime().asSeconds() >= 5.f)
	{
		if (animateSpark.getElapsedTime().asSeconds() >= 0.05f)
		{
			if (sparkAnimation.left >= 500)
			{
				sparkAnimation.left = 33;
				sparkAnimation.top += 100;
				sparkIterator = 0;
				sparkWillHappen.restart();

				if (sparkAnimation.top >= 245)
				{
					sparkAnimation.top = 45;
					sparkAnimation.left = 30;
				}
			}
			else
			{
				sparkAnimation.left += sparkStep[sparkIterator];
				sparkIterator++;
			}

			spark.setTextureRect(sparkAnimation);

			animateSpark.restart();
		}
	}
}

void World::playExplosionSound() {
	for (unsigned int i = 0; i < explosionSounds.size(); ++i) {
		if (explosionSounds[i].getStatus() != sf::Sound::Playing) {
			explosionSounds[i].play();
			break;
		}
	}
}

void World::update(float deltaTime)
{
	for (auto& starPtr : stars)
	{
		sf::Sprite& star = *starPtr;
		star.move(-200.f * deltaTime, 0.f);

		if (star.getPosition().x <= 0.f)
		{
			float randomY = Random::GenerateFloat(0.f, float(height));
			star.setPosition(width, randomY);
		}
	}

	auto& entitiesRef = entities; 
	entities.erase(std::remove_if(entities.begin(), entities.end(), [this, &deltaTime, &entitiesRef](const auto& entityPtr) {
		Entity& entity = *entityPtr;

		entity.update(deltaTime);
		if (!playerIsAppearing)
			checkEntityOutOfBounds(entity);
		if (entity.toPlayExplodeSound)
		{
			playExplosionSound();
			entity.toPlayExplodeSound = false;
		}
		return entityPtr->toRemove;
		}), entities.end());

	sparkingAnimation();

	checkCollision();
}

void World::updateScore(unsigned int points)
{
	score += points;
}

void World::add(std::unique_ptr<Entity> entity)
{
	if (Options::fullscreenOn)
	{
		float factorX = width / float(STANDART_WIDTH);
		float factorY = height / float(STANDART_HEIGHT);

		Entity* copy = entity.get();

		sf::Vector2u spriteSize = copy->getSpriteSize();

		copy->setScaleFactor(factorX, factorY);
	}

	entities.push_back(std::move(entity));

}

void World::checkCollision()
{
	for (auto it1 = entities.begin(); it1 != entities.end(); ++it1)
	{
		for (auto it2 = std::next(it1); it2 != entities.end(); ++it2)
		{
			Entity& entity1 = *(*it1);
			Entity& entity2 = *(*it2);

			if (Collision::CheckCircleCollision(entity1.getSprite(), entity2.getSprite()))
			{
				entity1.onCollide(entity2);
				entity2.onCollide(entity1);

				Collision::resolveCollision(entity1, entity2);
			}
		}
	}
}

void World::playerReady()
{
	playerIsAppearing = false;
}

//bool World::IsEntityCollideWithOthers(const Entity& entity) const
//{
//	for (const auto& entityPtr : entities)
//		if (entity.IsCollideWith(*entityPtr))
//			return true;
//
//	return false;
//}

int World::getWidth() const
{
	return width;
}

int World::getHeight() const
{
	return height;
}

unsigned int World::getScore() const
{
	return score;
}

sf::Vector2f World::getPlayerPosition() const
{
	for (const auto& entityPtr : entities) {
		if (entityPtr && entityPtr->getType() == Entity::Type::Player) {
			return entityPtr->getPosition();
		}
	}
	return sf::Vector2f(0.f, 0.f);
}

bool World::isPlayerDead() const
{
	for (const auto& entityPtr : entities) {
		if (entityPtr && entityPtr->getType() == Entity::Type::Player) {
			return false;
		}
	}

	return true;
}

std::list<std::unique_ptr<Entity>>& World::getEntities()
{
	return entities;
}


//Entity* World::getPlayer() 
//{
//
//	throw std::logic_error("Player entity not found in the world."); 
//}

//void World::setPlayer(Entity* player)
//{
//	this->player = player;
//}


void World::clearEntities() {
	entities.erase(std::remove_if(entities.begin(), entities.end(), [](const auto& entityPtr) {
		return entityPtr->getType() != Entity::Type::Player;
		}), entities.end());
}

void World::clearAllEntites()
{
	playerIsAppearing = true;
	score = 0;
	entities.clear();
}

void World::checkEntityOutOfBounds(Entity& entity) const
{

	sf::Vector2f position = entity.getPosition();

	if (entity.getType() == Entity::Type::BigAsteroid || entity.getType() == Entity::Type::SmallAsteroid)
		return;

	if (position.x < 0)
		position.x = float(width);
	else if (position.x > width)
		position.x = 0;

	if (position.y < 0)
		position.y = float(height);
	else if (position.y > height)
		position.y = 0;

	entity.setPosition(position);
}


void World::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(background);

	if (sparkWillHappen.getElapsedTime().asSeconds()>5.f)
		target.draw(spark);

	for (const auto& star : stars)
		target.draw(*star, states);

	for (auto entity = entities.rbegin(); entity != entities.rend(); ++entity)
		target.draw(*(*entity), states);
	
}