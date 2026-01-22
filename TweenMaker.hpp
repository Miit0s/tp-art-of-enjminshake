#pragma once

#include "Tween.hpp"
#include <list>

class TweenMaker
{
private:
	static TweenMaker* instancePtr;

	TweenMaker() {}

	std::list<Tween> activeTweens;
	
public:
	TweenMaker(const TweenMaker& obj) = delete;

	static TweenMaker* getInstance();

	void update(double deltaTime);

	Tween* startTween(sf::RectangleShape* shape, sf::Vector2f targetSize, sf::Color targetColor, double duration);
	Tween* startTween(sf::RectangleShape* shape, sf::RectangleShape* movingEndPosition, double duration);
};

