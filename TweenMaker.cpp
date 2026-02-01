#include "TweenMaker.hpp"

TweenMaker* TweenMaker::instancePtr = nullptr;

//Not thread safe, but not needed for the moment
TweenMaker* TweenMaker::getInstance() {
    if (instancePtr == nullptr) {
    	instancePtr = new TweenMaker();
    }
    return instancePtr;
}

Tween* TweenMaker::startTween(sf::RectangleShape* shape, sf::Vector2f targetSize, sf::Color targetColor, double duration, bool centerAnchor) {
    Tween newTween{};
    newTween.target = shape;
    newTween.duration = duration;

    newTween.startSize = shape->getSize();
    newTween.startColor = shape->getFillColor();

    newTween.endSize = targetSize;
    newTween.endColor = targetColor;

    newTween.startPosition = shape->getPosition();

    newTween.centerAnchor = centerAnchor;

    return &activeTweens.emplace_back(newTween);
}

Tween* TweenMaker::startTween(sf::RectangleShape* shape, sf::RectangleShape* movingEndPosition, double duration)
{
    Tween newTween{};
    newTween.target = shape;
    newTween.duration = duration;

    newTween.startSize = shape->getSize();
    newTween.startColor = shape->getFillColor();

    newTween.endSize = shape->getSize();
    newTween.endColor = shape->getFillColor();

    newTween.startPosition = shape->getPosition();
    newTween.movingEndPosition = movingEndPosition;

    return &activeTweens.emplace_back(newTween);
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

        //Anchor
        if (tween.centerAnchor) tween.target->setOrigin(currentSize.x / 2.0f, currentSize.y / 2.0f);

        //Color
        sf::Uint8 red = tween.startColor.r + (sf::Uint8)((tween.endColor.r - tween.startColor.r) * completionPercentage);
        sf::Uint8 green = tween.startColor.g + (sf::Uint8)((tween.endColor.g - tween.startColor.g) * completionPercentage);
        sf::Uint8 blue = tween.startColor.b + (sf::Uint8)((tween.endColor.b - tween.startColor.b) * completionPercentage);
        sf::Uint8 alpha = tween.startColor.a + (sf::Uint8)((tween.endColor.a - tween.startColor.a) * completionPercentage);

        tween.target->setFillColor(sf::Color{ red, green, blue, alpha });

        //Position
        if (tween.movingEndPosition != nullptr)
        {
            sf::Vector2f currentPosition = tween.startPosition + (tween.movingEndPosition->getPosition() - tween.startPosition) * completionPercentage;
            tween.target->setPosition(currentPosition);
        }
    }

    for (auto tweenPtr = activeTweens.begin(); tweenPtr != activeTweens.end();) {
        if (tweenPtr->isFinished) {

            if (tweenPtr->tweenFinishCallback) tweenPtr->tweenFinishCallback();
            
            tweenPtr = activeTweens.erase(tweenPtr);
        }
        else {
            ++tweenPtr;
        }
    }
}