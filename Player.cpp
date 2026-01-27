#include "Player.hpp"

Player::Player(std::vector<sf::Vector2i>* wallsPositionPtr, std::list<Entity*>* entitiesListPtr, std::vector<Enemy*>* enemiesVectorPtr) : petDrone(this, enemiesVectorPtr, entitiesListPtr) {
	tweenMaker = TweenMaker::getInstance();

	this->entitiesListPtr = entitiesListPtr;
	this->wallsPositionPtr = wallsPositionPtr;
	this->enemiesVectorPtr = enemiesVectorPtr;

	entitiesListPtr->push_back(&petDrone);
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

		for (sf::Vector2i wallPosition : *wallsPositionPtr) {
			if (wallPosition.x == checkX && wallPosition.y == startPosition.y) {
				hitPosition.x = checkX;
				hitFound = true;
				break;
			}
		}

		if (hitFound) break;

		for (Enemy* enemy : *enemiesVectorPtr) {
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

	//Laser creation
	float distance = (currentDirection == right) ? abs(hitPosition.x - startPosition.x - 1) : abs(hitPosition.x - startPosition.x);

	lasersSprite.emplace_back(sf::Vector2f(distance * C::GRID_SIZE, laserHeight * C::GRID_SIZE));
	sf::RectangleShape& createdLaser = lasersSprite.back();

	float yOffset = (C::GRID_SIZE - (C::GRID_SIZE * laserHeight)) / 2.0f;

	if (currentDirection == right)
		createdLaser.setPosition((startPosition.x + 1) * C::GRID_SIZE, startPosition.y * C::GRID_SIZE + yOffset);
	else
		createdLaser.setPosition((hitPosition.x + 1) * C::GRID_SIZE, startPosition.y * C::GRID_SIZE + yOffset);

	createdLaser.setFillColor(sf::Color{ 255, 20, 20, 255 });

	Tween* tweenCreated = tweenMaker->startTween(&createdLaser, sf::Vector2{ distance * C::GRID_SIZE, 0.0f }, sf::Color{ 255, 255, 255, 0 }, 1, false);
	auto laserIterator = std::prev(lasersSprite.end());

	tweenCreated->tweenFinishCallback = [this, laserIterator]() {
		this->lasersSprite.erase(laserIterator);
	};

	//Muzzle
	muzzlesSprite.emplace_back(sf::RectangleShape{sf::Vector2f{C::GRID_SIZE * 2, C::GRID_SIZE * 2}});
	sf::RectangleShape& createdMuzzle = muzzlesSprite.back();

	if (currentDirection == left)
		createdMuzzle.setPosition(sf::Vector2f{ (cx + xr + muzzleOffset.x) * C::GRID_SIZE, (cy + yr + muzzleOffset.y) * C::GRID_SIZE });
	else
		createdMuzzle.setPosition(sf::Vector2f{ (cx + xr + (muzzleOffset.x) * -2.5f) * C::GRID_SIZE, (cy + yr + muzzleOffset.y) * C::GRID_SIZE });

	createdMuzzle.setFillColor(sf::Color{ 255,0,0,255 });

	Tween* tweenForMuzzle = tweenMaker->startTween(&createdMuzzle, sf::Vector2f{ 0.0f, 0.0f }, sf::Color{ 255,255,255,255 }, 0.2, true);
	auto muzzleIterator = std::prev(muzzlesSprite.end());

	tweenCreated->tweenFinishCallback = [this, muzzleIterator]() {
		this->muzzlesSprite.erase(muzzleIterator);
	};
}

void Player::setPosition(sf::Vector2i newPosition)
{
	cx = newPosition.x;
	cy = newPosition.y;
	petDrone.tpToEntity();
}

void Player::update(double deltaTime) {
	if (dx > 0) setCurrentDirection(right);
	else setCurrentDirection(left);

	petDrone.update(deltaTime);
}

void Player::drawn(sf::RenderWindow& window) {
	for (sf::RectangleShape& laser : lasersSprite)
		window.draw(laser);

	for (sf::RectangleShape& muzzle : muzzlesSprite)
		window.draw(muzzle);

	petDrone.drawn(window);
}