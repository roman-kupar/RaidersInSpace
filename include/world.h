	#ifndef WORLD_H
	#define WORLD_H

	#include "entity.h"
	#include "options.h"
	#include "collision.h"
	#include <SFML/Graphics.hpp>

	//class Entity;

	// class World contains all entities in the game (player, shots, enemies,...) and manages them.
	class World : public sf::Drawable
	{
	public:
		World(unsigned int width, unsigned int height);
		~World();

		void sparkingAnimation();

		void playExplosionSound();

		void update(float deltaTime);
		void updateScore(unsigned int points);

		void add(std::unique_ptr<Entity> entity);

		void checkCollision();

		void playerReady();

		int getWidth() const;
		int getHeight() const;
		unsigned int getScore() const;

		sf::Vector2f getPlayerPosition() const;
		//sf::Vector2f getMousePosition() const;

		//Entity* getPlayer();
		//void setPlayer(Entity* player);
		bool isPlayerDead() const;

		std::list<std::unique_ptr<Entity>>& getEntities();

		void clearEntities();

		void clearAllEntites();

	private:
		sf::Sprite background, spark;
		sf::IntRect sparkAnimation;
		sf::Clock animateSpark, sparkWillHappen;
		sf::Sound explosionSound;

		std::vector<sf::Sound> explosionSounds;
		unsigned int currentSoundIndex, score;
		bool playerIsAppearing;

		Entity* player;

		sf::SoundBuffer& explosionSoundBuffer;

		const int sparkStep[8] = {80,60,60,70,80,77,81};
		int sparkIterator;

		std::list<std::unique_ptr<Entity>> entities;
		std::list<std::unique_ptr<sf::Sprite>> stars;

		const unsigned int width;
		const unsigned int height;

		void checkEntityOutOfBounds(Entity& entity) const;

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	};

	#endif WORLD_H	
