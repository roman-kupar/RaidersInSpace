#include"../include/collision.h"

bool Collision::CheckCircleCollision(const sf::Sprite& first, const sf::Sprite& second)
{
	sf::Vector2f rectangle1(static_cast<float>(first.getTextureRect().width), static_cast<float>(first.getTextureRect().height));
	rectangle1.x *= first.getScale().x;
	rectangle1.y *= first.getScale().y;

	sf::Vector2f rectangle2(static_cast<float>(second.getTextureRect().width), static_cast<float>(second.getTextureRect().height));
	rectangle2.x *= second.getScale().x;
	rectangle2.y *= second.getScale().y;

	float radius1 = (rectangle1.x + rectangle1.y) / 4;
	float radius2 = (rectangle2.x + rectangle2.y) / 4;

	float horDistance = first.getPosition().x - second.getPosition().x;
	float vertDistance = first.getPosition().y - second.getPosition().y;

	return (horDistance * horDistance) + (vertDistance * vertDistance) <= (radius1 + radius2) * (radius1 + radius2);
}


void Collision::resolveCollision(Entity& first, Entity& second) {
	sf::FloatRect rect1 = first.getSprite().getGlobalBounds();
	sf::FloatRect rect2 = second.getSprite().getGlobalBounds();

	bool dontMove1 = false, dontMove2 = false;

	if (first.getType() == Entity::Type::BigAsteroid || first.getType() == Entity::Type::SmallAsteroid)
		dontMove1 = true;
	if (second.getType() == Entity::Type::BigAsteroid || second.getType() == Entity::Type::SmallAsteroid)
		dontMove2 = true;
	if (first.getType() == Entity::Type::PlayerBullet || first.getType() == Entity::Type::EnemyBullet || 
		second.getType() == Entity::Type::PlayerBullet || second.getType() == Entity::Type::EnemyBullet)
	{
		return;
	}

	float overlapX = std::min(rect1.left + rect1.width, rect2.left + rect2.width) - std::max(rect1.left, rect2.left);
	float overlapY = std::min(rect1.top + rect1.height, rect2.top + rect2.height) - std::max(rect1.top, rect2.top);

	if (overlapX > 0 && overlapY > 0) {
		if (overlapX < overlapY) {
			float sign = (rect1.left + rect1.width / 2 < rect2.left + rect2.width / 2) ? -1.0f : 1.0f;
			float moveX = overlapX * sign / 2.0f;

			if (!dontMove1)
				first.move(sf::Vector2f(moveX, 0));
			if (!dontMove2)
				second.move(sf::Vector2f(-moveX, 0));
		}
		else {
			float sign = (rect1.top + rect1.height / 2 < rect2.top + rect2.height / 2) ? -1.0f : 1.0f;
			float moveY = overlapY * sign / 2.0f;
			if (!dontMove1)
				first.move(sf::Vector2f(0, moveY));
			if (!dontMove2)
				second.move(sf::Vector2f(0, -moveY));
		}
	}
}