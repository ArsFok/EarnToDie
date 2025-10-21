#pragma once
#include <memory>
#include "MovingObject.h"

using namespace std;
using namespace sf;


class EnemyController{
public:
	EnemyController(unique_ptr<MovingObject> enemyObj, float spd) : enemy(move(enemyObj)), speed(spd) {}

	bool update();
	void draw(RenderWindow& window);
	MovingObject* getEnemy();

private:
	unique_ptr<MovingObject> enemy;
	float speed;
};

