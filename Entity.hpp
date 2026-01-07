#pragma once

#include "SFML/Graphics.hpp"
#include "C.hpp"

class Entity
{
public:
	Entity();
	~Entity();

	sf::RectangleShape sprite;

	void update();
	void drawn(sf::RenderWindow& window);

	// Movements
	float dx{};
	float dy{};

	// Grid Coordinate
	int cx{};
	int cy{};

	// Ratio inside the grid cell
	float xr{};
	float yr{};

	float speed{ 0.01 };
	float jump_force{ 0.7 };

	//Gravity
	bool apply_gravity{ true };
	float gravity{ 0.01f };
	float max_fall_speed{ 0.5f };
	bool is_on_ground{ false };

	//Collision
	bool can_collide{ true };
	bool is_colliding_on_x{ false };
	bool is_colliding_on_y{ false };

private:
	// Resulting coordinates
	float xx{};
	float yy{};
};