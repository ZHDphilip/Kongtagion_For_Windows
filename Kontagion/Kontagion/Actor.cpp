/*
Project3 UCLA CS32
© 02/27/2020 by @zihaoDONG
ALL RIGHTS RESERVED
File: Actor.cpp
** Files Without This Note Are Provided By David Smallberg, Senior Lecturer @ UCLA **
*/

#include "Actor.h"
#include <iostream>
#include <cmath>
#include "StudentWorld.h"
using namespace std;

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp



///////Socrates implementation///////////
void Socrates::doSomething() {
	if (getHealth() == 0)
		return;
	else {
		int ch;
		//cin.ignore(100, '\n');
		if (getMyStudentWorld()->getKey(ch))
		{
			const double Pi = 4 * atan(1);
			switch (ch) {
			case KEY_PRESS_LEFT:
				m_directionAngle += 5;
				moveTo(getX() + (cos(m_directionAngle * Pi / 180) - cos((m_directionAngle-5) * Pi / 180)) * VIEW_RADIUS,
					getY() + (sin((m_directionAngle) * Pi / 180) - sin((m_directionAngle-5) * Pi / 180)) * VIEW_RADIUS);
				//moveAngle(getDirection() + 90, (5/180.0*Pi)*VIEW_RADIUS);
				setDirection(getDirection() +5);
				break;
			case KEY_PRESS_RIGHT:
				m_directionAngle -= 5;
				moveTo(getX() + (cos(m_directionAngle * Pi / 180) - cos((m_directionAngle + 5) * Pi / 180)) * VIEW_RADIUS,
					getY() + (sin((m_directionAngle)*Pi / 180) - sin((m_directionAngle + 5) * Pi / 180)) * VIEW_RADIUS);
				//moveAngle(getDirection() + 90, (5/180.0*Pi)*VIEW_RADIUS);
				setDirection(getDirection() - 5);
				break;
			case KEY_PRESS_SPACE:
				if (m_spray > 0) {
					getMyStudentWorld()->addSpray();
					getMyStudentWorld()->playSound(SOUND_PLAYER_SPRAY);
					m_spray--;
				}
				break;
			case KEY_PRESS_ENTER:
				if (m_flame > 0) {
					getMyStudentWorld()->addFlame();
					getMyStudentWorld()->playSound(SOUND_PLAYER_FIRE);
					m_flame--;
				}
				break;
			default:
				if (m_spray < 20)
					increSpray();
				break;
			}
		}
		else {
			if (m_spray < 20)
				increSpray();
		}
	}
}

///////Dirt Pile Implementation////////////
void Dirt_Pile::doSomething() {

}

///////Projectile Implementation//////////
void Projectile::doSomething() {
	if (!isAlive())
		return;
	else {
		
		moveAngle(m_angle, 2 * SPRITE_RADIUS);
		m_disTraveled += 2 * SPRITE_RADIUS;
		getMyStudentWorld()->deleteOverLap(this);
		if (m_disTraveled >= getMax())
			setDead();
	}
}

///////Goodies Implementation////////////////
void Goodies::doSomething() {
	if (!isAlive())
		return;
	else {
		getMyStudentWorld()->eatGoodie(this);
	}
}

void Restore_Health_Goodie::effect() {
	getMyStudentWorld()->eatHealth();
}

void Flame_Thrower_Goodie::effect() {
	getMyStudentWorld()->eatFlame();
}

void Extra_Life_Goodie::effect() {
	getMyStudentWorld()->eatLife();
}

void Fungus::effect() {
	getMyStudentWorld()->eatFungus();
}

/////pit Implementation.///////////
void Pit::doSomething() {
	if (m_R == 0 && m_A == 0 && m_E == 0)
		setDead();
	else {
		//bool emit = false;
		//while (!emit) {
		int indi = randInt(1, 50);
		//bool indicator = false;
		//while (!indicator) {
			if (indi == 1) {
				bool indicator = false;
				while(!indicator){
				int x = randInt(1, 3);
				switch (x) {
				case 1:
					if (m_R <= 0)
						break;
					else {
						getMyStudentWorld()->addRegular(getX(), getY());
						getMyStudentWorld()->playSound(SOUND_BACTERIUM_BORN);
						m_R--;
						indicator = true;
						//emit = true;
						break;
					}
				case 2:
					if (m_A <= 0)
						break;
					else {
						getMyStudentWorld()->addAggressive(getX(), getY());
						getMyStudentWorld()->playSound(SOUND_BACTERIUM_BORN);
						//emit an Aggressive backerium
						m_A--;
						indicator = true;
						break;
					}
				case 3:
					if (m_E <= 0)
						break;
					else {
						getMyStudentWorld()->addEcoli(getX(), getY());
						getMyStudentWorld()->playSound(SOUND_BACTERIUM_BORN);
						m_E--;
						indicator = true;
						//emit = true;
						break;
					}
				}
			}
		}
	}
}
////bacteria implementation////////////////
void bacteria::dying() const {
	getMyStudentWorld()->playSound(SOUND_SALMONELLA_DIE);
}
void bacteria::hurting() const {
	getMyStudentWorld()->playSound(SOUND_SALMONELLA_HURT);
}

void bacteria::randomDir() {
	Direction dir = randInt(0, 359);
	setDirection(dir);
	setMovePlan(10);
	return;
}

bool bacteria::moveOrBlock(int units) {
	double xc, yc;
	getPositionInThisDirection(getDirection(), units, xc, yc);
	if (getMyStudentWorld()->movement(xc, yc)) {
		this->moveTo(xc, yc);
		return true;
	}
	else {
		return false;
	}
}

