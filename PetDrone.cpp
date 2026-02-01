#include "PetDrone.hpp"

#include <random>


PetDrone::PetDrone(Entity* entityToFollow, std::vector<Enemy*>* enemiesVectorPtr, std::list<Entity*>* entitiesListPtr)
{
	this->entityToFollow = entityToFollow;
	this->applyGravity = false;
	this->enemiesVectorPtr = enemiesVectorPtr;
	this->entitiesListPtr = entitiesListPtr;

	texture.loadFromFile("res/pet_drone_sprite.PNG");
	sprite.setTexture(&texture);

	tpToEntity();
}

void PetDrone::tpToEntity()
{
	cx = (int)entityToFollow->cx + offset.x;
	cy = (int)entityToFollow->cy + offset.y;
}

void PetDrone::switchSide()
{
	offset = (offset == sf::Vector2f{ -2, -2 }) ? sf::Vector2f{ 2, -2 } : sf::Vector2f{ -2, -2 };
}

void PetDrone::shootMissile()
{
	std::vector<Enemy*> aliveEnemies;
	std::copy_if(enemiesVectorPtr->begin(), enemiesVectorPtr->end(), std::back_inserter(aliveEnemies), [](Enemy* enemy) {
		return enemy != nullptr && !enemy->isDead;
	});

	static std::mt19937 gen(std::random_device{}());

	if (aliveEnemies.empty()) {
		std::uniform_int_distribution<int> dist(0, 5);

		HomingMissile* newMissile = &homingMissiles.emplace_back(HomingMissile{ sf::Vector2i{cx, cy}, sf::Vector2i{20 + dist(gen), 5 + dist(gen)}, enemiesVectorPtr });
		setupNewMissile(newMissile);

		return;
	}

	std::uniform_int_distribution<size_t> dist(0, aliveEnemies.size() - 1);

	Enemy* target = aliveEnemies[dist(gen)];

	HomingMissile* newMissile = &homingMissiles.emplace_back(HomingMissile{ sf::Vector2i{cx, cy}, target, enemiesVectorPtr });
	setupNewMissile(newMissile);
}

void PetDrone::setupNewMissile(HomingMissile* newMissile)
{
	entitiesListPtr->push_back(newMissile);

	auto missileIterator = std::prev(entitiesListPtr->end());
	newMissile->missileWillBeClear = [this, missileIterator]() {
		this->entitiesListPtr->erase(missileIterator);
	};
}

void PetDrone::update(double deltaTime)
{
	sf::Vector2f selfPos = { (float)cx + xr, (float)cy + yr };
	sf::Vector2f targetPos = {
		(float)entityToFollow->cx + entityToFollow->xr,
		(float)entityToFollow->cy + entityToFollow->yr
	};
	targetPos += offset;

	float factor = 1.0f - (float)std::exp(-speed * deltaTime);
	sf::Vector2f nextPos = selfPos + (targetPos - selfPos) * factor;

	dx = (nextPos.x - selfPos.x) / (float)deltaTime;
	dy = (nextPos.y - selfPos.y) / (float)deltaTime;

	for (auto iterator = homingMissiles.begin(); iterator != homingMissiles.end();)
	{
		iterator->update(deltaTime);

		if (iterator->isDead) iterator = homingMissiles.erase(iterator);
		else ++iterator;
	}

	timeElapsed += deltaTime;
	if (timeElapsed >= shootDelay)
	{
		shootMissile();
		timeElapsed = 0;
	}
}

void PetDrone::drawn(sf::RenderWindow& window)
{
	for (auto& missile : homingMissiles)
	{
		missile.drawn(window);
	}
}
