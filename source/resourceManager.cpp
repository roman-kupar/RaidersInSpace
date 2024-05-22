#include"../include/resourceManager.h"
#include<iostream>

std::unordered_map<ResourceManager::Texture, sf::Texture> ResourceManager::textures;
std::unordered_map<ResourceManager::Sound, sf::SoundBuffer> ResourceManager::sounds;


void ResourceManager::initializeTextures()
{
	sf::Texture player, backgroud,shoot1,shoot2,stars,spark,flash, asteroidSmall, asteroidBig, 
		enemy, 
		enemyShot1, 
		enemyShot2, hit, 
		explosion,
		health, 
		teleport, 
		assets,
		cross;

	if (!player.loadFromFile("resources/player/player1.png") ||
		!backgroud.loadFromFile("resources/bg-preview-big.png") ||
		!shoot1.loadFromFile("resources/projectiles/shoot1.png") ||
		!shoot2.loadFromFile("resources/projectiles/shoot2.png") ||
		!stars.loadFromFile("resources/bg-stars.png") || 
		!spark.loadFromFile("resources/sparks.png") ||
		!flash.loadFromFile("resources/projectiles/flash.png") || 
		!asteroidSmall.loadFromFile("resources/asteroid/asteroid-small.png") ||
		!asteroidBig.loadFromFile("resources/asteroid/asteroid.png") ||
		!enemy.loadFromFile("resources/enemy/enemy.png") ||
		!enemyShot1.loadFromFile("resources/projectiles/enemy-shot1.png") || 
		!enemyShot2.loadFromFile("resources/projectiles/enemy-shot2.png") ||
		!hit.loadFromFile("resources/effects/hit.png") ||
		!explosion.loadFromFile("resources/effects/explosion.png") ||
		!health.loadFromFile("resources/health/animated/border/heart_animated_2.png") ||
		!teleport.loadFromFile("resources/teleport/teleport.png") ||
		!cross.loadFromFile("resources/assets/crossair1.png"))
		std::cout << "Unable to load images";
	
	player.setSmooth(true);

	textures.insert(std::make_pair(Texture::PlayerShip,player));
	textures.insert(std::make_pair(Texture::BackgroundImage,backgroud));
	textures.insert(std::make_pair(Texture::ShootBig, shoot1));
	textures.insert(std::make_pair(Texture::ShootSmall, shoot2));
	textures.insert(std::make_pair(Texture::Stars, stars));
	textures.insert(std::make_pair(Texture::Spark, spark));
	textures.insert(std::make_pair(Texture::Flash, flash));
	textures.insert(std::make_pair(Texture::AsteroidSmall, asteroidSmall));
	textures.insert(std::make_pair(Texture::AsteroidBig, asteroidBig));
	textures.insert(std::make_pair(Texture::Enemy, enemy));
	textures.insert(std::make_pair(Texture::EnemyShotSmall, enemyShot1));
	textures.insert(std::make_pair(Texture::EnemyShotBig, enemyShot2));
	textures.insert(std::make_pair(Texture::Hit, hit));
	textures.insert(std::make_pair(Texture::Explosion, explosion));
	textures.insert(std::make_pair(Texture::Health, health));
	textures.insert(std::make_pair(Texture::Teleport, teleport));
	textures.insert(std::make_pair(Texture::Cross, cross));

}

void ResourceManager::initializeSounds()
{
	sf::SoundBuffer shot1, shot2, hit, enemyShot1, enemyShot2, explosion, teleport, hoverButton, acceptButton, ready, set, go;

	if (!shot1.loadFromFile("resources/projectiles/shot1.wav") ||
		!shot2.loadFromFile("resources/projectiles/shot2.wav") ||
		!hit.loadFromFile("resources/effects/hit.wav") ||
		!enemyShot1.loadFromFile("resources/enemy/LaserGun.wav") ||
		!enemyShot2.loadFromFile("resources/enemy/LaserGun2.wav") ||
		!explosion.loadFromFile("resources/effects/explosion.wav") ||
		!teleport.loadFromFile("resources/teleport/teleport.wav") ||
		!hoverButton.loadFromFile("resources/buttons/Retro8.wav") ||
		!acceptButton.loadFromFile("resources/buttons/Retro10.wav") ||
		!ready.loadFromFile("resources/effects/Retro5.wav") ||
		!set.loadFromFile("resources/effects/Retro6.wav") || 
		!go.loadFromFile("resources/effects/Retro7.wav"))
	{
		std::cout << "Unable to load sounds";
	}

	sounds.insert(std::make_pair(Sound::Shot1, shot1));
	sounds.insert(std::make_pair(Sound::Shot2, shot2));
	sounds.insert(std::make_pair(Sound::Hit, hit));
	sounds.insert(std::make_pair(Sound::EnemyShot1, enemyShot1));
	sounds.insert(std::make_pair(Sound::EnemyShot2, enemyShot2));
	sounds.insert(std::make_pair(Sound::Explosion, explosion));
	sounds.insert(std::make_pair(Sound::Teleport, teleport));
	sounds.insert(std::make_pair(Sound::Hover, hoverButton));
	sounds.insert(std::make_pair(Sound::Accept, acceptButton));
	sounds.insert(std::make_pair(Sound::Ready, ready));
	sounds.insert(std::make_pair(Sound::Set, set));
	sounds.insert(std::make_pair(Sound::Go, go));
}

void ResourceManager::initialize()
{
	initializeTextures();
	initializeSounds();
}
	