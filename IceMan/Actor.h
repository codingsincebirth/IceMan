
#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"
#include <stack>

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class BaseObject : public GraphObject {
public: 
	BaseObject(int imageID, int startX, int startY, Direction startDirection, float size, unsigned int depth);
	bool isAlive();
	void isDead();
private:
	bool status;
};

class Actor : public BaseObject {
public:
	Actor(int imageID, int startX, int startY, Direction startDirection, float size, unsigned int depth, StudentWorld* w, int hp);
	virtual void annoy(int dmg) = 0;
	virtual void doSomething() = 0;
	virtual ~Actor();
	StudentWorld* getWorld();
	int getNum_hp();

private:
	StudentWorld* world;
	int num_hp;
};

class Goodie :public Actor {
public:
	Goodie(int imageID, int startX, int startY, Direction startDirection, float size, unsigned int depth, StudentWorld* w);
	virtual void doSomething();
	virtual int classType();
	/*
	1 boulder
  	2 barrel
  	3 gold
	*/
	virtual ~Goodie();
	void annoy(int dmg);
};

class Iceman : public Actor {
public:
	Iceman(StudentWorld* w);
	void doSomething();
	void annoy(int dmg);
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

class Barrel : public Goodie {
public:
	Barrel(int x, int y, StudentWorld* w);
	void doSomething();
	int classType();
	virtual ~Barrel();
};

class Boulder : public Goodie {
public:
	Boulder(int x, int y, StudentWorld* w);
	void doSomething();
	int classType();
	virtual ~Boulder();
	void setState(int st);
	int getState();
private:
	int state;
	int waitingTicks;

};

//class Protestor : public Actor {
//public:
//	Protestor(StudentWorld* w);
//	void doSomething();
//	~Protestor();
//	int check_if_rest();
//	void leaveIceField();
//	void shout();
//protected:
//	int numSquaresToMoveInCurrentDirection;
//	int maxMoves;
//	int ticksToWaitBetweenMoves;
//	bool leavetheicefield = false;
//private:
//	std::stack<Position*>leave;
//};

#endif // ACTOR_H_
