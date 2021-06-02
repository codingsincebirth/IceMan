#include "StudentWorld.h"
#include "Actor.h"
#include "GameConstants.h"
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

bool StudentWorld::withinDistance(int x, int y, double radius) {
	if (distance(x, player->getX(), y, player->getY()) <= radius) {
		return true;
	}
	else {
		return false;
	}
}

bool StudentWorld::checkUP(int x, int y)
{
	bool flag = false;
	for (int i = x; i < x + 4; i++)
	{
		if (y < 60) {
			if (ice[i][y + 4] == nullptr)
				flag = true;
			else
				flag = false;
		}
		//if (isBoulder(x, y + 1, 3.0))
		//	return false;
		else {
			flag = false;
		}
	}
	return flag;
}

bool StudentWorld::checkDOWN(int x, int y)
{
	bool flag = false;
	for (int i = x; i < x + 4; i++)
	{
		if (y > 0)
		{
			if (ice[i][y - 1] == nullptr)
				flag = true;
			else
				flag = false;

			//if (isBoulder(x, y - 1, 3.0))
			//	return false;
		}
		else {
			flag = false;
		}
	}
	return flag;
}

bool StudentWorld::checkLEFT(int x, int y)
{
	bool flag = false;
	for (int j = y; j < y + 4; j++)
	{
		if (x > 0)
		{
			if (ice[x - 1][j] == nullptr)
				flag = true;
			else
				flag = false;
		}
		else {
			flag = false;
		}

		//if (isBoulder(x - 1, y, 3.0))
		//	return false;

	}
	return flag;
}

bool StudentWorld::checkRIGHT(int x, int y)
{
	bool flag = false;
	for (int j = y; j < y + 4; j++)
	{
		if (x < 60)
		{
			if (ice[x + 4][j] == nullptr)
				flag = true;
			else
				flag = false;
		}
		else {
			flag = false;
		}
		//if (isBoulder(x + 1, y, 3.0))
		//	return false;
	}
	return flag;
}

void StudentWorld::shoot(Iceman* p1) {
	Goodie* g;
	switch (player->getDirection()) {
	case up:
		if (p1->getY() <= 60)
		{
			bool flag = false;
			for (int i = 0; i < 4; i++)
			{
				if (checkUP(p1->getX(), p1->getY() + i) != true) {
					flag = false;
					break;
				}
			}
			if (flag == true) {
				g = new Squirt(p1->getX(), p1->getY() + 4, this, player);
				goodies.push_back(g);
			}
		}
		break;
	case down:
		if (p1->getY() >= 4)
		{
			bool flag = false;
			for (int i = 0; i < 4; i++)
			{
				if (checkDOWN(p1->getX(), p1->getY() - i) != true) {
					flag = false;
					break;
				}
			}
			if (flag == true) {
				g = new Squirt(p1->getX(), p1->getY() - 4, this, player);
				goodies.push_back(g);
			}
		}
		break;
	case left:
		if (p1->getX() >= 4)
		{
			bool flag = false;
			for (int i = 0; i < 4; i++)
			{
				if (checkLEFT(p1->getX() - i, p1->getY()) != true) {
					flag = false;
					break;
				}
			}
			if (flag == true) {
				g = new Squirt(p1->getX() - 4, p1->getY(), this, player);
				goodies.push_back(g);
			}
		}
		break;
	case right:
		if (p1->getX() < 57)
		{
			bool flag = false;
			for (int i = 0; i < 4; i++)
			{
				if (checkRIGHT(p1->getX() + i, p1->getY()) != true) {
					flag = false;
					break;
				}
			}
			if (flag == true) {
				g = new Squirt(p1->getX() + 4, p1->getY(), this, player);
				goodies.push_back(g);
			}
		}
		break;
	}
	playSound(SOUND_PLAYER_SQUIRT);


}