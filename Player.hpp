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

    void moveLeft(float force = 0);
    void moveRight(float force = 0);

    void shoot();
    void setPosition(sf::Vector2i newPosition);

    std::vector<sf::Vector2i>* wallsPositionPtr;
    std::vector<Enemy*>* enemiesVectorPtr;
    std::list<Entity*>* entitiesListPtr;

    PetDrone petDrone;

private:
    float laserHeight{ 0.5 };
    int maxLaserRange{ 100 };
    float recoilForce{ 50 };

    sf::Vector2f muzzleOffset{ -0.5f, 0.5f };

    std::list<sf::RectangleShape> lasersSprite = {};
    std::list<sf::RectangleShape> muzzlesSprite = {};

    enum Direction {
        left,
        right
    };
    Direction currentDirection{ right };
    float movementToAdd{ 0 };

    void setCurrentDirection(Direction newDirection)
    {
        if (currentDirection != newDirection) petDrone.switchSide();

	    currentDirection = newDirection;
    }

    TweenMaker* tweenMaker;
};

