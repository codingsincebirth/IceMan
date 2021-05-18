#include "StudentWorld.h"
#include "Actor.h"
#include <string>
using namespace std;


GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

int StudentWorld::init() {
	player = new Iceman(this); // Create new iceman
	for (int i = 0; i < 64; i++) {
		for (int j = 0; j < 60; j++) {
			if (i >= 30 && i <= 30 && j >= 4)// creating the tunnel
				ice[i][j] = nullptr;
			ice[i][j] = new Ice(i, j);
		}
	}
	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	player->doSomething();
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp() {
	for (int i = 0; i < 60; i++) {
		for (int j = 0; j < 60; j++) {
			if (ice[i][j] != nullptr) {
				delete ice[i][j];
				ice[i][j] = nullptr;
			}
		}
	}
	delete player;
}

void StudentWorld::removeIce(Iceman* p1) {
	for (int i = player->getX(); i <= player->getX() + 3; i++) {
		for (int j = player->getY(); j <= player->getY() + 3; j++) {
			if (ice[i][j] != nullptr && j < 60) {
				delete ice[i][j];
				ice[i][j] = nullptr;
			}
		}
	}
}