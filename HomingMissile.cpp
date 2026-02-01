#include "HomingMissile.hpp"

HomingMissile::HomingMissile(sf::Vector2i startPosition, Entity* entityPtr, std::vector<Enemy*>* enemies)
{
	cx = startPosition.x;
	cy = startPosition.y;

	this->entityPtr = entityPtr;
	this->enemies = enemies;
	this->cameraShaker = CameraShaker::getInstance();
	this->tweenMaker = TweenMaker::getInstance();

	applyGravity = false;

	explosionTexture.loadFromFile("res/circle_explosion.PNG");
	sprite.setTexture(&getSharedTexture());
	sprite.setSize({ C::GRID_SIZE / 1.3, C::GRID_SIZE / 1.3 });
}

HomingMissile::HomingMissile(sf::Vector2i startPosition, sf::Vector2i endPosition, std::vector<Enemy*>* enemies)
{
	cx = startPosition.x;
	cy = startPosition.y;

	this->endPosition = endPosition;
	this->enemies = enemies;
	this->cameraShaker = CameraShaker::getInstance();
	this->tweenMaker = TweenMaker::getInstance();

	applyGravity = false;

	explosionTexture.loadFromFile("res/circle_explosion.PNG");
	sprite.setTexture(&getSharedTexture());
	sprite.setSize({ C::GRID_SIZE / 1.3, C::GRID_SIZE / 1.3 });
}

HomingMissile::~HomingMissile() = default;

sf::Texture& HomingMissile::getSharedTexture()
{
	static sf::Texture texture;

	if (texture.getSize().x == 0) {
		texture.loadFromFile("res/missile_sprite.PNG");
	}

	return texture;
}

void HomingMissile::update(double deltaTime)
{
	if (isDead or isExploding) return;

	if (isInvincible)
	{
		timeElasped += deltaTime;
		if (timeElasped >= invincibilityDuration) isInvincible = false;
	}

	if ((isCollidingOnX || isCollidingOnY) && !isInvincible) explosionEffect();
	
	if (endPosition.x == cx + 1 || endPosition.x == cx - 1 || endPosition.x == cx)
	{
		if (endPosition.y == cy + 1 || endPosition.y == cy - 1 || endPosition.y == cy) {
			explosionEffect();
		}
	}

	for (Enemy* enemy : *enemies) {
		if (enemy->cx == cx + 1 || enemy->cx == cx - 1 || enemy->cx == cx)
		{
			if (enemy->cy == cy + 1 || enemy->cy == cy - 1 || enemy->cy == cy) {

				if (!enemy->isDead) enemy->hit();
				explosionEffect();
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

void HomingMissile::drawn(sf::RenderWindow& window)
{
	for (sf::RectangleShape& explosion : explosions)
		window.draw(explosion);
}

void HomingMissile::explosionEffect()
{
	isExploding = true;

	explosions.emplace_back(sf::RectangleShape{ sf::Vector2f{C::GRID_SIZE * 3, C::GRID_SIZE * 3} });
	sf::RectangleShape& explosion = explosions.back();

	explosion.setPosition(sf::Vector2f{ (cx + xr) * C::GRID_SIZE, (cy + yr) * C::GRID_SIZE });
	explosion.setFillColor(sf::Color{ 255,0,0,255 });
	explosion.setTexture(&explosionTexture);

	Tween* tweenForExplosion = tweenMaker->startTween(&explosion, sf::Vector2f{ 0.0f, 0.0f }, sf::Color{ 20,20,20,255 }, 0.5, true);

	tweenForExplosion->tweenFinishCallback = [this]() {
		this->destroy();
	};

	cameraShaker->shakeCamera();

	sprite.setFillColor(sf::Color{ 255, 255, 255, 0 });
}

void HomingMissile::destroy()
{
	isDead = true;
	missileWillBeClear();
}
