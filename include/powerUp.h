#ifndef POWER_UP
#define POWER_UP
#include"entity.h"
#include"world.h"

class PowerUP : public Entity
{
private:
public:
	PowerUP(World& world);

	virtual void update(float deltaTime);
	virtual void onCollide(Entity& other) override;
	//virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};
#endif
