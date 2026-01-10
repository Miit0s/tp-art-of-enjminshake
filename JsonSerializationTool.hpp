#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <tuple>

#include "SFML/System.hpp"

class JsonSerializationTool
{
public:
	std::string vector2iToJson(sf::Vector2i vector);

	void savelLevel(std::string filename, std::vector<sf::Vector2i> wallsPosition, std::vector<sf::Vector2i> enemiesSpawnPoint, sf::Vector2i playerSpawnPoint);
	std::tuple<std::vector<sf::Vector2i>, std::vector<sf::Vector2i>, sf::Vector2i> loadLevel(std::string filename); //Return walls, then ennemies spawn point and player spawn point

private:
	enum DataType {
		wallsData,
		ennemiesData,
		playerData
	};

	DataType currentDataType{ playerData };
};

