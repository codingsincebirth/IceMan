
#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"
#include <stack>

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor : public GraphObject {
public:
	Actor(int imageID, int startX, int startY, Direction startDirection, float size, unsigned int depth, StudentWorld* w, int hp);
	bool isAlive();
	void isDead();
	virtual void annoy(int dmg) = 0;
	virtual void doSomething() = 0;
	virtual ~Actor();
	StudentWorld* getWorld();
	int getNum_hp();

private:
	StudentWorld* world;
	int num_hp;
	bool status;
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
	4 water pool
	5 sonar
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
	void inc_water();
	void inc_gold();
	void dec_gold();
	void inc_sonar();
	int getWater();
	int getSonar();
	int getGoldNugs();
private:
	int num_water;
	int num_sonarCharge;
	int num_goldNugs;

};

class Ice : public GraphObject {
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

class Squirt : public Goodie {
public:
	Squirt(int x, int y, StudentWorld* w, Iceman* p1);
	void doSomething();
	virtual ~Squirt();
private:
	int travelDistance;
	Iceman* player;
};

class Waterpool : public Goodie {
public:
	Waterpool(int x, int y, StudentWorld* w);
	void doSomething();
	virtual ~Waterpool();
private:
	int num_ticks;
};

class Perm_Nuggets : public Goodie {
public:
	Perm_Nuggets(int x, int y, StudentWorld* w);
	void doSomething();
	virtual ~Perm_Nuggets();
};

class Temp_Nuggets : public Goodie {
public:
	Temp_Nuggets(int x, int y, StudentWorld* w);
	void doSomething();
	virtual~Temp_Nuggets();
private:
	int num_ticks;
};

class Sonar : public Goodie {
public:
	Sonar(int x, int y, StudentWorld* w);
	void doSomething();
	int classType();
	~Sonar();
	
private:
	int num_ticks;
};


#endif // ACTOR_H_
