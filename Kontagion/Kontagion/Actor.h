/*
Project3 UCLA CS32
© 02/27/2020 by @zihaoDONG
ALL RIGHTS RESERVED
File: Actor.h
** Files Without This Note Are Provided By David Smallberg, Senior Lecturer @ UCLA **
*/

#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
//#include "GameConstants.h"
//#include "StudentWorld.h"
#include <cstdlib>
#include <string>


// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;

class Actor :public GraphObject {
public:
	Actor(int health,StudentWorld* studentWorld, int imageID, double startX, double startY, Direction dir = 0, int depth = 0, double size = 1.0)
		:GraphObject(imageID, startX, startY, dir, depth, size), m_studentWorld(studentWorld), m_alive(true) {
		m_health = health;
	}
	virtual ~Actor() {}
	virtual void doSomething() = 0;
	StudentWorld* getMyStudentWorld() const { return m_studentWorld; }
	bool isAlive() const { return m_alive; }
	void setDead() { m_alive = false; }
	virtual bool isFood() const {
		return false;
	}
	virtual bool isDamageable() const {
		return false;
	}
	virtual bool isDirt() const {
		return false;
	}
	virtual bool isBacteria() const {
		return false;
	}
	int getHealth() const { return m_health; }
	void takeDamage(int amount) { m_health -= amount; }
	virtual int getDamage() const { return 0; }
	void restoreHealth() { m_health = 100; }
	virtual void dying() const {}
	virtual void hurting() const {}
	virtual int worth() const { return 0; }
private:
	StudentWorld* m_studentWorld;
	bool m_alive;
	int m_health;
};

class Socrates :public Actor {
public:
	Socrates(StudentWorld* studentWorld,double startX, double startY)
		:Actor(100,studentWorld, IID_PLAYER, startX, startY, 0, 0, 1) {
		m_spray = 20;
		m_flame = 5;
		m_directionAngle = 180;
	}
	virtual ~Socrates() {}
	virtual void doSomething();
	int getSpray() const { return m_spray; }
	int getFlame() const { return m_flame; }
	void increFlame() { m_flame+=5; }
	void increSpray() { m_spray++; }
private:
	int m_spray;
	int m_flame;
	int m_directionAngle;
};

class Damageable :public Actor {
public:
	Damageable(int health,StudentWorld* studentWorld, int imageID, double startX, double startY, Direction dir, int depth, double size)
		:Actor(health,studentWorld, imageID, startX, startY, dir, depth, size) {}
	virtual ~Damageable() {}
	virtual bool isDamageable() const {
		return true;
	}
};

class Dirt_Pile :public Damageable {   //remember this should be a Damageable object
public:
	Dirt_Pile(StudentWorld* studentWorld, double startX, double startY)
		:Damageable(-1,studentWorld, IID_DIRT, startX, startY, 0, 1, 1) {}
	virtual ~Dirt_Pile() {}
	virtual void doSomething();
	virtual bool isDirt() const {
		return true;
	}
};
class Projectile :public Actor {
public:
	Projectile(StudentWorld* studentWorld,int imageID, double startX, double startY, Direction dir)
		:Actor(-1,studentWorld, imageID, startX, startY, dir, 1, 1) {
		m_disTraveled = 0;
		m_angle = dir;
	}
	virtual ~Projectile() {}
	virtual int getMax() const = 0;
	virtual void doSomething() ;
	virtual int Power() const = 0;
private:
	int m_disTraveled;
	int m_angle;
};

class Flame :public Projectile {
public:
	Flame(StudentWorld* studentWorld,double startX, double startY, Direction dir)
		:Projectile(studentWorld, IID_FLAME, startX, startY, dir) {}
	virtual ~Flame() {}
	virtual int getMax() const  { return 32; }
	virtual int Power() const { return 10; }
private:
	
};

class Spray :public Projectile {
public:
	Spray(StudentWorld* studentWorld,double startX, double startY, Direction dir)
		:Projectile(studentWorld, IID_SPRAY, startX, startY, dir) {}
	virtual ~Spray() {}
	virtual int getMax() const { return 112; }
	virtual int Power() const { return 2; }
private:
	
};

class Goodies :public Damageable {
public:
	Goodies(int lifetime,StudentWorld* studentWorld, int imageID, double startX, double startY)
		:Damageable(-1,studentWorld, imageID, startX, startY, 0,1,1) {
		m_lifetime = lifetime;
	}
	virtual ~Goodies() {}
	virtual void doSomething();
	virtual void decLifetime() { m_lifetime--; }
	virtual int getLifetime() const { return m_lifetime; }
	virtual void effect() = 0;
private:
	double m_lifetime;
};

