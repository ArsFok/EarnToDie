#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.h"
#include "Zomby.h"

using namespace sf;

class EntityController {
public:

	EntityController();
	~EntityController();

	Entity* entity;
	Zomby* zomby;

	void update(RenderWindow &window);

private:
	void checkInputs();
	void inputMove();
	void zombyMove();
	bool checkCollision();
	void handleCollision();

	bool boundingBoxCollision();

	Clock clock;
	Clock spawnClock;
	float dt = 0.0f;
	std::vector<Zomby*> zombies;
};