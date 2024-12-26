#ifndef COLLISION_H
#define COLLISION_H

#include <SFML/Graphics.hpp>
#include"entity.h"

class Collision
{
public:
	Collision() = delete;

	static bool CheckCircleCollision(const sf::Sprite& first, const sf::Sprite& second);
	static void resolveCollision(Entity& first, Entity& second);

};

#endif