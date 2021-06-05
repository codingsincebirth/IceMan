
#include "StudentWorld.h"
#include "Actor.h"
#include "GameConstants.h"
#include <string>
#include <cmath>
using namespace std;


GameWorld* createStudentWorld(string assetDir) {
	return new StudentWorld(assetDir);
}

int StudentWorld::init() {
	player = new Iceman(this); // Create new iceman
	for (int i = 0; i < VIEW_WIDTH; i++) {
		for (int j = 0; j < 60; j++) {
			if (i >= 30 && i <= 33 && j >= 4) { // Creating the tunnel
				ice[i][j] = nullptr;
			} else {
				ice[i][j] = new Ice(i, j);
			}
		}
	}
	for (int i = 0; i < 64; i++) { // Creating the surface
		for (int j = 60; j < 64; j++) {
			ice[i][j] = nullptr;
		}
	}
	// coords
	int x, y;
	// Boulders
	num_boulders = min((getLevel() / 2) + 2, 9);
	for (int i = 0; i < num_boulders; i++) {
		for (;;) {
			x = rand() % 61; // 0 - 60
			y = rand() % 37 + 20; // 20 - 56
			if (x < 27 || x > 33 && canDistribute(x, y)) { // Accounting for the tunnel from x = 30 to x = 33
				break;
			}
		}
		Goodie *g = new Boulder(x, y, this);
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				delete ice[g->getX() + j][g->getY() + k]; // clearing ice to place the boulders
				ice[g->getX() + j][g->getY() + k] = nullptr;
			}
		}
		goodies.push_back(g);
	}
	// Barrels
	num_barrels = min(2 + getLevel(), 21);
	for (int i = 0; i < num_barrels; i++) {
		for (;;) {
			x = rand() % 61;
			y = rand() % 57;
			if (y == 0 && canDistribute(x, y)) {
				break;
			}
			else if ((x < 26 || x > 33) && canDistribute(x, y)){
				break;
			}
		}
		goodies.push_back(new Barrel(x, y, this));
	}
	num_nuggets = max(5 - (getLevel() / 2), 2);
	for (int i = 0; i < num_nuggets; i++) {
		for (;;) {
			x = rand() % 61; // 0 - 60
			y = rand() % 57; // 0 - 56
			if (y == 0 && canDistribute(x, y)) {
				break;
			}
			else if ((x < 26 || x > 33) && canDistribute(x, y)){
				break;
			}
		}
		goodies.push_back(new Perm_Nuggets(x, y, this));
	}
	// Check if any barrels left
	return GWSTATUS_CONTINUE_GAME;
}
bool StudentWorld::canDistribute(int x, int y) {
	for (size_t i = 0; i < goodies.size(); i++) {
		if (distance(goodies[i]->getX() + 2, x + 2, goodies[i]->getY() + 2, y + 2) <= 6.0)
			return false;
	}
	return true;
}

