#include "Actor.h"
#include "StudentWorld.h"
#include <cmath>

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp


/////////// ACTOR IMPLEMENTATION /////////////

Actor::Actor(int imageID, int startX, int startY, Direction startDirection, float size, unsigned int depth, StudentWorld* w, int hp)
	: GraphObject(imageID, startX, startY, startDirection, size, depth)
{
	num_hp = hp;
	world = w;
	status = true;
	setVisible(true);
}

bool Actor::isAlive() {
	return status;
}

void Actor::isDead() {
	status = false;
}

int Actor::getNum_hp() {
	return num_hp;
}

Actor::~Actor() {

}

StudentWorld* Actor::getWorld() {
	return world;
}
/////////// ICEMAN IMPLEMENTATION /////////////

Iceman::Iceman(StudentWorld* w)
	:Actor(IID_PLAYER, 30, 60, right, 1.0, 0, w, 10)
{
	this->num_water = 5;
	this->num_sonarCharge = 1;
	this->num_goldNugs = 0;
}

Iceman::~Iceman() {

}

void Iceman::doSomething() {
	if (isAlive() != true) {
		return;
	}
	else {
		int ch;
		if (getWorld()->getKey(ch) == true)
		{
			// user hit a key this tick! 
			switch (ch)
			{
			case KEY_PRESS_LEFT:
				if (getDirection() == left)
				{
					if (getX() == 0)
						moveTo(getX(), getY());
					else if (getWorld()->isBoulder(getX() - 1, getY(), 3.0) == false) {
						moveTo(getX() - 1, getY());
						getWorld()->removeIce(this);
					}
				}
				else
					setDirection(left);
				break;
			case KEY_PRESS_RIGHT:
				if (getDirection() == right)
				{
					if (getX() == 60)
						moveTo(getX(), getY());
					else if (getWorld()->isBoulder(getX() + 1, getY(), 3.0) == false){
						moveTo(getX() + 1, getY());
						getWorld()->removeIce(this);
					}
				}
				else
					setDirection(right);
				break;
			case KEY_PRESS_UP:
				if (getDirection() == up) {
					if (getY() == 60)
						moveTo(getX(), getY());
					else if(getWorld()->isBoulder(getX(), getY() + 1, 3.0) == false){	
						moveTo(getX(), getY() + 1);
						getWorld()->removeIce(this);
					}
				}
				else
					setDirection(up);
				break;
			case KEY_PRESS_DOWN:
				if (getDirection() == down) {
					
					if (getY() == 0)
						moveTo(getX(), getY());
					else if(getWorld()->isBoulder(getX(), getY() -1, 3.0) == false){
						moveTo(getX(), getY() - 1);
						getWorld()->removeIce(this);
					}
				}
				else
					setDirection(down);
				break;
			case KEY_PRESS_ESCAPE:
				isDead();
				break;
			case KEY_PRESS_SPACE:
				if (num_water>0) {
					getWorld()->shoot(this);
					num_water--;
				}
				break;
			case 'z':
			case 'Z':
				if (num_sonarCharge > 0) {
					getWorld()->activateSonar(getX(), getY());
					num_sonarCharge--;
				}
				break;
			case KEY_PRESS_TAB:
				if (num_goldNugs > 0) {
					getWorld()->dropNugget(this);
					num_goldNugs--;
				}
				break;
			}
		}
	}
}
void Iceman::inc_water() {
	num_water += 5;
}

void Iceman::inc_gold() {
	num_goldNugs += 1;
}

void Iceman::dec_gold() {
	num_goldNugs -= 1;
}

void Iceman::inc_sonar() {
	num_sonarCharge += 1;
}

void Iceman::annoy(int dmg) {
	int hp = getNum_hp();
	hp -= dmg;
	if (hp <= 0) {
		getWorld()->playSound(SOUND_PLAYER_GIVE_UP);
		getWorld()->getPlayer()->isDead();
	}
}

int Iceman::getWater() {
	return num_water;
}

