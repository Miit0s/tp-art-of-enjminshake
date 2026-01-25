#pragma once

#include "Entity.hpp"
#include "Enemy.hpp"
#include "HomingMissile.hpp"
#include "TweenMaker.hpp"
#include "PetDrone.hpp"

class Player :
    public Entity
{
public:
    Player(std::vector<sf::Vector2i>* wallsPositionPtr, std::list<Entity*>* entitiesListPtr, std::vector<Enemy*>* enemiesVectorPtr);
    ~Player();

    void update(double deltaTime);
    void drawn(sf::RenderWindow& window);

    void shoot();
    void setPosition(sf::Vector2i newPosition);

    std::vector<sf::Vector2i>* wallsPositionPtr;
    std::vector<Enemy*>* enemiesVectorPtr;
    std::list<Entity*>* entitiesListPtr;

    PetDrone petDrone;

private:
    float laserHeight{ 0.5 };
    int maxLaserRange{ 100 };

    std::list<sf::RectangleShape> lasersSprite = {};

    enum Direction {
        left,
        right
    };
    Direction currentDirection{ right };

    void setCurrentDirection(Direction newDirection)
    {
        if (currentDirection != newDirection) petDrone.switchSide();

	    currentDirection = newDirection;
    }

    TweenMaker* tweenMaker;
};

