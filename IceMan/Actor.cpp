#include "Actor.h"
#include "StudentWorld.h"
#include <cmath>

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp
struct Position {
	int x;
	int y;
	Position(int x, int y)
	{
		this->x = x;
		this->y = y;
	}

};

BaseObject::BaseObject(int imageID, int startX, int startY, Direction startDirection, float size, unsigned int depth):
	GraphObject(imageID, startX, startY, startDirection, size, depth) {
	setVisible(true);
	status = true;
}

bool BaseObject::isAlive() {
	return status;
}

void BaseObject::isDead() {
	status = false;
}
/////////// ACTOR IMPLEMENTATION /////////////

Actor::Actor(int imageID, int startX, int startY, Direction startDirection, float size, unsigned int depth, StudentWorld* w, int hp)
	: BaseObject(imageID, startX, startY, startDirection, size, depth)
{
	num_hp = hp;
	world = w;
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
					getWorld()->removeIce(this);
					if (getX() == 0)
						moveTo(getX(), getY());
					else{
						moveTo(getX() - 1, getY());
					}
				}
				else
					setDirection(left);
				break;
			case KEY_PRESS_RIGHT:
				if (getDirection() == right)
				{
					getWorld()->removeIce(this);
					if (getX() == 60)
						moveTo(getX(), getY());
					else{
						moveTo(getX() + 1, getY());
					}
				}
				else
					setDirection(right);
				break;
			case KEY_PRESS_UP:
				if (getDirection() == up)
				{
					getWorld()->removeIce(this);
					if (getY() == 60)
						moveTo(getX(), getY());
					else{	
						moveTo(getX(), getY() + 1);
					}
				}
				else
					setDirection(up);
				break;
			case KEY_PRESS_DOWN:
				if (getDirection() == down)
				{
					getWorld()->removeIce(this);
					if (getY() == 0)
						moveTo(getX(), getY());
					else{
						moveTo(getX(), getY() - 1);
					}
				}
				else
					setDirection(down);
				break;
			case KEY_PRESS_ESCAPE:
				isDead();
				break;
			case KEY_PRESS_SPACE:
				if (num_water>0)
				{
					getWorld()->shoot(this);
					num_water--;
				}
				break;
			case 'z'||'Z':
				//sonar charge
				num_sonarCharge--;
				break;
			case KEY_PRESS_TAB:
				//gold nugget
				num_goldNugs--;
				break;
			}
		}
	}
}

void Iceman::annoy(int dmg) {
	int hp = getNum_hp();
	hp -= dmg;
	if (hp <= 0) {
		getWorld()->playSound(SOUND_PLAYER_GIVE_UP);
		getWorld()->getPlayer()->isDead();
	}
}

/////////// ICE IMPLEMENTATION /////////////

Ice::Ice(int x, int y)
	:BaseObject(IID_ICE, x, y, right, .25, 3) {

}

/////////// GOODIE IMPLEMENTATION /////////////
Goodie::Goodie(int imageID, int startX, int startY, Direction startDirection, float size, unsigned int depth, StudentWorld* w) 
	:Actor(imageID, startX, startY, startDirection, size, depth, w, 100)
{
	
}

void Goodie::doSomething() {
	return;
}

void Goodie::annoy(int dmg) {
	return;
}

Goodie::~Goodie() {

}
/////////// BOULDER IMPLEMENTATION /////////////

Boulder::Boulder(int x, int y, StudentWorld* w)
	:Goodie(IID_BOULDER, x, y, down, 1.0, 1, w) {
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
				if (getWorld()->withinDistance(getX(), getY(), 3.0)) {
					isDead();
					getWorld()->getPlayer()->annoy(100);
				}
			}
			else {
				isDead();
			}
			break;
		}
	}

}

Boulder::~Boulder() {}

/////////// SQUIRT IMPLEMENTATION /////////////

Squirt::Squirt(int x, int y, StudentWorld* w, Iceman* p1)
	:Goodie(IID_WATER_SPURT, x, y, p1->getDirection(), 1.0, 1, w)
{
	travelDistance = 4;
}

void Squirt::doSomething() {
	if (isAlive() != true) {
		return;
	}
	// implement with protestor
	
	switch (getDirection()) {
	case up:
		if (getWorld()->checkUP(getX(), getY())) {
			moveTo(getX(), getY() + 1);
		}
		else {
			isDead();
		}
		break;
	case down:
		if (getWorld()->checkDOWN(getX(), getY())) {
			moveTo(getX(), getY() - 1);
		}
		else {
			isDead();
		}
		break;
	case left:
		if (getWorld()->checkLEFT(getX(), getY())) {
			moveTo(getX() - 1, getY());
		}
		else {
			isDead();
		}
		break;
	case right:
		if (getWorld()->checkRIGHT(getX(), getY())) {
			moveTo(getX() + 1, getY());
		}
		else {
			isDead();
		}
		break;
	}
	if (travelDistance > 0) {
		travelDistance--;
	}
	else {
		isDead();
		return;
	}
}

Squirt::~Squirt(){}

//Protestor::Protestor(StudentWorld* w)
//	:Actor(IID_PROTESTER, 60, 60, left, 1, 0, w, 5) {
//	numSquaresToMoveInCurrentDirection = rand() % 53 + 8;
//	maxMoves = 3 - (getWorld()->getLevel() / 4);
//	ticksToWaitBetweenMoves = std::max(0, maxMoves);
//
//}
//
//void Protestor::doSomething() {
//	if (isAlive() == false) {
//		setVisible(false);
//		return;
//	}
//	else {
//		if (check_if_rest() != 0 ) {
//			ticksToWaitBetweenMoves--;
//			return;
//		}
//		else if(getNum_hp() <=0) {
//			leaveIceField();
//		}
//		else if (distance(getX(), getY(), getWorld()->getPlayer()->getX(), getWorld()->getPlayer()->getY() <= 4)) {
//
//		}
//
//	}
//
//}
//
//int Protestor::check_if_rest() {
//	if (ticksToWaitBetweenMoves != 0) {
//		return 1;
//	}
//	else {
//		return 0;
//	}
//}
//
//void Protestor::leaveIceField() {
//	if (getX() == 60 && getY() == 60) {
//		isDead();
//		return;
//	}
//	else {
//		Position* pos = leave.top();
//		leave.pop();
//		if (pos->x < getX()) {
//			setDirection(left);
//		}
//		else if (pos->x > getX()) {
//			setDirection(right);
//		}
//		else if (pos->y < getY()) {
//			setDirection(down);
//		}
//		else if (pos->y > getY()) {
//			setDirection(up);
//		}
//		moveTo(pos->x, pos->y);
//		delete pos;
//	}
//
//}
//
//void Protestor::shout() {
//
//}
//
//Protestor::~Protestor() {
//
//}