#include "TweenMaker.hpp"

TweenMaker* TweenMaker::instancePtr = nullptr;

//Not thread safe, but not needed for the moment
TweenMaker* TweenMaker::getInstance() {
    if (instancePtr == nullptr) {
        if (instancePtr == nullptr) {
            instancePtr = new TweenMaker();
        }
    }
    return instancePtr;
}

Tween* TweenMaker::startTween(sf::RectangleShape* shape, sf::Vector2f targetSize, sf::Color targetColor, double duration) {
    Tween newTween{};
    newTween.target = shape;
    newTween.duration = duration;

    newTween.startSize = shape->getSize();
    newTween.startColor = shape->getFillColor();

    newTween.endSize = targetSize;
    newTween.endColor = targetColor;

    activeTweens.push_back(newTween);

    return &activeTweens.back();
}

void TweenMaker::update(double deltaTime) {
    for (Tween& tween : activeTweens) {
        tween.timeElapsed += deltaTime;

        //0 to 1
        float completionPercentage = (float)(tween.timeElapsed / tween.duration);

        if (completionPercentage >= 1) {
            completionPercentage = 1;
            tween.isFinished = true;
        }

        //Size
        sf::Vector2f currentSize = tween.startSize + (tween.endSize - tween.startSize) * completionPercentage;
        tween.target->setSize(currentSize);

        //Color
        sf::Uint8 red = tween.startColor.r + (sf::Uint8)((tween.endColor.r - tween.startColor.r) * completionPercentage);
        sf::Uint8 green = tween.startColor.g + (sf::Uint8)((tween.endColor.g - tween.startColor.g) * completionPercentage);
        sf::Uint8 blue = tween.startColor.b + (sf::Uint8)((tween.endColor.b - tween.startColor.b) * completionPercentage);
        sf::Uint8 alpha = tween.startColor.a + (sf::Uint8)((tween.endColor.a - tween.startColor.a) * completionPercentage);

        tween.target->setFillColor(sf::Color{ red, green, blue, alpha });
    }

    for (auto tweenPtr = activeTweens.begin(); tweenPtr != activeTweens.end();) {
        if (tweenPtr->isFinished) {
            tweenPtr->tweenFinishCallback();
            tweenPtr = activeTweens.erase(tweenPtr);
        }
        else {
            ++tweenPtr;
        }
    }
}