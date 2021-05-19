#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

BaseObject::BaseObject(int imageID, int startX, int startY, Direction startDirection, float size, unsigned int depth):
	GraphObject(imageID, startX, startY, startDirection, size, depth) {
	setVisible(true);
}

Actor::Actor(int imageID, int startX, int startY, Direction startDirection, float size, unsigned int depth, StudentWorld* w, int hp)
	: BaseObject(imageID, startX, startY, startDirection, size, depth)
{
	num_hp = hp;
	world = w;
	status_alive = true;
}
Actor::~Actor() {

}
StudentWorld* Actor::getWorld() {
	return world;
}

bool Actor::isAlive() {
	return status_alive;
}

void Actor::isDead() {
	status_alive = false;
}

void Actor::irritated(int dmg) {
	num_hp -= dmg;
}

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
				if (getDirection() == left && getX() == 0)
					moveTo(getX(), getY());
				if (getDirection() == left) {
					getWorld()->removeIce(this);
					moveTo(getX() - 1, getY());
				}
				else
					setDirection(left);
				break;
			case KEY_PRESS_RIGHT:
				if (getDirection() == right && getX() == 60)
					moveTo(getX(), getY());
				if (getDirection() == right) {
					getWorld()->removeIce(this);
					moveTo(getX() + 1, getY());
				}
				else
					setDirection(right);
				break;
			case KEY_PRESS_UP:
				if (getDirection() == up && getY() == 60)
					moveTo(getX(), getY());
				if (getDirection() == up) {
					getWorld()->removeIce(this);
					moveTo(getX(), getY() + 1);
				}
				else
					setDirection(up);
				break;
			case KEY_PRESS_DOWN:
				if (getDirection() == down && getY() == 0)
					moveTo(getX(), getY());
				if (getDirection() == down) {
					getWorld()->removeIce(this);
					moveTo(getX(), getY() - 1);
				}
				else
					setDirection(down);
				break;
			}
		}
	}
}

Ice::Ice(int x, int y)
	:BaseObject(IID_ICE, x, y, right, .25, 3) {

}

