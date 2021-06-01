#include "StudentWorld.h"
#include "Actor.h"
#include <string>
#include <cmath>
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
			else {
				ice[i][j] = new Ice(i, j);
			}

		}
	}
	for (int i = 0; i < 64; i++) {
		for (int j = 60; j < 64; j++) {
			ice[i][j] = nullptr;

		}
	}
	int B = min((getLevel() / 2) + 2, 9);
	for (int i = 0; i < B; i++) {
		int x, y;
		for (;;)
		{
			x = rand() % 61;
			y = rand() % 37 + 20;
			if (x < 26 || x > 33 && canDistribute(x, y))
				break;
		}
		Goodie* g = new Boulder(x, y, this);
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				delete ice[g->getX() + j][g->getY() + k];
				ice[g->getX() + j][g->getY() + k] = nullptr;
			}
		}
		goodies.push_back(g);
	}
	return GWSTATUS_CONTINUE_GAME;
}
bool StudentWorld::canDistribute(int x, int y) {
	for (int i = 0; i < goodies.size(); i++)
	{
		if (distance(goodies[i]->getX() + 2, goodies[i]->getY() + 2, x + 2, y + 2) <= 6.0)
			return false;
	}
	return true;
}

int StudentWorld::move()
{
	player->doSomething();
	if (player->isAlive() != true)
	{	
		decLives();
		return GWSTATUS_PLAYER_DIED;
	}
	
	for (int i = 0; i < goodies.size(); i++) {
		goodies[i]->doSomething();
	}
	for (int i = 0; i < goodies.size(); i++) {
		if (goodies[i]->isAlive() != true) {
			delete goodies[i];
			goodies[i] = nullptr;
			goodies.erase(goodies.begin() + i);
			--i;
		}
	}
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
	for (int i = 0; i < goodies.size(); i++) {
		delete goodies[i];
		goodies[i] = nullptr;
		goodies.erase(goodies.begin() + i);
		--i;
	}
	delete player;
}
Iceman* StudentWorld::getPlayer() {
	return player;
}

void StudentWorld::removeIce(Iceman* p1) {
	bool dig = false;
	player = p1;
	for (int i = player->getX(); i <= player->getX() + 3; i++) {
		for (int j = player->getY(); j <= player->getY() + 3; j++) {
			if (ice[i][j] != nullptr && j < 60) {
				delete ice[i][j];
				ice[i][j] = nullptr;
				dig = true;
			}
		}
	}
	if (dig == true) {
		playSound(SOUND_DIG);
	}
}

bool StudentWorld::checkForIce(Boulder* b1) {
	boulder = b1;
	int j = boulder->getY() - 1;
	for (int i = boulder->getX(); i <= boulder->getX() + 3; i++) {
		if (ice[i][j] != nullptr && j < 60) {
			return false;
		}
		else {
			return true;
		}

	}
}

int StudentWorld::min(int a, int b) {
	if (a < b)
		return a;
	else {
		return b;
	}
}
int StudentWorld::max(int a, int b) {
	if (a > b)
		return a;
	else {
		return b;
	}
}

int StudentWorld::distance(int x1, int x2, int y1, int y2) {
	return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}
