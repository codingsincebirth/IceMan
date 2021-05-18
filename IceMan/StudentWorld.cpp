#include "StudentWorld.h"
#include <string>
#include "Actor.h"
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

int StudentWorld::init() {
	player = new Iceman(this); // Create new iceman
	for (int i = 0; i < 60; i++) {
		for (int j = 0; j < 60; j++) {
			ice[i][j] = new Ice(i, j);
		}
	}
	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	player->doSomething();
	if (player->isAlive() != true) {
		decLives();
		return GWSTATUS_PLAYER_DIED;
	}
	else if (player->isAlive() == true) {
		return GWSTATUS_CONTINUE_GAME;
	}
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

}