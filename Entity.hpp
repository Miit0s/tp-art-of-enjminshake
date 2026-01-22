#pragma once

#include "SFML/Graphics.hpp"
#include "C.hpp"

class Entity
{
public:
	Entity();
	~Entity();

	sf::RectangleShape sprite;

	void update(double deltaTime);
	void drawn(sf::RenderWindow& window);

	// Movements
	float dx{};
	float dy{};

	float friction{ 0.96 };

	// Grid Coordinate
	int cx{};
	int cy{};

	// Ratio inside the grid cell
	float xr{};
	float yr{};

	float speed{ 1 };
	float jumpForce{ 50 };

	//Gravity
	bool applyGravity{ true };
	float gravity{ 0.1 };
	float maxFallSpeed{ 50 };
	bool isOnGround{ false };

	//Collision
	bool canCollide{ true };
	bool isCollidingOnX{ false };
	bool isCollidingOnY{ false };

	bool isDead{ false };

private:
	// Resulting coordinates
	float xx{};
	float yy{};
};