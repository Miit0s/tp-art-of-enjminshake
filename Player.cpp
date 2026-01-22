#include "Player.hpp"

#include <random>

Player::Player() {
	tweenMaker = TweenMaker::getInstance();
}

Player::~Player()
{
}

void Player::shoot() {
	//Really dum research, have to be optimize if needed

	sf::Vector2i startPosition = sf::Vector2i{ cx, cy };
	sf::Vector2i hitPosition = sf::Vector2i{ cx, cy };

	int step = (currentDirection == right) ? 1 : -1;
	bool hitFound{ false };

	for (int i = 1; i <= maxLaserRange; i++) {
		int checkX{ cx + (i * step) };

		for (sf::Vector2i wallPosition : *wallsPosition) {
			if (wallPosition.x == checkX && wallPosition.y == startPosition.y) {
				hitPosition.x = checkX;
				hitFound = true;
				break;
			}
		}

		if (hitFound) break;

		for (Enemy* enemy : *enemies) {
			if (enemy->cx == checkX && enemy->cy == startPosition.y && !enemy->isDead) {
				hitPosition.x = checkX;
				hitPosition.x += (currentDirection == left) ? -1 : 2;
				hitFound = true;
				enemy->hit();
				break;
			}
		}

		if (hitFound) break;

		hitPosition.x = checkX;
	}

	float distance = (currentDirection == right) ? abs(hitPosition.x - startPosition.x - 1) : abs(hitPosition.x - startPosition.x);

	lasersSprite.emplace_back(sf::Vector2f(distance * C::GRID_SIZE, laserHeight * C::GRID_SIZE));
	sf::RectangleShape& createdLaser = lasersSprite.back();

	float yOffset = (C::GRID_SIZE - (C::GRID_SIZE * laserHeight)) / 2.0f;

	if (currentDirection == right)
		createdLaser.setPosition((startPosition.x + 1) * C::GRID_SIZE, startPosition.y * C::GRID_SIZE + yOffset);
	else
		createdLaser.setPosition((hitPosition.x + 1) * C::GRID_SIZE, startPosition.y * C::GRID_SIZE + yOffset);

	createdLaser.setFillColor(sf::Color{ 255, 20, 20, 255 });

	Tween* tweenCreated = tweenMaker->startTween(&createdLaser, sf::Vector2{ distance * C::GRID_SIZE, 0.0f }, sf::Color{ 255, 255, 255, 0 }, 1);
	auto laserIterator = std::prev(lasersSprite.end());

	tweenCreated->tweenFinishCallback = [this, laserIterator]() {
		this->lasersSprite.erase(laserIterator);
	};
}

HomingMissile* Player::shootMissile()
{
	std::vector<Enemy*> aliveEnemies;
	std::copy_if(enemies->begin(), enemies->end(), std::back_inserter(aliveEnemies), [](Enemy* enemy) {
		return enemy != nullptr && !enemy->isDead;
	});

	static std::mt19937 gen(std::random_device{}());

	if (aliveEnemies.empty()) {
		std::uniform_int_distribution<int> dist(0, 10);

		return &homingMissiles.emplace_back(HomingMissile{ sf::Vector2i{cx, cy}, sf::Vector2i{40 + dist(gen), 20 + dist(gen)}, enemies });
	}

	std::uniform_int_distribution<size_t> dist(0, aliveEnemies.size() - 1);

	Enemy* target = aliveEnemies[dist(gen)];

	return &homingMissiles.emplace_back(HomingMissile{ sf::Vector2i{cx, cy}, target, enemies });
	//Tween* tweenCreated = tweenMaker->startTween(&newMissile.sprite, &enemies->front()->sprite, 5);
}

void Player::update(double deltaTime) {
	if (dx > 0) currentDirection = right;
	else currentDirection = left;

	for (auto iterator = homingMissiles.begin(); iterator != homingMissiles.end();)
	{
		iterator->update(deltaTime);

		if (iterator->isDead) iterator = homingMissiles.erase(iterator);
		else ++iterator;
	}
}

void Player::drawn(sf::RenderWindow& window) {
	for (sf::RectangleShape& laser : lasersSprite)
		window.draw(laser);

}