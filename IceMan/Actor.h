
#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class BaseObject : public GraphObject {
public: 
	BaseObject(int imageID, int startX, int startY, Direction startDirection, float size, unsigned int depth);

};


class Actor : public BaseObject {
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

class Ice : public BaseObject {
public:
	Ice(int x, int y);

};

#endif // ACTOR_H_
