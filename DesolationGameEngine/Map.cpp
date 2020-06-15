#include "Map.h"
#include "TextureManager.h"
#include <fstream>
#include <random>


Map::Map(){
	
}

Map::~Map() {
	
}

void Map::loadMap(std::string path, int sizeX, int sizeY) {
	char tile;
	std::fstream mapFile;
	mapFile.open(path);

	for (int y = 0; y < sizeY; y++) {
		for (int x = 0; x < sizeX; x++) {
			mapFile.get(tile);
			Game::AddTile(atoi(&tile), x * 32, y * 32);
			mapFile.ignore();
		}
	}

	mapFile.close();
}

int Map::generateTile(const std::vector<std::pair<int, float>> &tileProbsList) {
	int r = std::rand() % 100 + 1;
	int runningProb = 0;
	for (int i = 0; i < tileProbsList.size()-1; i++) {
		runningProb += tileProbsList[i].second;
		if (r <= runningProb)
			return tileProbsList[i].first;
	}
	return tileProbsList[tileProbsList.size() - 1].first;
}


void Map::tileFill(std::vector<std::vector<int>> &blankMap, int x, int y, int prevTile, int currTile,
	const std::vector<std::pair<int, float>> &tileProbsList) {
	
	int sizeX = blankMap[0].size();
	int sizeY = blankMap.size();

	if (x < 0 || y < 0 || x >= sizeX || y >= sizeY) {
		return;
	}

	if (prevTile >= 0) {
		return;
	}
	blankMap[y][x] = currTile;
	int nextTile = generateTile(tileProbsList);// randomly get tile
	int dir = std::rand() % 3;
	
}

std::unordered_map<std::string, std::vector<std::vector<int>>> Map::perlinNoiseMap(std::vector<std::vector<int>> &map, float modifier, bool edgesAreWalls) {
	int newPoint;
	std::unordered_map<std::string, std::vector<std::vector<int>>> maps;

	std::vector<std::vector<int>> mapMask1(map.size(), std::vector<int>(map[0].size(), 0));
	std::vector<std::vector<int>> mapMask2(map.size(), std::vector<int>(map[0].size(), 0));

	for (int x = 0; x < map[0].size(); x++) {
		for (int y = 0; y < map.size(); y++) {
			if (edgesAreWalls && (x == 0 || y == 0 || x == map[0].size() || y == map.size())) {
				map[y][x] == -1;
			}
			else {
				newPoint = (int)round(pn.noise(x * modifier, y * modifier));
				if (newPoint == 1) {
					newPoint = (int)round(pn.noise(x * modifier, y * modifier)) * newPoint + rand() % 8;
				}
				else {
					newPoint = -1;
				}

				if (newPoint >= 8 && newPoint < 16) {
					mapMask1[y][x] = 0;
					mapMask2[y][x] = 1;
				}
				else if (newPoint == -1) {
					mapMask1[y][x] = 0;
					mapMask2[y][x] = 0;
				}
				else {
					mapMask1[y][x] = 1;
					mapMask2[y][x] = 0;
				}
				map[y][x] = newPoint;
			}
		}
	}
	maps.insert({ "Map", map });
	maps.insert({ "MapMask1", mapMask1 });
	maps.insert({ "MapMask2", mapMask2 });
	return maps;
}