class Restore_Health_Goodie :public Goodies {
public:
	Restore_Health_Goodie(int lifetime,StudentWorld* studentWorld,double startX, double startY)
		:Goodies(lifetime,studentWorld, IID_RESTORE_HEALTH_GOODIE, startX, startY) {}
	virtual ~Restore_Health_Goodie() {}
	virtual void effect();
	virtual int worth() const {
		return 250;
	}
};

class Flame_Thrower_Goodie :public Goodies {
public:
	Flame_Thrower_Goodie(int lifetime,StudentWorld* studentWorld,double startX, double startY)
		:Goodies(lifetime,studentWorld, IID_FLAME_THROWER_GOODIE, startX, startY) {}
	virtual ~Flame_Thrower_Goodie() {}
	virtual void effect();
	virtual int worth() const {
		return 300;
	}
};

class Extra_Life_Goodie :public Goodies {
public:
	Extra_Life_Goodie(int lifetime,StudentWorld* studentWorld,  double startX, double startY)
		:Goodies(lifetime,studentWorld, IID_EXTRA_LIFE_GOODIE, startX, startY) {}
	virtual ~Extra_Life_Goodie() {}
	virtual void effect();
	virtual int worth() const {
		return 500;
	}
};

class Fungus :public Goodies {
public:
	Fungus(int lifetime, StudentWorld* studentWorld,  double startX, double startY)
		:Goodies(lifetime, studentWorld, IID_FUNGUS, startX, startY) {}
	virtual ~Fungus() {}
	virtual void effect();
	virtual int worth() const {
		return -50;
	}
};

class Food :public Actor {
public:
	Food(StudentWorld* studentWorld, double startX, double startY)
		:Actor(-1,studentWorld, IID_FOOD, startX, startY, 90, 1, 1.0) {}
	virtual ~Food() {}
	virtual void doSomething() {}
	virtual bool isFood() const {
		return true;
	}
};

class Pit :public Actor {
public:
	Pit(StudentWorld* studentWorld, double startX, double startY)
		:Actor(-1,studentWorld, IID_PIT, startX, startY, 0, 1, 1) {
		m_R = 5;
		m_A = 3;
		m_E = 2;

	}
	virtual ~Pit() {}
	void emitR() { m_R--; }
	void emitA() { m_A--; }
	void emitE() { m_E--; }
	virtual void doSomething();
private:
	int m_R;
	int m_A;
	int m_E;
	//void generateNewPos(Pit* p, double& x, double& y);
};

class bacteria :public Damageable {
public:
	bacteria(int health, StudentWorld* studentWorld, int imageID, double startX, double startY)
		:Damageable(health,studentWorld, imageID, startX, startY, 90, 0, 1) {
		m_movePlan = 0;
		m_foodEaten = 0;
	}
	virtual ~bacteria() {}
	void doSomething() ;
	int getFoodEaten() const { return m_foodEaten; }
	int getMovePlan() const { return m_movePlan; }
	void setFood(int x) { m_foodEaten = x; }
	void setMovePlan(int x) { m_movePlan = x; }
	virtual int getDamage() const = 0;
	virtual bool isBacteria() const {
		return true;
	}
	virtual void dying() const;
	virtual void hurting() const;
	bool moveOrBlock(int units);
	void goToFood();
	void randomDir();
	void pointTowardS();
	virtual int worth() const { return 100; }
	virtual int towardLim() const = 0;
private:
	void generateNewCoor(bacteria* a, double& xC, double& yC);
	double distToS(bacteria* a);
	int m_movePlan;
	int m_foodEaten;
};

class regular :public bacteria {
public:
	regular(StudentWorld* studentWorld, double startX, double startY)
		:bacteria(4, studentWorld, IID_SALMONELLA, startX, startY) {}
	virtual ~regular() {}
	virtual int getDamage() const { return 1; }
	virtual int towardLim() const { return -1; }
};

class Aggressive :public bacteria {
public:
	Aggressive(StudentWorld* studentWorld,double startX, double startY)
		:bacteria(10, studentWorld, IID_SALMONELLA, startX, startY) {}
	virtual ~Aggressive() {}
	virtual int getDamage() const { return 2; }
	virtual int towardLim() const { return 72; }
};

class Ecoli :public bacteria {
public:
	Ecoli(StudentWorld* studentWorld, double startX, double startY)
		:bacteria(5, studentWorld, IID_ECOLI, startX, startY) {}
	virtual ~Ecoli(){}
	virtual int getDamage() const { return 4; }
	virtual void dying() const;
	virtual void hurting() const;
	virtual int towardLim() const { return 256; }
};
#endif // ACTOR_H_
