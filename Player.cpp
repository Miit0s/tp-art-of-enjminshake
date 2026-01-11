#include "Player.hpp"

void Player::shoot() {
	if (haveToShoot) { return; }

	haveToShoot = true;
}

void Player::update(double deltaTime) {
	if (dx > 0) currentDirection = right;
	else currentDirection = left;

	//Really dum research, have to be optimize if needed
	if (haveToShoot) {
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
				if (enemy->cx == checkX && enemy->cy == startPosition.y) {
					hitPosition.x = checkX + enemy->xr;
					hitFound = true;
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

		tweenMaker = TweenMaker::getInstance();

		Tween* tweenCreated = tweenMaker->startTween(&createdLaser, sf::Vector2{ distance * C::GRID_SIZE, 0.0f }, sf::Color{ 255, 255, 255, 0 }, 1);
		auto laserIterator = std::prev(lasersSprite.end());

		tweenCreated->tweenFinishCallback = [this, laserIterator]() {
			this->lasersSprite.erase(laserIterator);
		};

		haveToShoot = false;
	}
}

void Player::drawn(sf::RenderWindow& window) {
	for (sf::RectangleShape& laser : lasersSprite)
		window.draw(laser);
}