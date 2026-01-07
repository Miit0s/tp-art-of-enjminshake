#include "Entity.hpp"

Entity::Entity()
{
	sprite.setOrigin(0, 0);
	sprite.setSize({ C::GRID_SIZE * 2, C::GRID_SIZE * 2 });
	sprite.setFillColor(sf::Color{ 255, 255, 255, 255 });
}

Entity::~Entity()
{
}

void Entity::update() {
	xx = (int)(cx + xr) * 16;
	yy = (int)(cy + yr) * 16;

	dx *= 0.96;
	dy *= 0.96;

	xr += dx;
	yr += dy;

	while (xr > 1) { xr--;	cx++; }
	while (xr < 0) { xr++;	cx--; }

	while (yr > 1) { yr--;	cy++; }
	while (yr < 0) { yr++;	cy--; }

	sprite.setPosition(xx, yy);
}

void Entity::drawn(sf::RenderWindow& window) {
	window.draw(sprite);
}