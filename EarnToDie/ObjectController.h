#pragma once
#include <memory>
#include "StaticObject.h"

using namespace std;
using namespace sf;


class ObjectController {
public:
	ObjectController(unique_ptr<StaticObject> enemyObj, float spd) : subject(move(enemyObj)), speed(spd) {}

	bool update();
	void draw(RenderWindow& window);
	StaticObject* getEnemy();

private:
	unique_ptr<StaticObject> subject;
	float speed;
};

