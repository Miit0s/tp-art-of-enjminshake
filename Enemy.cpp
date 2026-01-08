#include "Enemy.hpp"

Enemy::Enemy()
{
	sprite.setOrigin(0, 0);
	sprite.setSize({ C::GRID_SIZE, C::GRID_SIZE });
	sprite.setFillColor(sf::Color{ 255, 0, 0, 255 });
}

Enemy::~Enemy()
{
}

void Enemy::update(double deltaTime) {
	if (isCollidingOnX) {
		currentDirection = currentDirection == left ? right : left;
	}

	switch (currentDirection)
	{
	case Enemy::left:
		dx -= speed; 
		break;
	case Enemy::right:
		dx += speed;
		break;
	default:
		break;
	}
}
