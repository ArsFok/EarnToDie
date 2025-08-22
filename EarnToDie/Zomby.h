#pragma once
#include <SFML/Graphics.hpp>
#include <random>

using namespace sf;

class Zomby
{
public:
	Zomby();
	~Zomby() = default;

	RectangleShape shape;
	Vector2f position;
	float velocity;
	std::mt19937 rng;
	std::uniform_int_distribution<int> distributionX;
};

