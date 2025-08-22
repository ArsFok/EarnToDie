#pragma once
#include <SFML/Graphics.hpp>
#include <random>

using namespace sf;

class Zomby
{
public:
	Zomby();
	~Zomby() = default;

	void draw(RenderWindow& window);
	void update(float dt, const RectangleShape& plaer);

private:
	RectangleShape shape;
	Vector2f position;
	float velocity = 0.1f;
	std::mt19937 rng;
	std::uniform_int_distribution<int> distributionX;
};

