#pragma once
#include <memory>
#include "MovingObject.h"
#include "ZombieState.h"

using namespace std;
using namespace sf;


class EnemyController{
public:
	EnemyController(unique_ptr<MovingObject> enemyObj, float spd);

	bool update();
	void draw(RenderWindow& window);
	MovingObject* getEnemy();

	void applyKnockback(const sf::Vector2f& direction, float force = 300.0f, float duration = 0.5f);
	void kill();
	bool shouldRemove() const;
	bool isDead() const { return zombieState.isDead(); }

	bool isKnockback() const { return zombieState.isKnockback(); }
	bool isAlive() const { return zombieState.isAlive(); }

	void takeDamage();

private:
	int hitCount = 0;
	const int maxHits = 2;

	std::unique_ptr<MovingObject> enemy;
	ZombieState zombieState;
	float speed;
	bool markedForRemoval = false;
};

