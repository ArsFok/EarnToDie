#pragma once
#include <SFML/Graphics.hpp>;
#include "Entity.h"

using namespace sf;

class EntityController
{
public:

	EntityController();
	~EntityController();

	Entity* entity;

	void update(RenderWindow& window);
	Entity* getEntity();

private:

	void checkInputs();
	void inputMove();
};
