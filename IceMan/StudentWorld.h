#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_
#include "GameWorld.h"
#include "GameConstants.h"
#include <string>
#include "Actor.h"

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld {
public:
	StudentWorld(std::string assetDir) : GameWorld(assetDir) {}
	virtual int init();
	virtual int move();
	virtual void cleanUp();
	void removeIce(Iceman* p1);
	Iceman* getPlayer() { return player };

private:
	Iceman* player;
	Ice* ice[64][64];
};

#endif // STUDENTWORLD_H_