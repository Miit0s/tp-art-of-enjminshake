#pragma once

#include "Entity.hpp"
#include "HomingMissile.hpp"

class PetDrone :
    public Entity
{
private:
    Entity* entityToFollow = nullptr;

    sf::Vector2f offset{-2, -2};
    float speed{ 10.0f };

    float shootDelay{ 5.0f };
    double timeElapsed{ 0.0f };

    std::vector<Enemy*>* enemiesVectorPtr;
    std::list<Entity*>* entitiesListPtr;
    std::list<HomingMissile> homingMissiles = {};

    void shootMissile();
    void setupNewMissile(HomingMissile* newMissile);

public:
    PetDrone(Entity* entityToFollow, std::vector<Enemy*>* enemiesVectorPtr, std::list<Entity*>* entitiesListPtr);
    void tpToEntity();
    void switchSide();

    void update(double deltaTime);
};

