#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include<unordered_map>
#include<SFML/Graphics/Texture.hpp>
#include<SFML/Audio.hpp>

class ResourceManager
{
private:
	static void initializeTextures();
	static void initializeSounds();
public:

	enum class Texture
	{
		PlayerShip,
		BackgroundImage,
		ShootSmall,
		ShootBig,
		Stars,
		Spark,
		Flash,
		AsteroidSmall,
		AsteroidBig,
		Enemy,
		EnemyShotSmall,
		EnemyShotBig,
		Hit,
		Explosion,
		Health,
		Teleport,
		Cross
	};

	enum class Sound
	{
		Shot1,
		Shot2,
		Hit,
		EnemyShot1,
		EnemyShot2,
		Explosion,
		Teleport,
		Hover,
		Accept,
		Ready,
		Set,
		Go
	};

	static std::unordered_map<Texture, sf::Texture> textures;
	static std::unordered_map<Sound,sf::SoundBuffer> sounds;

	static void initialize();
};


#endif RESOURCEMANAGER_H