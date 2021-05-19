#include "StudentWorld.h"
#include "Actor.h"
#include <string>
using namespace std;


GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

int StudentWorld::init() {
	player = new Iceman(this); // Create new iceman
	for (int i = 0; i < 64; i++) {
		for (int j = 0; j < 60; j++) {
			if (i >= 30 && i <= 33 && j >= 4)// creating the tunnel
				ice[i][j] = nullptr;
			else{
				ice[i][j] = new Ice(i, j);
			}
			
		}
	}
	for (int i = 0; i < 64; i++) {
		for (int j = 60; j < 64; j++) {
				ice[i][j] = nullptr;
			
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
Iceman* StudentWorld::getPlayer() {
	return player;
}

void StudentWorld::removeIce(Iceman* p1) {
	player = p1;
	for (int i = player->getX(); i <= player->getX() + 3; i++) {
		for (int j = player->getY(); j <= player->getY() + 3; j++) {
			if (ice[i][j] != nullptr && j < 60) {
				delete ice[i][j];
				ice[i][j] = nullptr;
			}
		}
	}
}