int StudentWorld::move() {
	setGameStatText(overHeadText());
	player->doSomething();
	if (player->isAlive() != true) {
		decLives();
		return GWSTATUS_PLAYER_DIED;
	}
	for (size_t i = 0; i < goodies.size(); i++) {
		goodies[i]->doSomething();
	}
	for (size_t i = 0; i < goodies.size();) {
		if (goodies[i]->isAlive() != true) {
			delete goodies[i];
			goodies[i] = nullptr;
			goodies.erase(goodies.begin() + i);
		} 
		else {
			i++;
		}
	}
	m_goodie = (getLevel() * 25) + 300;
	if ((rand() % m_goodie) < 1){ // spawn sonar and water pools
		int chance = rand() % 5; // 0 - 4
		if (chance < 1 && !sonarExists()) { 
			goodies.push_back(new Sonar(0, 60, this));
		} else if (chance > 1) {
			int x = rand() % 61;
			int	y = rand() % 61;
			while (!checkForIce(x,y)) {
				x = rand() % 61;
				y = rand() % 61;
			}
			if (canDistribute(x, y)) {
				goodies.push_back(new Waterpool(x, y, this));
			}
		}
	}
	if (num_barrels <= 0) {
		playSound(SOUND_FINISHED_LEVEL);
		return GWSTATUS_FINISHED_LEVEL;
	} else if (player->isAlive()) {
		return GWSTATUS_CONTINUE_GAME;
	}
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp() {
	for (int i = 0; i < VIEW_WIDTH; i++) {
		for (int j = 0; j < 60; j++) {
			if (ice[i][j] != nullptr) {
				delete ice[i][j];
				ice[i][j] = nullptr;
			}
		}
	}
	for (size_t i = 0; i < goodies.size();) {
		delete goodies[i];
		goodies[i] = nullptr;
		goodies.erase(goodies.begin() + i);
	}
	delete player;
}

Iceman* StudentWorld::getPlayer() {
	return player;
}

void StudentWorld::removeIce(Iceman* p1) {
	
	for (int i = p1->getX(); i <= p1->getX() + 3; i++) {
		for (int j = p1->getY(); j <= p1->getY() + 3; j++) {
			if (ice[i][j] != nullptr && j < 60) {
				delete ice[i][j];
				ice[i][j] = nullptr;
				playSound(SOUND_DIG);
			}
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

double StudentWorld::distance(int x1, int x2, int y1, int y2) {
	return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

bool StudentWorld::withinDistanceofPlayer(int x, int y, double radius) {
	if (distance(x, player->getX(), y, player->getY()) <= radius) {
		return true;
	}
	else {
		return false;
	}
}

bool StudentWorld::checkUP(int x, int y) {
	bool flag = false;
	for (int i = x; i < x + 4; i++) {
		if (y < 60) {
			if (ice[i][y + 4] == nullptr)
				flag = true;
			else
				return false;
		}
		else {
			return false;
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
				return false;
		} 
		else {
			return false;
		}
	}
	return flag;
}

bool StudentWorld::checkLEFT(int x, int y) {
	bool flag = true;
	for (int j = y; j < y + 4; j++)
	{
		if (x > 0)
		{
			if (ice[x - 1][j] == nullptr)
				flag = true;
			else
				return false;
		}
		else {
			return false;
		}
	}
	return flag;
}

bool StudentWorld::checkRIGHT(int x, int y) {
	bool flag = false;
	for (int j = y; j < y + 4; j++) {
		if (x < 60)	{
			if (ice[x + 4][j] == nullptr) {
				flag = true;
			} else {
				return false;
			}
		} else {
			return false;
		}
	}
	return flag;
}

void StudentWorld::shoot(Iceman* p1) {
	switch (p1->getDirection()) {
	case up:
		if (p1->getY() <= 60) {
			bool flag = true;
			for (int i = 0; i < 4; i++) {
				if (!(checkUP(p1->getX(), p1->getY() + i)) && isBoulder(p1->getX(), p1->getY(), 3.0)) {
					flag = false;
					break;
				}
			}
			if (flag == true) {
				goodies.push_back(new Squirt(p1->getX(), p1->getY() + 4, this, p1));
			}
		}
		break;
	case down:
		if (p1->getY() >= 4) {
			bool flag = true;
			for (int i = 0; i < 4; i++) {
				if (!(checkDOWN(p1->getX(), p1->getY() - i)) && isBoulder(p1->getX(), p1->getY(), 3.0)) {
					flag = false;
					break;
				}
			}
			if (flag == true) {
				goodies.push_back(new Squirt(p1->getX(), p1->getY() - 4, this, p1));
			}
		}
		break;
	case left:
		if (p1->getX() >= 4) {
			bool flag = true;
			for (int i = 0; i < 4; i++) {
				if (!(checkLEFT(p1->getX() - i, p1->getY())) && isBoulder(p1->getX(), p1->getY(), 3.0)) {
					flag = false;
					break;
				}
			}
			if (flag == true) {
				goodies.push_back(new Squirt(p1->getX() - 4, p1->getY(), this, p1));
			}
		}
		break;
	case right:
		if (p1->getX() < 57) {
			bool flag = true;
			for (int i = 0; i < 4; i++) {
				if (!(checkRIGHT(p1->getX() + i, p1->getY())) && isBoulder(p1->getX(), p1->getY(), 3.0)) {
					flag = false;
					break;
				}
			}
			if (flag == true) {
				goodies.push_back(new Squirt(p1->getX() + 4, p1->getY(), this, p1));
			}
		}
		break;
	}
	playSound(SOUND_PLAYER_SQUIRT);
}

void StudentWorld::activateSonar(int x, int y) {
	for (size_t i = 0; i < goodies.size(); i++) {
		if (!goodies[i]->isVisible()) {
			if (distance(goodies[i]->getX(), x, goodies[i]->getY(), y) <= 12.0) {
				goodies[i]->setVisible(true);
			}
		}
	}
	playSound(SOUND_SONAR);
}

void StudentWorld::dropNugget(Iceman * p1) {
	goodies.push_back(new Temp_Nuggets(p1->getX(), p1->getY(), this));
	p1->dec_gold();
}

void StudentWorld::decBarrels() {
	num_barrels--;
}

bool StudentWorld::isBoulder(int x, int y, double radius) {
	for (size_t i = 0; i < goodies.size(); i++) {
		if (goodies[i]->classType() == 1) {
			if (distance(goodies[i]->getX(), x, goodies[i]->getY(), y) <= radius && distance(goodies[i]->getX(), x, goodies[i]->getY(), y) != 1) {
				return true;
			}
		}
	}
	return false;
}

bool StudentWorld::sonarExists() {
	for (size_t i = 0; i < goodies.size(); i++) {
		if (goodies[i]->classType() == 5) {
			return true;
		} else {
			return false;
		}
	}
	return false;
}

bool StudentWorld::checkForIce(int x, int y) {
	if (ice[x][y] == nullptr) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (ice[x + i][y + j] != nullptr) {
					return false;
				} else {
					return true;
				}
			}
		}
	} else {
		return false;
	}
	return false;
	
}

int StudentWorld::getBarrelsLeft() {
	return num_barrels;
}

// Lvl: 52 Lives: 3 Hlth: 80% Wtr: 20 
// Gld: 3 Oil Left: 2 Sonar: 1 Scr: 321000

std::string StudentWorld::overHeadText() {
	std::string level  = to_string(				getLevel() + 1);
	std::string lives  = to_string(				getLives());
	std::string health = to_string(getPlayer()->getNum_hp()*10);
	std::string water  = to_string(getPlayer()->getWater());
	std::string gold   = to_string(getPlayer()->getGoldNugs());
	std::string oil    = to_string(				getBarrelsLeft());
	std::string sonar  = to_string(				getPlayer()->getSonar());
	std::string score  = to_string(				getScore());
	while (water.size() < 2) {
		water = " " + water;
	}
	while (health.size() < 2) {
		health = " " + health;
	}
	while (score.size() < 6) {
		score = "0" + score;
	}
	return ("Lvl: " + level + 
			"  Lives: " + lives + 
			"  Hlth: " + health + 
			"%  Wtr: " + water + 
			"  Gld: " + gold + 
			"  Oil Left: " + oil + 
			"  Sonar: " + sonar + 
			"  Scr: " + score);
}