int Iceman::getSonar() {
	return num_sonarCharge;
}

int Iceman::getGoldNugs() {
	return num_goldNugs;
}

/////////// ICE IMPLEMENTATION /////////////

Ice::Ice(int x, int y)
	:GraphObject(IID_ICE, x, y, right, .25, 3) {
	setVisible(true);
}



/////////// GOODIE IMPLEMENTATION /////////////
Goodie::Goodie(int imageID, int startX, int startY, Direction startDirection, float size, unsigned int depth, StudentWorld* w) 
	:Actor(imageID, startX, startY, startDirection, size, depth, w, 100)
{
	
}

void Goodie::doSomething() {
	return;
}

int Goodie::classType() {
	return 0;
}

void Goodie::annoy(int dmg) {
	return;
}

Goodie::~Goodie() {

}
/////////// BOULDER IMPLEMENTATION /////////////

Boulder::Boulder(int x, int y, StudentWorld* w)
	: Goodie(IID_BOULDER, x, y, down, 1.0, 1, w) {
	state = 0;
	waitingTicks = 30;
}

void Boulder::setState(int st) {
	state = st;
	
	// 0 = stable
	// 1 = waiting
	// 2 = falling
}

int Boulder::getState() {
	return state;
}

void Boulder::doSomething() {
	if (isAlive() == false)
		return;
	else {
		switch (getState()) {
		case 0:
			if (getWorld()->checkDOWN(getX(), getY())) {
				setState(1);
			}
			break;
		case 1:
			waitingTicks--;
			if (waitingTicks == 0) {
				setState(2);
				getWorld()->playSound(SOUND_FALLING_ROCK);
			}
			break;
		case 2:
			if (getWorld()->checkDOWN(getX(), getY()))
			{
				moveTo(getX(), getY() - 1);
				if (getWorld()->withinDistanceofPlayer(getX(), getY(), 3.0)) { // if the boulder runs into the player
					isDead();
					getWorld()->getPlayer()->annoy(100);
				}
				if (getWorld()->isBoulder(getX(), getY() - 1, 3.0)) { // if the boulder runs into another boulder
					isDead();
				}
			}
			else {
				isDead();
			}
			break;
		}
	}

}

int Boulder::classType() {
	return 1;
}

Boulder::~Boulder() {}

/////////// BARREL IMPLEMENTATION /////////////
Barrel::Barrel(int x, int y, StudentWorld* w)
	: Goodie(IID_BARREL, x, y, right, 1.0, 2, w) {
	setVisible(false);
}

void Barrel::doSomething() {
	if (isAlive() == false) {
		return;
	} 
	else if (isVisible() == false && getWorld()->withinDistanceofPlayer(getX(), getY(), 4.0)) {
		setVisible(true);
		return;
	} 
	else if (getWorld()->withinDistanceofPlayer(getX(), getY(), 3.0)) {
		isDead();
		getWorld()->playSound(SOUND_FOUND_OIL);
		getWorld()->increaseScore(1000);
		getWorld()->decBarrels();
		return;
	}
}

int Barrel::classType() {
	return 2;
}

Barrel::~Barrel() {}

/////////// SQUIRT IMPLEMENTATION /////////////

Squirt::Squirt(int x, int y, StudentWorld* w, Iceman* p1)
	:Goodie(IID_WATER_SPURT, x, y, p1->getDirection(), 1.0, 1, w)
{
	travelDistance = 4;
	player = p1;
}

