
#include <imgui.h>
#include <array>
#include <vector>

#include "C.hpp"
#include "Game.hpp"

#include "HotReloadShader.hpp"

static int cols = C::RES_X / C::GRID_SIZE;
static int lastLine = C::RES_Y / C::GRID_SIZE - 1;

Game::Game(sf::RenderWindow* win) : levelEditor{ &walls, &enemiesSpawnPoint, &playerSpawnPoint } {
	this->win = win;
	bg = sf::RectangleShape(Vector2f((float)win->getSize().x, (float)win->getSize().y));

	bool isOk = tex.loadFromFile("res/bg_stars.png");
	if (!isOk) {
		printf("ERR : LOAD FAILED\n");
	}
	bg.setTexture(&tex);
	bg.setSize(sf::Vector2f(C::RES_X, C::RES_Y));

	bgShader = new HotReloadShader("res/bg.vert", "res/bg.frag");
	
	for (int i = 0; i < C::RES_X / C::GRID_SIZE; ++i) 
		walls.push_back( Vector2i(i, lastLine) );

	//Walls Setup
	walls.push_back(Vector2i(0, lastLine-1));
	walls.push_back(Vector2i(0, lastLine-2));
	walls.push_back(Vector2i(0, lastLine-3));

	walls.push_back(Vector2i(cols-1, lastLine - 1));
	walls.push_back(Vector2i(cols-1, lastLine - 2));
	walls.push_back(Vector2i(cols-1, lastLine - 3));

	walls.push_back(Vector2i(cols >>2, lastLine - 2));
	walls.push_back(Vector2i(cols >>2, lastLine - 3));
	walls.push_back(Vector2i(cols >>2, lastLine - 4));
	walls.push_back(Vector2i((cols >> 2) + 1, lastLine - 4));
	cacheWalls();

	//Player Setup
	playerSpawnPoint = sf::Vector2i{ 10, lastLine - 1 };
	playerResetPosition();

	entities.push_back(&player);

	//Enemies Setup
	enemiesSpawnPoint.push_back(Vector2i(70, 0));
	enemiesSpawnPoint.push_back(Vector2i(75, 0));

	spawnEnemies();

	levelEditor.applyChangeCallback = [this]() {
		this->updateLevel();
	};
}

void Game::cacheWalls()
{
	wallSprites.clear();
	for (Vector2i & w : walls) {
		sf::RectangleShape rect(Vector2f(16,16));
		rect.setPosition((float)w.x * C::GRID_SIZE, (float)w.y * C::GRID_SIZE);
		rect.setFillColor(sf::Color(0x07ff07ff));
		wallSprites.push_back(rect);
	}
}

void Game::processInput(sf::Event ev) {
	if (ev.type == sf::Event::Closed) {
		win->close();
		closing = true;
		return;
	}
	if (ev.type == sf::Event::KeyReleased) {
		int here = 0;
		if (ev.key.code == Keyboard::K) {
			int there = 0;
			walls.clear();
			cacheWalls();
		}
	}
}


static double g_time = 0.0;
static double g_tickTimer = 0.0;


void Game::pollInput(double dt) {

	float lateralSpeed = 8.0;
	float maxSpeed = 40.0;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
		player.dx -= player.speed;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
		player.dx += player.speed;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
		if (player.isOnGround) {
			player.dy -= player.jumpForce;
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::T)) {

	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
		if (!wasPressed) {
			onSpacePressed();
			wasPressed = true;
		}
	}
	else {
		wasPressed = false;
	}

}

static sf::VertexArray va;
static RenderStates vaRs;
static std::vector<sf::RectangleShape> rects;

int blendModeIndex(sf::BlendMode bm) {
	if (bm == sf::BlendAlpha) return 0;
	if (bm == sf::BlendAdd) return 1;
	if (bm == sf::BlendNone) return 2;
	if (bm == sf::BlendMultiply) return 3;
	return 4;
};

