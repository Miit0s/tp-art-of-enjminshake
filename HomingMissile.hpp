#pragma once

#include "Entity.hpp"
#include "Enemy.hpp"

#include <functional>

class HomingMissile :
    public Entity
{
public:
    HomingMissile(sf::Vector2i startPosition, Entity* entityPtr, std::vector<Enemy*>* enemies);
    HomingMissile(sf::Vector2i startPosition, sf::Vector2i endPosition, std::vector<Enemy*>* enemies);
    ~HomingMissile();

    void update(double deltaTime);

    std::function<void()> missileWillBeClear;
private:
    Entity* entityPtr = nullptr;

    sf::Vector2i endPosition;

    std::vector<Enemy*>* enemies;

    float invincibilityDuration{ 0.2f };
    float speed{ 10.0f };
    double timeElasped{ 0 };
    bool isInvincible{ true };

    static sf::Texture& getSharedTexture();

    void destroy();
};

