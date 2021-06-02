#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_
#include "GameWorld.h"
#include "GameConstants.h"
#include "GraphObject.h"
#include <string>
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class Iceman;
class Ice;
class Boulder;
class Goodie;


class StudentWorld : public GameWorld {
public:
	enum Direction { none, up, down, left, right };
	StudentWorld(std::string assetDir) : GameWorld(assetDir) {}
	virtual int init();
	virtual int move();
	virtual void cleanUp();
	void removeIce(Iceman* p1);
	Iceman* getPlayer();
	int distance(int x1, int x2, int y1, int y2);
	int max(int a, int b);
	int min(int a, int b);
	bool canDistribute(int x, int y);
	bool withinDistance(int x, int y, double radius);
	bool withinProtestorDistance(int x, int y, double radius);
	bool checkUP(int x, int y);
	bool checkDOWN(int x, int y);
	bool checkLEFT(int x, int y);
	bool checkRIGHT(int x, int y);
	void shoot(Iceman* p1);
	void decBarrels();
private:
	std::vector<Goodie*>goodies;
	Iceman* player;
	Ice* ice[64][64];
	Boulder* boulder;
	int num_barrels;
};

#endif // STUDENTWORLD_H_