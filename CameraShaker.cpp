#include "CameraShaker.hpp"

CameraShaker* CameraShaker::instancePtr = nullptr;

CameraShaker* CameraShaker::getInstance()
{
    if (instancePtr == nullptr) {
        instancePtr = new CameraShaker();
    }
    return instancePtr;
}

void CameraShaker::processCameraShake(sf::View* view)
{
	view->setCenter(view->getSize().x / 2, view->getSize().y / 2);

	if (currentShakeTime > 0) {

		float dampFactor = currentShakeTime / shakeDuration;

		float offsetX = randomFloat(-1.0f, 1.0f) * shakeMagnitude * dampFactor;
		float offsetY = randomFloat(-1.0f, 1.0f) * shakeMagnitude * dampFactor;

		view->move(offsetX, offsetY);
	}
}

void CameraShaker::shakeCamera()
{
	currentShakeTime = shakeDuration;
}

float CameraShaker::randomFloat(float min, float max)
{
	return min + (float)(rand() / (float)(RAND_MAX / (max - min)));
}

void CameraShaker::update(double dt)
{
	if (currentShakeTime > 0) {
		currentShakeTime -= dt;

		if (currentShakeTime < 0) currentShakeTime = 0;
	}
}