#pragma once

#include "Entity.hpp"
#include "Enemy.hpp"
#include "HomingMissile.hpp"
#include "TweenMaker.hpp"

class Player :
    public Entity
{
public:
    Player();
    ~Player();

    void update(double deltaTime);
    void drawn(sf::RenderWindow& window);

    void shoot();
    HomingMissile* shootMissile();

    std::vector<sf::Vector2i>* wallsPosition;
    std::vector<Enemy*>* enemies;

private:
    float laserHeight{ 0.5 };
    int maxLaserRange{ 100 };

    std::list<sf::RectangleShape> lasersSprite = {};
    std::list<HomingMissile> homingMissiles = {};

    enum Direction {
        left,
        right
    };
    Direction currentDirection{ right };

    TweenMaker* tweenMaker;
};

