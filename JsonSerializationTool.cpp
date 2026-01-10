#include "JsonSerializationTool.hpp"

std::string JsonSerializationTool::vector2iToJson(sf::Vector2i vector) {
	return "{\"x\":" + std::to_string(vector.x) + ",\"y\":" + std::to_string(vector.y) + "}";
}

void JsonSerializationTool::savelLevel(std::string filename, std::vector<sf::Vector2i> wallsPosition, std::vector<sf::Vector2i> enemiesSpawnPoint, sf::Vector2i playerSpawnPoint) {
	std::ofstream file(filename);

	file << "{\n";

	//Save player spawn point
	file << "	\"playerSpawnPoint\": " << vector2iToJson(playerSpawnPoint) << ",\n";

	//Save enemies spawn point
	file << "	\"enemiesSpawnPoint\": [\n";
	for (size_t i = 0; i < enemiesSpawnPoint.size(); ++i) {
		file << "	" << vector2iToJson(enemiesSpawnPoint[i]) << (i == enemiesSpawnPoint.size() - 1 ? "" : ",");
		file << "\n";
	}
	file << "	],\n";

	//Save walls position
	file << "	\"walls\": [\n";
	for (size_t i = 0; i < wallsPosition.size(); ++i) {
		file << "	" << vector2iToJson(wallsPosition[i]) << (i == wallsPosition.size() - 1 ? "" : ",");
		file << "\n";
	}
	file << "	]\n";

	file << "}";
	file.close();
}

//Return walls, then ennemies spawn point and player spawn point
std::tuple<std::vector<sf::Vector2i>, std::vector<sf::Vector2i>, sf::Vector2i> JsonSerializationTool::loadLevel(std::string filename) {
	std::vector<sf::Vector2i> wallsPosition;
	std::vector<sf::Vector2i> ennemiesSpawnPoint;
	sf::Vector2i playerSpawnPoint;

	std::ifstream file(filename);
	
	if (!file.is_open()) return std::make_tuple(wallsPosition, ennemiesSpawnPoint, playerSpawnPoint);;

	std::string line;
	while (std::getline(file, line)) {
		if (line.find("playerSpawnPoint") != std::string::npos) {
			currentDataType = playerData;
		}
		else if (line.find("enemiesSpawnPoint") != std::string::npos) {
			currentDataType = ennemiesData;
		}
		else if (line.find("walls") != std::string::npos) {
			currentDataType = wallsData;
		}

		if (line.find("\"x\":") != std::string::npos) {
			int x, y;

			size_t xPosition = line.find("\"x\":") + 4;
			size_t commaPos = line.find(",", xPosition);
			size_t yPosition = line.find("\"y\":") + 4;
			size_t closePos = line.find("}", yPosition);

			x = std::stoi(line.substr(xPosition, commaPos - xPosition));
			y = std::stoi(line.substr(yPosition, closePos - yPosition));

			switch (currentDataType)
			{
			case wallsData:
				wallsPosition.push_back(sf::Vector2i{ x, y });
				break;
			case ennemiesData:
				ennemiesSpawnPoint.push_back(sf::Vector2i{ x, y });
				break;
			case playerData:
				playerSpawnPoint = sf::Vector2i{ x, y };
				break;
			default:
				break;
			}
		}
	}

	return std::make_tuple(wallsPosition, ennemiesSpawnPoint, playerSpawnPoint);
}