#pragma once

#include "Entity.hpp"
#include "TweenMaker.hpp"

class Enemy : public Entity
{
public:
	Enemy();
	~Enemy();
	
	void update(double deltaTime);

	void hit();

private:
	int life{ 5 };

	float upForceWhenDead{ 20 };
	float horizontalForceWhenDead{ 60 };

	sf::Color baseColor{ 255, 0, 0, 255 };

	TweenMaker* tweenMaker;

	enum Direction {
		left,
		right
	};
	Direction currentDirection{ left };

	void makeHitEffect();
	void makeDeathEffect();
};