std::unordered_map<std::string, std::vector<std::vector<int>>> Map::directionalTunnelMap(std::vector<std::vector<int>> &map, int minWidth,
	int maxWidth, int maxChange, int roughness, int curvyness) {

	int newPoint;
	std::unordered_map<std::string, std::vector<std::vector<int>>> maps;

	std::vector<std::vector<int>> mapMask1(map.size(), std::vector<int>(map[0].size(), 0));
	std::vector<std::vector<int>> mapMask2(map.size(), std::vector<int>(map[0].size(), 0));

	int tunnelWidth = 1;
	int x = map[0].size() / 2;

	for (int i = -tunnelWidth; i <= tunnelWidth; i++) {
		map[0][x + i];
	}
	for (int y = 1; y < map.size(); y++) {
		if ((rand() % 100) > roughness) {
			int widthChange = rand() % (abs(minWidth) + maxWidth + 1) + (minWidth);
			//Add it to our tunnel width value
			tunnelWidth += widthChange;
			if (tunnelWidth < minWidth) {
				tunnelWidth = minWidth;
			}
			//check that the path 
			if (tunnelWidth > maxWidth) {
				tunnelWidth = maxWidth;
			}
		}

		//Check if we can change the curve
		if ((rand() % 100) > curvyness) {
			int xChange = rand() % (2*maxChange + 1) + (maxChange);
			x += xChange;
			//Check we arent too close to the left side of the map
			if (x < maxWidth) {
				x = maxWidth;
			}
			//Check we arent too close to the right side of the map
			if (x > map[0].size() - maxWidth) {
				x = map[0].size() - maxWidth;
			}
		}

		//Work through width of the tunnel
		for (int i = -tunnelWidth; i <= tunnelWidth; i++) {
			newPoint = rand() % 8 + 8;
			if (newPoint >= 8 && newPoint < 16) {
				mapMask1[y][x] = 0;
				mapMask2[y][x] = 1;
			}
			map[y][x + i];
		}
	}
	maps.insert({ "Map", map });
	maps.insert({ "MapMask1", mapMask1 });
	maps.insert({ "MapMask2", mapMask2 });
	return maps;
}

std::vector<std::vector<int>> Map::addEdges(std::vector<std::vector<int>> &mapMask, bool isSet2) {

	int x, y;
	for (std::pair<int, std::vector<std::vector<int>>> edge : edgesDict1) {
		for (int j = 0; j < mapMask.size(); j++) {
			for (int i = 0; i < mapMask[0].size(); i++) {
				x = i;
				y = j;
				bool checker = true; //Checks if a false match is detected, breaks out of kernel loops
				for (int k = 0; k < 3; k++) {
					for (int l = 0; l < 3; l++) {
						if (edge.second[k][l] != mapMask[y][x]) { // Check if edge kernel doesn't match conditions
							checker = false;
							break;
						}
						x++;
					}
					y++;
					if (checker == false)
						break;
				}
				if (checker) { // If edge kernel checker is true, then modify value in the mask.
					mapMask[y + 1][x + 1] = edge.first + isSet2 * 12;
				}
			}
		}
	}
}

std::vector<std::vector<int>> Map::generateMap(std::string tileType, int sizeX, int sizeY,
	const std::vector<std::pair<int, float>> &tileProbsList) {
	
	std::string tileDir;
	if (tileType == "DarkSpace") {
		tileDir = "assets/DarkSpaceTiles/";
	}
	//Loop through size of map
	//Do check to see adjacent tile types,
	//If specific type (i.e. edge) then randomly select from edge types
	//Else randomly pick from entire collection of tiles for spot x,y
	int startingTile = generateTile(tileProbsList);
	std::vector<std::vector<int>> mapArr(sizeY, std::vector<int> (sizeX, -1)); 

	int mapType = rand();
	bool perlin = (mapType >= RAND_MAX / 4) ? true : false;
	bool tunnel = (mapType < RAND_MAX / 4) ? true : false;
	std::unordered_map<std::string, std::vector<std::vector<int>>> mapLists;
	if (perlin) {
		mapLists = perlinNoiseMap(mapArr, 0.2, true);
	}
	else if (tunnel) {
		mapLists = directionalTunnelMap(mapArr, 5, 20, 10, 30, 50);
	}
	else {
		std::cout << "Error Generating Map: Invalid map type." << std::endl;
	}
	// Generate map masks
	mapLists["MapMask1"] = addEdges(mapLists["MapMask1"], false);
	mapLists["MapMask2"] = addEdges(mapLists["MapMask2"], true);

	for (int j = 0; j < mapArr.size(); j++) {
		for (int i = 0; i < mapArr[0].size(); i++) {
			if (mapLists["MapMask1"][j][i] > 1)
				mapArr[j][i] = mapLists["MapMask1"][j][i];
			if (mapLists["MapMask2"][j][i] > 1)
				mapArr[j][i] = mapLists["MapMask2"][j][i];
		}
	}
	return mapArr;
}

