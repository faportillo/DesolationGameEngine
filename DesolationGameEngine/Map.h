#pragma once
#include <string>
#include <unordered_map>
#include <utility>
#include "PerlinNoise.h"

class Map
{
public:
	static PerlinNoise pn;

	Map();
	~Map();

	static void loadMap(std::string path, int sizeX, int sizeY);

	static std::vector<std::vector<int>> generateMap(std::string tileType, int sizeX, int sizeY, const std::vector<std::pair<int, float>> &tileProbsList);
	static std::unordered_map<std::string, std::vector<std::vector<int>>> perlinNoiseMap(std::vector<std::vector<int>> &map, float modifier, bool edgesAreWalls);
	static std::unordered_map<std::string, std::vector<std::vector<int>>> directionalTunnelMap(std::vector<std::vector<int>> &map, int minWidth,
		int maxWidth, int maxChange, int roughness, int curvyness);

private:
	
	// Edge kernels for printing corner tiles for more realism
	std::vector<std::vector<int>> left_up_corner = {
		{0, 0, 0},
		{0, 1, 1},
		{0, 1, 1}
	};
	std::vector<std::vector<int>> left_up_dip = {
		{0, 1, 1},
		{1, 1, 1},
		{1, 1, 1}
	};
	std::vector<std::vector<int>> left_side = {
		{0, 1, 1},
		{0, 1, 1},
		{0, 1, 1}
	};

	std::vector<std::vector<int>> right_up_corner = {
		{0, 0, 0},
		{0, 1, 1},
		{0, 1, 1}
	};
	std::vector<std::vector<int>> right_up_dip = {
		{1, 1, 0},
		{1, 1, 1},
		{1, 1, 1}
	};
	std::vector<std::vector<int>> right_side = {
		{1, 1, 0},
		{1, 1, 0},
		{1, 1, 0}
	};

	std::vector<std::vector<int>> left_down_corner = {
		{0, 1, 1},
		{0, 1, 1},
		{0, 0, 0}
	};
	std::vector<std::vector<int>> left_down_dip = {
		{1, 1, 1},
		{1, 1, 1},
		{0, 1, 1}
	};
	std::vector<std::vector<int>> down_side = {
		{1, 1, 1},
		{1, 1, 1},
		{0, 0, 0}
	};

	std::vector<std::vector<int>> right_down_corner = {
		{1, 1, 0},
		{1, 1, 0},
		{0, 0, 0}
	};
	std::vector<std::vector<int>> right_down_dip = {
		{1, 1, 1},
		{1, 1, 1},
		{1, 1, 0}
	};
	std::vector<std::vector<int>> up_side = {
		{0, 0, 0},
		{1, 1, 1},
		{1, 1, 1}
	};

	const std::vector<std::pair<int, float>> darkTileProbs = {
		std::make_pair(0, 0.3), // Regular tiles - type 1
		std::make_pair(1, 0.4), // Regular tiles - type 2
		std::make_pair(2, 0.15), // Edge tiles - type 1
		std::make_pair(3, 0.15) // Edge tiles - type 2
	};

	static const std::unordered_map<int, std::vector<std::vector<int>>> edgesDict1 = {
		{16, left_up_corner},
		{17, right_up_corner},
		{18, left_down_corner},
		{19, right_down_corner},
		{20, right_down_dip},
		{21, down_side},
		{22, left_down_dip},
		{23, left_side},
		{24, right_side},
		{25, right_up_dip},
		{26, up_side},
		{27, left_up_dip}
	};

		
	static int generateTile(const std::vector<std::pair<int, float>> &tileProbsList);

	static std::vector<std::vector<int>> addEdges(std::vector<std::vector<int>> &mapMask, bool isSet2);

	static void tileFill(std::vector<std::vector<int>> &input, int x, int y,
		int prevTile, int currTile, const std::vector<std::pair<int, float>> &tileProbsList);

};

