#pragma once

struct vector2 {
	float x, y;
};

class Perlin {
public:
	Perlin(unsigned long seed) : seed(seed) {}
	float getPerlin(float x, float y);

private:
	unsigned long seed;

	float interpolate(float a0, float a1, float w);
	vector2 randomGradient(int ix, int iy);
	float dotGridGradient(int ix, int iy, float x, float y);
};
