#pragma once

#include "Tween.hpp"
#include <vector>

class TweenMaker
{
private:
	static TweenMaker* instancePtr;

	TweenMaker() {}

	std::vector<Tween> activeTweens;
	
public:
	TweenMaker(const TweenMaker& obj) = delete;

	static TweenMaker* getInstance();

	void update(double deltaTime);

	Tween* startTween(sf::RectangleShape* shape, sf::Vector2f targetSize, sf::Color targetColor, double duration);
};

