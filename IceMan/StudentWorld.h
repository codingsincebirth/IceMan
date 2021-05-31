#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_
#include "GameWorld.h"
#include "GameConstants.h"
#include <string>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class Iceman;
class Ice;
class Boulder;

class StudentWorld : public GameWorld {
public:
	StudentWorld(std::string assetDir) : GameWorld(assetDir) {}
	virtual int init();
	virtual int move();
	virtual void cleanUp();
	void removeIce(Iceman* p1);
	Iceman* getPlayer();
	bool checkForIce(Boulder* b1);
private:
	Iceman* player;
	Ice* ice[64][64];
	Boulder* boulder;
};

#endif // STUDENTWORLD_H_