void Game::update(double dt) {
	if (!showLevelEditorWindows) {
		pollInput(dt);
		processCollision();
		processEntityUpdate(dt);
	}

	g_time += dt;
	if (bgShader) bgShader->update(dt);

	beforeParts.update(dt);
	afterParts.update(dt);

	trackPlayerStats();
	manageLevelEditor();
}

 void Game::draw(sf::RenderWindow & win) {
	if (closing) return;

	sf::RenderStates states = sf::RenderStates::Default;
	sf::Shader * sh = &bgShader->sh;
	states.blendMode = sf::BlendAdd;
	states.shader = sh;
	states.texture = &tex;
	sh->setUniform("texture", tex);
	//sh->setUniform("time", g_time);
	win.draw(bg, states);

	beforeParts.draw(win);

	for (sf::RectangleShape & r : wallSprites)
		win.draw(r);

	for (sf::RectangleShape& r : rects) 
		win.draw(r);
	

	afterParts.draw(win);

	for (Entity* entity_ptr : entities) {
		Entity& entity = *entity_ptr;

		entity.drawn(win);
	}

	if (showLevelEditorWindows) {
		levelEditor.drawLevelEditorGui(&showLevelEditorWindows);
	}
}

void Game::onSpacePressed() {
	
}


bool Game::isWall(int cx, int cy)
{
	for (Vector2i & w : walls) {
		if (w.x == cx && w.y == cy)
			return true;
	}
	return false;
}

void Game::im()
{

}

//Not optimise, check all the wall in the vector with isWall. If lag, optimise that
void Game::processCollision() {
	for (Entity* entity_ptr : entities) {
		Entity& entity = *entity_ptr;

		if (!entity.canCollide) { continue; }

		entity.isCollidingOnX = false;
		entity.isCollidingOnY = false;

		//Collision Sol
		if (entity.dy >= 0 && isWall(entity.cx, entity.cy + 1)) { 
			entity.isCollidingOnY = true;
			entity.yr = 0;
		}

		//Collision Plafond
		if (entity.dy < 0 && isWall(entity.cx, entity.cy - 1)) { 
			entity.isCollidingOnY = true; 
			entity.yr = 0;
		}

		//Collision mur
		if (entity.dx > 0 && isWall(entity.cx + 1, entity.cy)) { 
			entity.isCollidingOnX = true; 
			entity.xr = 0;
		}
		if (entity.dx < 0 && isWall(entity.cx - 1, entity.cy)) { 
			entity.isCollidingOnX = true;
			entity.xr = 0;
		}
	}
}

void Game::processEntityUpdate(double deltaTime) {
	for (Entity* entity_ptr : entities) {
		Entity& entity = *entity_ptr;

		entity.update(deltaTime);
	}

	for (Enemy* enemy_ptr : enemies) {
		Enemy& enemy = *enemy_ptr;

		enemy.update(deltaTime);
	}
}

void Game::spawnEnemies() {
	for (Enemy* enemy_ptr : enemies) delete enemy_ptr;
	enemies.clear();

	entities.clear();
	entities.push_back(&player);

	for (sf::Vector2i position : enemiesSpawnPoint) {
		Enemy* newEnemy = new Enemy{};
		newEnemy->cx = position.x;
		newEnemy->cy = position.y;

		enemies.push_back(newEnemy);
		entities.push_back(newEnemy);
	}
}

void Game::playerResetPosition() {
	player.cx = playerSpawnPoint.x;
	player.cy = playerSpawnPoint.y;
}


void Game::trackPlayerStats() {
	if (ImGui::CollapsingHeader("Player")) {
		ImGui::LabelText("Player X Pos", "%i", player.cx);
		ImGui::LabelText("Player Y Pos", "%i", player.cy);
		ImGui::LabelText("Player X Ratio", "%i", player.xr);
		ImGui::LabelText("Player Y Ratio", "%i", player.yr);
		ImGui::LabelText("Player X Speed", "%f", player.dx);
		ImGui::LabelText("Player Y Pos", "%f", player.dy);
		if (ImGui::Button("Reset Player Position")) {
			playerResetPosition();
		}
	}
}

void Game::manageLevelEditor() {
	if (ImGui::CollapsingHeader("Level Editor")) {
		if (ImGui::Button("Open Level Editor")) {
			showLevelEditorWindows = !showLevelEditorWindows;
		}
	}
}

void Game::updateLevel() {
	cacheWalls();
	spawnEnemies();
	playerResetPosition();
}