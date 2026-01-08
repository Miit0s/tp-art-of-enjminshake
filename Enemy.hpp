#pragma once

#include "Entity.hpp"

class Enemy : public Entity
{
public:
	Enemy();
	~Enemy();
	
	void update(double deltaTime);

private:
	enum Direction {
		left,
		right
	};
	Direction currentDirection{ left };
};

