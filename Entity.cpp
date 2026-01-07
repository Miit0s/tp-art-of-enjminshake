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

void Entity::update() {
	if (!is_on_ground && apply_gravity) { dy += gravity; }

	if (dy > max_fall_speed) { dy = max_fall_speed; }

	dx *= 0.96;

	if (is_colliding_on_x) {
		dx = 0;
	}

	if (is_colliding_on_y) {
		//Si le joueur est en train de tomber
		if (dy > 0) { is_on_ground = true; }

		dy = 0;
	}
	else {
		is_on_ground = false;
	}

	xr += dx;
	yr += dy;

	while (xr > 1) { xr--;	cx++; }
	while (xr < 0) { xr++;	cx--; }

	while (yr > 1) { yr--;	cy++; }
	while (yr < 0) { yr++;	cy--; }

	xx = (int)(cx + xr) * 16;
	yy = (int)(cy + yr) * 16;

	sprite.setPosition(xx, yy);
}

void Entity::drawn(sf::RenderWindow& window) {
	window.draw(sprite);
}