void Squirt::doSomething() {
	if (isAlive() != true) {
		return;
	}
	// implement with protestor
	
	if (travelDistance > 0)
	{
		switch (getDirection()) {
		case up:
			if (getWorld()->checkUP(getX(), getY()) && !(getWorld()->isBoulder(getX(), getY(), 3.0))) {
				moveTo(getX(), getY() + 1);
			}
			else {
				isDead();
			}
			break;
		case down:
			if (getWorld()->checkDOWN(getX(), getY()) && !(getWorld()->isBoulder(getX(), getY(), 3.0))) {
				moveTo(getX(), getY() - 1);
			}
			else {
				isDead();
			}
			break;
		case left:
			if (getWorld()->checkLEFT(getX(), getY()) && !(getWorld()->isBoulder(getX(), getY(), 3.0))) {
				moveTo(getX() - 1, getY());
			}
			else {
				isDead();
			}
			break;
		case right:
			if (getWorld()->checkRIGHT(getX(), getY()) && !(getWorld()->isBoulder(getX(), getY(), 3.0))) {
				moveTo(getX() + 1, getY());
			}
			else {
				isDead();
			}
			break;
		}
		travelDistance--;
	}
	else {
		isDead();
		return;
	}
	
}

Squirt::~Squirt(){}

/////////// WATER POOL IMPLEMENTATION /////////////

Waterpool::Waterpool(int x, int y, StudentWorld* w)
	: Goodie(IID_WATER_POOL, x, y, right, 1.0, 2, w) {
	num_ticks = getWorld()->max(100, 300 - (10 * getWorld()->getLevel()));
}

void Waterpool::doSomething() {
	if (isAlive() != true) {
		return;
	}
	if (num_ticks > 0) {
		if (getWorld()->withinDistanceofPlayer(getX(), getY(), 3.0)) {
			isDead();
			getWorld()->playSound(SOUND_GOT_GOODIE);
			getWorld()->getPlayer()->inc_water(); // increase water count by 5
		}
	} else {
		isDead();
	}
	num_ticks--;
}
Waterpool::~Waterpool() {}

/////////// PERMANANT GOLD NUGGETS IMPLEMENTATION /////////////

Perm_Nuggets::Perm_Nuggets(int x, int y, StudentWorld *w)
	:Goodie(IID_GOLD, x, y, right, 1.0, 2, w) 
{
	setVisible(false);
}

void Perm_Nuggets::doSomething() {
	if (isAlive() != true) {
		return;
	}
	if (isVisible() == false && getWorld()->withinDistanceofPlayer(getX(), getY(), 4.0)) {
		setVisible(true);
		return;

	}
	else if (getWorld()->withinDistanceofPlayer(getX(), getY(), 3.0)) {
		isDead();
		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->increaseScore(25);
		getWorld()->getPlayer()->inc_gold(); // increase gold nuggets by 1

	}
}
Perm_Nuggets::~Perm_Nuggets() {

}

/////////// TEMPORARY GOLD NUGGETS IMPLEMENTATION /////////////

Temp_Nuggets::Temp_Nuggets(int x, int y, StudentWorld* w)
	: Goodie(IID_GOLD, x, y, right, 1.0, 2, w)
{
	num_ticks = getWorld()->max(100, 300 - (10 * getWorld()->getLevel()));
	setVisible(true);
}

void Temp_Nuggets::doSomething() {
	if (isAlive() != true) {
		return;
	}
	if (num_ticks > 0) {
		//if gold nugget is near protestor he picks it up
	}
	else {
		isDead();
		return;
	}
	num_ticks--;
}
Temp_Nuggets::~Temp_Nuggets(){}

/////////// SONAR KITS IMPLEMENTATION /////////////

Sonar::Sonar(int x, int y, StudentWorld* w)
	:Goodie(IID_SONAR, x, y, right, 1.0, 2, w) {
	num_ticks = getWorld()->max(100, 300 - (10 * getWorld()->getLevel()));
}

void Sonar::doSomething() {
	if (isAlive() != true) {
		return;
	} else if(num_ticks > 0) {
		if (getWorld()->withinDistanceofPlayer(getX(), getY(), 3.0)) {
			isDead();
			getWorld()->playSound(SOUND_GOT_GOODIE);
			getWorld()->increaseScore(75);
			getWorld()->getPlayer()->inc_sonar();
		}
		else {
			isDead();
		}
	}
	num_ticks--;
}

int Sonar::classType() {
	return 5;
}

Sonar::~Sonar() {

}

