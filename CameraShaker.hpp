#pragma once

#include "SFML/Graphics.hpp"

class CameraShaker
{
private:
	static CameraShaker* instancePtr;

	CameraShaker() {}

	float shakeDuration{ 0.2f };
	float shakeMagnitude{ 2.5f };

	double currentShakeTime{ 0.0f };

	float randomFloat(float min, float max);
public:
	static CameraShaker* getInstance();

	void shakeCamera();

	void processCameraShake(sf::View*);
	void update(double dt);
};

