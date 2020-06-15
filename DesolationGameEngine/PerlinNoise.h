#pragma once
#include<vector>
#include <random>
#include <cmath>
#include <numeric>

class PerlinNoise
{
	std::vector<int> p;
public:
	PerlinNoise();

	PerlinNoise(unsigned int seed);

	double noise(double x, double y);

private:
	double fade(double t);
	double lerp(double t, double a, double b);
	double grad(int hash, double x, double y);
};

