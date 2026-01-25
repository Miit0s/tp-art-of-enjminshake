#pragma once

#include <vector>

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"

#include "sys.hpp"

#include "Particle.hpp"
#include "ParticleMan.hpp"
#include "TweenMaker.hpp"

#include "Player.hpp"
#include "Enemy.hpp"
#include "LevelEditor.hpp"
#include "JsonSerializationTool.hpp"

using namespace sf;

class HotReloadShader;
class Game {
public:
	sf::RenderWindow*				win = nullptr;

	sf::RectangleShape				bg;
	HotReloadShader *				bgShader = nullptr;

	sf::Texture						tex;

	bool							closing = false;
	
	std::vector<sf::Vector2i>		walls;
	std::vector<sf::RectangleShape> wallSprites;

	std::list<Entity*> entities;

	std::vector<sf::Vector2i> enemiesSpawnPoint;
	std::vector<Enemy*> enemies;

	ParticleMan beforeParts;
	ParticleMan afterParts;

	Game(sf::RenderWindow * win);

	void cacheWalls();

	void processInput(sf::Event ev);
	void processCollision();
	void processEntityUpdate(double deltaTime);

	void updateLevel();

	bool wasPressed = false;
	void pollInput(double dt);
	void onSpacePressed();

	void update(double dt);

	void draw(sf::RenderWindow& win);

	bool isWall(int cx, int cy);
	void im();

private:
	sf::Vector2i playerSpawnPoint;
	Player player;

	//Level Editor
	LevelEditor levelEditor;
	bool showLevelEditorWindows{ false };

	JsonSerializationTool jsonTool{};
	std::string levelPath = "LevelSave/mainLevel.json";

	void trackPlayerStats();
	void manageLevelEditor();
	void spawnEnemies();

	TweenMaker* tweenMaker;
};