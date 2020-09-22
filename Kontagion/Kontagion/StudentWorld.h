/*
Project3 UCLA CS32
© 02/27/2020 by @zihaoDONG
ALL RIGHTS RESERVED
File: StudentWorld.h
** Files Without This Note Are Provided By David Smallberg, Senior Lecturer @ UCLA **
*/

#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    void addSpray();
    void addFlame();
    void addRegular(double x, double y);
    void addAggressive(double x, double y);
    void addEcoli(double x, double y);
    void addFood(double x, double y);
    void deleteOverLap(Projectile* a);
    void updateScore() {
        std::ostringstream oss;
        oss << "Score:  ";
        if (getScore() < 0) {
            oss << "-";
            oss.fill('0');
            oss << std::setw(5) << -1*getScore();
        }
        else {
            oss.fill('0');
            oss << std::setw(6) << getScore();
        }
        oss << "  Level:  " << getLevel() << "  ";
        oss << "Lives:  "<<getLives() << " ";
        oss << "health:  " << m_s->getHealth() << "  ";
        oss << "Sprays:  " << m_s->getSpray() << "  ";
        //oss << "BacLeft:  " << m_totalBac;
        oss << "Flames:  " << m_s->getFlame();
        std::string s = oss.str();
        setGameStatText(s);
    }
    void eatGoodie(Goodies* a);
    bool overLapSocrates(bacteria* b);
    bool overLapWithFood(Actor* a);
    void eatFood(bacteria* b);
    bool movement(double xC, double yC);
    Direction findClosestFood(bacteria* b);
    void increTotalBac() { m_totalBac++; }
    void eatHealth() { m_s->restoreHealth(); }
    void eatFlame() { m_s->increFlame(); }
    void eatLife() { incLives(); }
    void eatFungus() { 
        m_s->takeDamage(20);
        if (m_s->getHealth() <= 0)
            m_s->setDead();
    }
    void getM_sPos(double& x, double& y) const {
        x = m_s->getX();
        y = m_s->getY();
    }
private:
    std::vector<Actor*> m_a;
    Socrates* m_s;
    int m_L;
    int m_totalBac;
};

#endif // STUDENTWORLD_H_
