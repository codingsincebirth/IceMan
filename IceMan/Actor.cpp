#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

Object::Object(int imageID, int startX, int startY, Direction startDirection, float size, unsigned int depth):
	GraphObject(imageID, startX, startY, startDirection, size, depth) {
	setVisible(true);
}

Actor::Actor(int imageID, int startX, int startY, Direction startDirection, float size, unsigned int depth, StudentWorld* w, int hp)
	:Object(imageID, startX, startY, startDirection, size, depth)
{
	num_hp = hp;
	world = w;
	status_alive = true;
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
				if (getDirection() == left)
					moveTo(getX() - 1, getY());
				else
					setDirection(left);
				break;
			case KEY_PRESS_RIGHT:
				if (getDirection() == left && getX() == 60)
					moveTo(getX(), getY());
				if (getDirection() == right)
					moveTo(getX() + 1, getY());
				else
					setDirection(right);
				break;
			case KEY_PRESS_UP:
				if (getDirection() == up && getY() == 60)
					moveTo(getX(), getY());
				if (getDirection() == up)
					moveTo(getX(), getY() + 1);
				else
					setDirection(up);
				break;
			case KEY_PRESS_DOWN:
				if (getDirection() == left && getY() == 0)
					moveTo(getX(), getY());
				if (getDirection() == down)
					moveTo(getX(), getY() - 1);
				else
					setDirection(down);
				break;
			}
		}
	}
}

Ice::Ice(int x, int y)
	:Object(IID_ICE, x, y, right, .25, 3) {

}

void Ice::doSomething() {
	return;
}