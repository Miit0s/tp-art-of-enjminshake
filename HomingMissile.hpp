#pragma once

#include "Entity.hpp"
#include "Enemy.hpp"

#include <functional>

#include "CameraShaker.hpp"

class HomingMissile :
    public Entity
{
public:
    HomingMissile(sf::Vector2i startPosition, Entity* entityPtr, std::vector<Enemy*>* enemies);
    HomingMissile(sf::Vector2i startPosition, sf::Vector2i endPosition, std::vector<Enemy*>* enemies);
    ~HomingMissile();

    void update(double deltaTime);
    void drawn(sf::RenderWindow& window);

    std::function<void()> missileWillBeClear;
private:
    Entity* entityPtr = nullptr;
    CameraShaker* cameraShaker;

    sf::Vector2i endPosition;

    std::vector<Enemy*>* enemies;
    std::list<sf::RectangleShape> explosions = {};

    float invincibilityDuration{ 0.2f };
    float speed{ 10.0f };
    double timeElasped{ 0 };
    bool isInvincible{ true };
    bool isExploding{ false };

    static sf::Texture& getSharedTexture();
    sf::Texture	explosionTexture;

    void explosionEffect();
    void destroy();

    TweenMaker* tweenMaker;
};

