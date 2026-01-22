#pragma once

#include "SFML/Graphics.hpp"
#include <functional>

struct Tween
{
	sf::RectangleShape* target;

	sf::Vector2f startSize;
	sf::Vector2f endSize;

	sf::Color startColor;
	sf::Color endColor;

	sf::Vector2f startPosition;
	sf::RectangleShape* movingEndPosition = nullptr;

	double duration;
	double timeElapsed{ 0 };

	bool isFinished;

	std::function<void()> tweenFinishCallback;
};