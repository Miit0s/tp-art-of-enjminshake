#include "HomingMissile.hpp"

HomingMissile::HomingMissile(sf::Vector2i startPosition, Entity* entityPtr, std::vector<Enemy*>* enemies)
{
	cx = startPosition.x;
	cy = startPosition.y;

	this->entityPtr = entityPtr;
	this->enemies = enemies;

	applyGravity = false;
}

HomingMissile::HomingMissile(sf::Vector2i startPosition, sf::Vector2i endPosition, std::vector<Enemy*>* enemies)
{
	cx = startPosition.x;
	cy = startPosition.y;

	this->endPosition = endPosition;
	this->enemies = enemies;

	applyGravity = false;
}

HomingMissile::~HomingMissile() = default;

void HomingMissile::update(double deltaTime)
{
	if (isDead) return;

	if (isInvincible)
	{
		timeElasped += deltaTime;
		if (timeElasped >= invincibilityDuration) isInvincible = false;
	}

	if ((isCollidingOnX || isCollidingOnY) && !isInvincible) destroy();
	
	if (endPosition.x == cx + 1 || endPosition.x == cx - 1 || endPosition.x == cx)
	{
		if (endPosition.y == cy + 1 || endPosition.y == cy - 1 || endPosition.y == cy) {
			destroy();
		}
	}

	for (Enemy* enemy : *enemies) {
		if (enemy->cx == cx + 1 || enemy->cx == cx - 1 || enemy->cx == cx)
		{
			if (enemy->cy == cy + 1 || enemy->cy == cy - 1 || enemy->cy == cy) {

				if (!enemy->isDead) enemy->hit();
				destroy();
			}
		}
	}

	//Homing Logic
	sf::Vector2f targetPos = (entityPtr != nullptr) ? entityPtr->sprite.getPosition() : sf::Vector2f{(float)endPosition.x * C::GRID_SIZE, (float)endPosition.y * C::GRID_SIZE};
	sf::Vector2f currentPos = sprite.getPosition();

	sf::Vector2f direction = targetPos - currentPos;

	float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
	if (length != 0) {
		direction /= length;
	}

	dx = direction.x * speed;
	dy = direction.y * speed;

	//Rotation logic
	float radians = std::atan2(dy, dx);
	float degrees = radians * (180.0f / 3.14f);

	sprite.setRotation(degrees);
}

void HomingMissile::destroy()
{
	isDead = true;
	missileWillBeClear();
}