void bacteria::goToFood() {
	if (getMyStudentWorld()->findClosestFood(this) == -1) {
		randomDir();
	}
	else {
		setDirection(getMyStudentWorld()->findClosestFood(this));
		if (moveOrBlock(3)) {
			;
		}
		else {
			randomDir();
		}
		return;
	}
}

void bacteria::generateNewCoor(bacteria* a, double& xC, double& yC) {
	if (a->getX() <= VIEW_RADIUS)
		xC = a->getX() + SPRITE_RADIUS;
	else
		xC = a->getX() - SPRITE_RADIUS;
	if (a->getY() <= VIEW_RADIUS)
		yC = a->getY() + SPRITE_RADIUS;
	else
		yC = a->getY() - SPRITE_RADIUS;
}

void bacteria::pointTowardS() {
	double delX, delY, x, y;
	getMyStudentWorld()->getM_sPos(x, y);
	delY = y - getY();
	delX = x - getX();
	double pi = 4 * atan(1);
	double tangent = delY / delX;
	double dir = static_cast<double>(atan(tangent) * 180 / pi);
	if (delX > 0 && delY > 0)  //figure out the exact direction bacteria should point to because the atan function only returns angle between -pi/2 to pi/2
	{
		;
	}
	if (delX < 0 && delY>0)
		dir += 180;
	if (delX < 0 && delY < 0)
		dir += 180;
	if (delX > 0 && delY < 0)
		dir += 360;
	Direction direction = static_cast<int>(dir);
	setDirection(direction);
}

void Ecoli::dying() const {
	getMyStudentWorld()->playSound(SOUND_ECOLI_DIE);
}
void Ecoli::hurting() const {
	getMyStudentWorld()->playSound(SOUND_ECOLI_HURT);
}

/*void regular::doSomething() {
	if (!isAlive()) { //step 1
		return;
	}
	else {
		if (!getMyStudentWorld()->overLapSocrates(this)) { //do step 2
			//do step 3
			if (getFoodEaten() == 3) {
				double xC, yC;
				generateNewCoor(this, xC, yC);
				getMyStudentWorld()->addRegular(xC, yC);
				getMyStudentWorld()->increTotalBac();
				setFood(0);
			}
			else {
				//do step 4
				getMyStudentWorld()->eatFood(this);
			}
		}
		if(getMovePlan()>0) {
			//do step 5
			setMovePlan(getMovePlan() - 1);
			if (moveOrBlock()) {
				;
			}
			else {
				randomDir();
			}
		}
		else {
			//do step 6
			goToFood();
		}
	}
}*/

/*void Aggressive::doSomething() {
	if (!isAlive()) { //step 1
		return;
	}
	else {
		bool perfSixOrNot = true;
		if (distToS(this) <= 72) { //do step 2
			//TODO:make it points toward Socrates
			pointTowardS();
			if (moveOrBlock()) {
				;
			}
			perfSixOrNot = false;
		}
		if (getMyStudentWorld()->overLapSocrates(this)) {
			if (!perfSixOrNot)
				return;
		}//do step 3
			//do step 4
		else if (getFoodEaten() == 3) {
			double xC, yC;
			generateNewCoor(this, xC, yC);
			getMyStudentWorld()->addAggressive(xC, yC);
			getMyStudentWorld()->increTotalBac();
			setFood(0);
			if (!perfSixOrNot)
				return;
		}
		else {
			//do step 5
			getMyStudentWorld()->eatFood(this);
			if (!perfSixOrNot)
				return;
		}
		
		if (getMovePlan() > 0) {
			//do step 6
			setMovePlan(getMovePlan() - 1);
			if (moveOrBlock()) {
				;
			}
			else {
				randomDir();
			}
		}
		else {
			//do step 7
			goToFood();
		}
	}
}*/

void bacteria::doSomething() {
	if (!isAlive()) { //step 1
		return;
	}
	else {
		bool perfSixOrNot = true;
		if (getDamage()==2) {
			if (distToS(this) <= towardLim()) { //do step 2
				//TODO:make it points toward Socrates
				pointTowardS();
				if (moveOrBlock(3)) {
					;
				}
				perfSixOrNot = false;
			}
		}
		if (getMyStudentWorld()->overLapSocrates(this)) {
			if (!perfSixOrNot)
				return;
		}//do step 3
			//do step 4
		else if (getFoodEaten() == 3) {
			double xC, yC;
			generateNewCoor(this, xC, yC);
			getMyStudentWorld()->addAggressive(xC, yC);
			getMyStudentWorld()->increTotalBac();
			setFood(0);
			if (!perfSixOrNot)
				return;
		}
		else {
			//do step 5
			getMyStudentWorld()->eatFood(this);
			if (!perfSixOrNot)
				return;
		}
		if (getDamage()!=4&&perfSixOrNot) {
			if (getMovePlan() > 0) {
				//do step 6
				setMovePlan(getMovePlan() - 1);
				if (moveOrBlock(3)) {
					;
				}
				else {
					randomDir();
				}
			}
			else {
				//do step 7
				goToFood();
			}
		}
		else {
			if (distToS(this) <= towardLim()) {
				pointTowardS();
				for (int i = 0;i < 10;i++) {
					if (moveOrBlock(2)) {
						return;
					}
					else {
						setDirection((getDirection() + 10) % 360);
					}
				}
			}
		}
	}
}

double getDis(double x1, double y1, double x2, double y2) {
	double delX = x2 - x1;
	double delY = y2 - y1;
	double dis = sqrt(delX * delX + delY * delY);
	return dis;
}

double bacteria::distToS(bacteria* a) {
	double x, y;
	getMyStudentWorld()->getM_sPos(x, y);
	double xC, yC;
	xC = a->getX();
	yC = a->getY();
	return getDis(x, y, xC, yC);
}