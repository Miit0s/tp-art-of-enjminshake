#include "Entity.hpp"

Entity::Entity()
{
	sprite.setOrigin(0, 0);
	sprite.setSize({ C::GRID_SIZE, C::GRID_SIZE });
	sprite.setFillColor(sf::Color{ 255, 255, 255, 255 });
}

Entity::~Entity()
{
}

void Entity::update(double deltaTime) {
	if (!isOnGround && applyGravity) { dy += gravity; }

	if (dy > maxFallSpeed) { dy = maxFallSpeed; }

	dx *= 0.96;

	if (isCollidingOnX) {
		dx = 0;
	}

	if (isCollidingOnY) {
		//Si le joueur est en train de tomber
		if (dy > 0) { isOnGround = true; }

		dy = 0;
	}
	else {
		isOnGround = false;
	}

	xr += dx * deltaTime;
	yr += dy * deltaTime;

	while (xr > 1) { xr--;	cx++; }
	while (xr < 0) { xr++;	cx--; }

	while (yr > 1) { yr--;	cy++; }
	while (yr < 0) { yr++;	cy--; }

	xx = (cx + xr) * C::GRID_SIZE;
	yy = (cy + yr) * C::GRID_SIZE;

	sprite.setPosition(xx, yy);
}

void Entity::drawn(sf::RenderWindow& window) {
	window.draw(sprite);
}