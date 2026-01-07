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

	float speed{ 0.001 };
	float jumpForce{ 0.12 };

	//Gravity
	bool applyGravity{ true };
	float gravity{ 0.001f };
	float maxFallSpeed{ 0.05f };
	bool isOnGround{ false };

	//Collision
	bool canCollide{ true };
	bool isCollidingOnX{ false };
	bool isCollidingOnY{ false };

private:
	// Resulting coordinates
	float xx{};
	float yy{};
};