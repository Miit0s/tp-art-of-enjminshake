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

	const float SPEED{ 0.01 };

private:

	// Base coordinates
	// 
	// Grid Coordinate
	int cx{};
	int cy{};
	// Ratio inside the grid cell
	float xr{};
	float yr{};

	// Resulting coordinates
	float xx{};
	float yy{};
};