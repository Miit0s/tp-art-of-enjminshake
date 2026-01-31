#include "Enemy.hpp"

Enemy::Enemy()
{
	sprite.setOrigin(0, 0);
	sprite.setSize({ C::GRID_SIZE, C::GRID_SIZE });
	//sprite.setFillColor(baseColor);

	texture.loadFromFile("res/monster_sprite.PNG");
	sprite.setTexture(&texture);

	tweenMaker = TweenMaker::getInstance();
}

Enemy::~Enemy()
{
}

void Enemy::update(double deltaTime) {
	//Si lag quand beaucoup d'ennemie, regarder ici + opti l'update de entity (car va appliquer graviter et tout alors qu'il est mort)
	if (isDead) return;

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

void Enemy::hit() {
	if (isDead) return;

	life -= 1;

	if (life <= 0) {
		isDead = true;
		makeDeathEffect();
	}
	else {
		makeHitEffect();
	}
}

void Enemy::makeHitEffect() {
	Tween* tweenCreated = tweenMaker->startTween(&sprite, sprite.getSize(), sf::Color{255, 0, 0, 255}, 0.1, false);

	tweenCreated->tweenFinishCallback = [this]() {
		this->tweenMaker->startTween(&sprite, sprite.getSize(), baseColor, 0.1, false);
	};
}

void Enemy::makeDeathEffect() {
	friction = 0.99;

	dy -= upForceWhenDead;
	dx += (currentDirection == left) ? horizontalForceWhenDead : -horizontalForceWhenDead;

	Tween* tweenCreated = tweenMaker->startTween(&sprite, sprite.getSize(), sf::Color{ 255, 255, 255, 100 }, 0.1, false);
}