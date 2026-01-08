#pragma once

#include "imgui.h"
#include <vector>
#include <string>
#include <functional>

class LevelEditor
{
public:
	LevelEditor(std::vector<sf::Vector2i>* wallsPosition, std::vector<sf::Vector2i>* enemiesSpawnPoint, sf::Vector2i* playerSpawn);
	~LevelEditor();

	void drawLevelEditorGui(bool* ptrOpen);

	std::function<void()> applyChangeCallback;

private:
	float cellSize{ 12.0f };
	float padding{ 20.0f };
	float minCanvasSize{ 500 };

	//Color for entiy
	ImU32 wallColor{};
	ImU32 playerSpawnColor{};
	ImU32 enemySpawnColor{};

	//LevelInfo
	std::vector<sf::Vector2i>* wallsPosition;
	std::vector<sf::Vector2i>* enemiesSpawnPoint;
	sf::Vector2i* playerSpawnPoint;

	enum ToolType {
		wall,
		playerSpawn,
		enemySpawn,
		ereaser
	};
	ToolType currentEntitySelected{};
	std::string getToolName(ToolType toolType);

	void manageMouseClick(int x, int y);
	void drawLevelSquare(int gridX, int gridY, ImVec2 gridTopLeft, ImDrawList* drawList, ImU32 squareColor);
};

