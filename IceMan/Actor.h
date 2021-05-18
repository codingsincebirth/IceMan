#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor : public GraphObject {
public:
	Actor(int imageID, int startX, int startY, Direction startDirection, float size, unsigned int depth, StudentWorld* w, int hp);
	bool isAlive();
	void isDead();
	void irritated(int dmg);
	virtual void doSomething() = 0;
	virtual ~Actor();
	StudentWorld* getWorld();
private:
	StudentWorld* world;
	int num_hp;
	bool status_alive;
};
class Iceman : public Actor {
public:
	Iceman(StudentWorld* w);
	void doSomething();
	virtual ~Iceman();
private:
	int num_water;
	int num_sonarCharge;
	int num_goldNugs;

};

class Ice : public Actor {
public:
	Ice(int x, int y) : Actor(IID_ICE, x, y, right, 0.25, 3, getWorld(), 0) {}
	virtual ~Ice();
	virtual void doSomething();

};

#endif // ACTOR_H_


