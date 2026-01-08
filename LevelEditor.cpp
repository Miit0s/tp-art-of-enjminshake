#include "LevelEditor.hpp"

#include "C.hpp"

LevelEditor::LevelEditor(std::vector<sf::Vector2i>* wallsPosition, std::vector<sf::Vector2i>* enemiesSpawnPoint, sf::Vector2i* playerSpawn)
{
	this->wallsPosition = wallsPosition;
	this->enemiesSpawnPoint = enemiesSpawnPoint;
	this->playerSpawnPoint = playerSpawn;

	wallColor = IM_COL32(255, 255, 255, 255);
	playerSpawnColor = IM_COL32(0, 255, 0, 255);
	enemySpawnColor = IM_COL32(255, 0, 0, 255);
}

LevelEditor::~LevelEditor()
{
}

void LevelEditor::drawLevelEditorGui(bool* ptrOpen) {
	if (ImGui::Begin("Level Editor##Default", ptrOpen, ImGuiWindowFlags_NoMove)) {
		//Background
		ImVec2 origin = ImGui::GetCursorScreenPos();
		ImVec2 gridSize = ImVec2{((int)(C::RES_X / C::GRID_SIZE) * cellSize), ((int)(C::RES_Y / C::GRID_SIZE) * cellSize)};

		ImVec2 canvasTopLeft = ImVec2{ origin.x + padding, origin.y + padding };
		ImVec2 canvasBottomRight = ImVec2{ canvasTopLeft.x + gridSize.x, canvasTopLeft.y + gridSize.y };

		ImDrawList* drawList = ImGui::GetWindowDrawList();

		drawList->AddRectFilled(canvasTopLeft, canvasBottomRight, IM_COL32(50, 50, 50, 255));
		drawList->PushClipRect(canvasTopLeft, canvasBottomRight, true);

		//Interaction
		ImGuiIO& inputOutput = ImGui::GetIO();

		bool isHovered = ImGui::IsMouseHoveringRect(canvasTopLeft, canvasBottomRight);
		if (isHovered && ImGui::IsMouseDown(ImGuiMouseButton_Left)) {

			//Not a single click, so if there is lag do something to only trigger the function one time
			ImVec2 mousePosInCanvas = ImVec2{ inputOutput.MousePos.x - canvasTopLeft.x, inputOutput.MousePos.y - canvasTopLeft.y };
			int gridX = (int)(mousePosInCanvas.x / cellSize);
			int gridY = (int)(mousePosInCanvas.y / cellSize);

			manageMouseClick(gridX, gridY);
		}

		//GridLine
		for (float x = 0; x <= gridSize.x; x += cellSize)
			drawList->AddLine(ImVec2{ canvasTopLeft.x + x, canvasTopLeft.y }, ImVec2{ canvasTopLeft.x + x, canvasBottomRight.y }, IM_COL32(200, 200, 200, 40));
		for (float y = 0; y <= gridSize.y; y += cellSize)
			drawList->AddLine(ImVec2{ canvasTopLeft.x, canvasTopLeft.y + y }, ImVec2{ canvasBottomRight.x, canvasTopLeft.y + y }, IM_COL32(200, 200, 200, 40));

		drawList->PopClipRect();

		//Draw Current Level
		for (sf::Vector2i wall : *wallsPosition) {
			drawLevelSquare(wall.x, wall.y, canvasTopLeft, drawList, wallColor);
		}
		for (sf::Vector2i enemies : *enemiesSpawnPoint) {
			drawLevelSquare(enemies.x, enemies.y, canvasTopLeft, drawList, enemySpawnColor);
		}
		drawLevelSquare(playerSpawnPoint->x, playerSpawnPoint->y, canvasTopLeft, drawList, playerSpawnColor);

		//Button
		ImGui::SetCursorScreenPos(ImVec2(origin.x, canvasBottomRight.y + padding));

		if (ImGui::Button("Wall Tool")) currentEntitySelected = wall;
		ImGui::SameLine();
		if (ImGui::Button("Move Player Spawn Point Tool")) currentEntitySelected = playerSpawn;
		ImGui::SameLine();
		if (ImGui::Button("Place Enemy Spawn Point Tool")) currentEntitySelected = enemySpawn;
		ImGui::SameLine();
		if (ImGui::Button("Ereaser Tool")) currentEntitySelected = ereaser;
		ImGui::SameLine();
		ImGui::Text("Current Tool : %s", getToolName(currentEntitySelected).c_str());

		if (ImGui::Button("Apply change")) {
			applyChangeCallback();
		}
	}
	ImGui::End();
}

std::string LevelEditor::getToolName(ToolType toolType) {
	switch (toolType) 
	{
	case LevelEditor::wall: return "Wall Tool";
	case LevelEditor::playerSpawn: return "Move Player Spawn Point Tool";
	case LevelEditor::enemySpawn: return "Place Enemy Spawn Point Tool";
	case LevelEditor::ereaser: return "Ereaser Tool";
	}
}

void LevelEditor::manageMouseClick(int x, int y) {
	switch (currentEntitySelected)
	{
	case LevelEditor::wall:
		{
			bool wallExists = false;

			for (sf::Vector2i wall : *wallsPosition) {
				if (wall.x == x && wall.y == y) {
					wallExists = true;
					break;
				}
			}

			if (!wallExists) {
				wallsPosition->push_back(sf::Vector2i(x, y));
			}
		}
		break;
	case LevelEditor::playerSpawn:
		playerSpawnPoint->x = x;
		playerSpawnPoint->y = y;
		break;
	case LevelEditor::enemySpawn:
	{
		bool enemyExists = false;

		for (sf::Vector2i enemy : *enemiesSpawnPoint) {
			if (enemy.x == x && enemy.y == y) {
				enemyExists = true;
				break;
			}
		}

		if (!enemyExists) {
			enemiesSpawnPoint->push_back(sf::Vector2i(x, y));
		}
	}
		break;
	case LevelEditor::ereaser:
		{
			for (auto wallPtr = wallsPosition->begin(); wallPtr != wallsPosition->end();) {
				if (wallPtr->x == x && wallPtr->y == y) {
					wallPtr = wallsPosition->erase(wallPtr);
					break;
				}
				else {
					++wallPtr;
				}
			}

			for (auto enemyPtr = enemiesSpawnPoint->begin(); enemyPtr != enemiesSpawnPoint->end();) {
				if (enemyPtr->x == x && enemyPtr->y == y) {
					enemyPtr = enemiesSpawnPoint->erase(enemyPtr);
					break;
				}
				else {
					++enemyPtr;
				}
			}
		}
		break;
	default:
		break;
	}
}

void LevelEditor::drawLevelSquare(int gridX, int gridY, ImVec2 gridTopLeft, ImDrawList* drawList, ImU32 squareColor) {
	drawList->AddRectFilled(
		ImVec2{ gridX * cellSize + gridTopLeft.x, gridY * cellSize + gridTopLeft.y },
		ImVec2{ gridX * cellSize + cellSize + gridTopLeft.x, gridY * cellSize + cellSize + gridTopLeft.y },
		squareColor
	);
}