/*
Project3 UCLA CS32
© 02/27/2020 by @zihaoDONG
ALL RIGHTS RESERVED
File: StudentWorld.cpp
** Files Without This Note Are Provided By David Smallberg, Senior Lecturer @ UCLA **
*/

#include "StudentWorld.h"
#include "GameConstants.h"
#include <iostream>
#include <string>
#include <cmath>
using namespace std;

double getDist(Actor* a, Actor* b);
bool block(Actor* a, double xC, double yC);
bool overLap(Actor* a, Actor* b);

double getDist(Actor* a, Actor* b) {
    double x1 = a->getX();
    double x2 = b->getX();
    double delX = x2 - x1;
    double y1 = a->getY();
    double y2 = b->getY();
    double delY = y2 - y1;
    double r = sqrt(delX * delX + delY * delY);
    return r;
}

bool block(Actor* a, double xC, double yC) {
    double x = a->getX();
    double y = a->getY();
    double delX = xC - x;
    double delY = yC - y;
    double r = sqrt(delX * delX + delY * delY);
    if (r <= SPRITE_RADIUS) {
        return true;
    }
    else {
        return false;
    }
}

bool overLap(Actor* a, Actor* b) {
    double r = getDist(a, b);
    if (r <= 2 * SPRITE_RADIUS)
        return true;
    else
        return false;
}

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    m_L = 1;
    m_totalBac = 10 * getLevel();
}

StudentWorld::~StudentWorld() {
    cleanUp();
}

int StudentWorld::init()
{
    m_totalBac = 10 * getLevel();
    m_s = new Socrates(this, 0, VIEW_HEIGHT / 2);
    int Dirt = max(180-20*getLevel(),20);
    int food = min(5 * getLevel(), 25);
    int pit = getLevel();
    int countDirt = 0;
    int countFood = 0;
    int countPit = 0;
    while (countPit < pit) {
        int u = randInt(0, 120) + randInt(0, 120);
        int r;
        if (u >= 120)
            r = 2 * 120 - u;
        else
            r = u;
        int angle = randInt(0, 360);
        int x = VIEW_WIDTH / 2 + r * cos(angle * (4 * atan(1)) / 180);
        //int y = VIEW_WIDTH / 2 + r * sin(angle * (4 * atan(1)) / 180);
        int y = VIEW_WIDTH / 2 + r * sin(angle * (4 * atan(1)) / 180);
        vector<Actor*>::iterator p = m_a.begin();
        bool indi = true;
        while (p != m_a.end()) {
            double delX = (*p)->getX() - x;
            double delY = (*p)->getY() - y;
            if (sqrt(delX*delX+delY*delY)<=2*SPRITE_RADIUS ) {
                indi = false;
                break;
            }
            else
                p++;
        }
        if (indi) {
            Pit* pi = new Pit(this, x, y);
            m_a.push_back(pi);
            countPit++;
        }
    }
    while (countFood < food) {
        int u = randInt(0, 120) + randInt(0, 120);
        int r;
        if (u >= 120)
            r = 2 * 120 - u;
        else
            r = u;
        int angle = randInt(0, 360);
        int x = VIEW_WIDTH / 2 + r * cos(angle * (4 * atan(1)) / 180);
        int y = VIEW_WIDTH / 2 + r * sin(angle * (4 * atan(1)) / 180);
        vector<Actor*>::iterator p = m_a.begin();
        
        bool indi = true;
        while (p != m_a.end()) {
            double delX = (*p)->getX() - x;
            double delY = (*p)->getY() - y;
            if (sqrt(delX * delX + delY * delY) <= 2 * SPRITE_RADIUS) {
                indi = false;
                break;
            }
            else
                p++;
        }
        if (indi) {
            Food* f = new Food(this, x, y);
            m_a.push_back(f);
            countFood++;
        }
    }
    while (countDirt<Dirt) {
        int u = randInt(0, 120) + randInt(0, 120);
        int r;
        if (u >= 120)
            r = 2 * 120 - u;
        else
            r = u;
        int angle = randInt(0, 360);
        int x = VIEW_WIDTH / 2 + r * cos(angle * (4 * atan(1)) / 180);
        int y = VIEW_WIDTH / 2 + r * sin(angle * (4 * atan(1)) / 180);
        vector<Actor*>::iterator p = m_a.begin();
        
        bool indi = true;
        while (p != m_a.end()) {
            double delX = (*p)->getX() - x;
            double delY = (*p)->getY() - y;
            if (sqrt(delX * delX + delY * delY) <= 2 * SPRITE_RADIUS && (!(*p)->isDirt())) {
                indi = false;
                break;
            }
            else
                p++;
        }
        if (indi) {
            Dirt_Pile* d = new Dirt_Pile(this, x, y);
            m_a.push_back(d);
            countDirt++;
        }
    }
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    /*if (!m_s->isAlive()) {
        cleanUp();
        playSound(SOUND_PLAYER_DIE);
        return GWSTATUS_PLAYER_DIED;
    }
    else {*/
        int chanceFungus = min(510 - m_L * 10, 200);
        int chanceGoodie = min(510 - m_L * 10, 250);
        int fungus = randInt(0, chanceFungus - 1);
        int goodie = randInt(0, chanceGoodie - 1);
        if (fungus == 0) {
            int angle = randInt(0, 359);
            int x = VIEW_RADIUS + VIEW_RADIUS * cos(angle);
            int y = VIEW_RADIUS + VIEW_RADIUS * sin(angle);
            Fungus* f = new Fungus(max(rand() % (300 - 10 * m_L), 50), this, x, y);
            m_a.push_back(f);
        }
        if (goodie == 0) {
            int angle = randInt(0, 359);
            int x = VIEW_RADIUS + VIEW_RADIUS * cos(angle);
            int y = VIEW_RADIUS + VIEW_RADIUS * sin(angle);
            Restore_Health_Goodie* r;
            Flame_Thrower_Goodie* ff;
            Extra_Life_Goodie* e;
            int index = randInt(0, 9);
            switch (index) {
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
                r= new Restore_Health_Goodie(max(rand() % (300 - 10 * m_L), 50), this, x, y);
                m_a.push_back(r);
                break;
            case 6:
            case 7:
            case 8:
                ff= new Flame_Thrower_Goodie(max(rand() % (300 - 10 * m_L), 50), this, x, y);
                m_a.push_back(ff);
                break;
            case 9:
                e= new Extra_Life_Goodie(max(rand() % (300 - 10 * m_L), 50), this, x, y);
                m_a.push_back(e);
                break;
            }
        }
        m_s->doSomething();
        vector<Actor*>::iterator a = m_a.begin();
        while (a != m_a.end()) {
            if ((*a)->isAlive()) {
                (*a)->doSomething();
                a++;
            }
            else
                a++;
            /*else {
                if ((*a)->isBacteria())
                    m_totalBac--;
                delete *a;
                a = m_a.erase(a);
            }*/
        }
        vector<Actor*>::iterator b = m_a.begin();
        while (b != m_a.end()) {
            if (!(*b)->isAlive()) {
                if ((*b)->isBacteria())
                    m_totalBac--;
                delete *b;
                b = m_a.erase(b);
            }
            else {
                b++;
            }
        }
        //decLives();
        if (!m_s->isAlive()) {
            decLives();
            //cleanUp();
            //delete m_s;
            this->playSound(SOUND_PLAYER_DIE);
            return GWSTATUS_PLAYER_DIED;
        }
        updateScore();
        if (m_totalBac != 0)
            return GWSTATUS_CONTINUE_GAME;
        else {
            playSound(SOUND_FINISHED_LEVEL);
            return GWSTATUS_FINISHED_LEVEL;
        }
    //}
}

void StudentWorld::cleanUp()
{
    vector<Actor*>::iterator p = m_a.begin();
    while(p != m_a.end()) {
        if ((*p)->isAlive()) {
            delete* p;
            p = m_a.erase(p);
        }
        else {
            p++;
        }
        //p = m_a.erase(p);
    }
    delete m_s;
    m_s = nullptr;
    //delete m_s;
}

void StudentWorld::addSpray() {
    double x, y;
    m_s->getPositionInThisDirection(m_s->getDirection(), 2 * SPRITE_RADIUS, x, y);
    Spray* s = new Spray(this, x,y, m_s->getDirection());
    //s->moveAngle(m_s->getDirection(), 2 * SPRITE_RADIUS);
    m_a.push_back(s);
    deleteOverLap(s);
}

void StudentWorld::addFlame() {
    for (int i = 1;i < 17;i++)
    {
        Flame* f = new Flame(this, m_s->getX() + cos(m_s->getDirection() * (4 * atan(1)) / 180) * SPRITE_RADIUS,
            m_s->getY() + sin(m_s->getDirection() * (4 * atan(1)) / 180) * SPRITE_RADIUS, 0 + i * 22);
        //f->moveAngle(f->getDirection(), 2 * SPRITE_RADIUS);
        m_a.push_back(f);
        deleteOverLap(f);
    }
}

void StudentWorld::addRegular(double x, double y) {
    regular* r = new regular(this, x, y);
    m_a.push_back(r);
}

void StudentWorld::addAggressive(double x, double y) {
    Aggressive* a = new Aggressive(this, x, y);
    m_a.push_back(a);
}

void StudentWorld::addEcoli(double x, double y) {
    Ecoli* e = new Ecoli(this, x, y);
    m_a.push_back(e);
}

void StudentWorld::addFood(double x, double y) {
    Food* f = new Food(this, x, y);
    m_a.push_back(f);
}

void StudentWorld::deleteOverLap(Projectile* a) {
    std::vector<Actor*> ::iterator p = m_a.begin();
    while (p != m_a.end()) {
        if (overLap(a, *p) && (*p)->isDamageable()) {
            if ((*p)->isBacteria()) {
                (*p)->takeDamage(a->Power());
                a->setDead();
                if ((*p)->getHealth() <= 0) {
                    if ((*p)->getDamage()!=4) {
                        if (!overLapWithFood((*p))) {
                            int indi = randInt(0, 1);
                            if (indi == 0) {
                                addFood((*p)->getX(), (*p)->getY());
                            }
                        }
                    }
                    else {
                        if (!overLapWithFood((*p))) {
                            addFood((*p)->getX(), (*p)->getY());
                        }
                    }
                    (*p)->dying();
                    (*p)->setDead();
                    increaseScore((*p)->worth());
                    //m_totalBac--;
                }
                else {
                    (*p)->hurting();
                }
            }
            else {
                a->setDead();
                (*p)->setDead();
            }
            return;
        }
        else {
            p++;
        }
    }
}

void StudentWorld::eatGoodie(Goodies* a) {
    if (overLap(m_s, a))
    {
        a->effect();
        increaseScore(a->worth());
        if (a->worth() > 0) {
            playSound(SOUND_GOT_GOODIE);
        }
        else {
            playSound(SOUND_PLAYER_HURT);
        }
        a->setDead();
    }
    a->decLifetime();
    if (a->getLifetime()<0)
        a->setDead();
}

bool StudentWorld::overLapSocrates(bacteria* b) {
    if (m_s->isAlive()&&overLap(b, m_s)) {
        m_s->takeDamage(b->getDamage());
        if (m_s->getHealth() > 0) {
            playSound(SOUND_PLAYER_HURT);
        }
        else {
            m_s->setDead();
            playSound(SOUND_PLAYER_DIE);
        }
        return true;
    }
    else
        return false;
}

bool StudentWorld::overLapWithFood(Actor* a) {
    std::vector<Actor*>::iterator p = m_a.begin();
    while (p != m_a.end()) {
        if (overLap(a, (*p)) && (*p)->isFood() && (*p)->isAlive()) {
            return true;
        }
        else {
            p++;
        }
    }
    return false;
}

void StudentWorld::eatFood(bacteria* b) {
    std::vector<Actor*>::iterator p = m_a.begin();
    while (p != m_a.end()) {
        if (overLap(b, (*p)) && (*p)->isFood()&&(*p)->isAlive()) {
            (*p)->setDead();
            b->setFood(b->getFoodEaten() + 1);
            return;
        }
        else {
            p++;
        }
    }
}

bool StudentWorld::movement(double xC, double yC) {
    std::vector<Actor*>::iterator p = m_a.begin();
    while (p != m_a.end()) {
        if ((block((*p), xC, yC) && (*p)->isDirt() )|| sqrt((xC - 128) * (xC - 128) + (yC - 128) * (yC - 128)) > 128) {
            return false;
        }
        else
            p++;
    }
    return true;
}



Direction StudentWorld::findClosestFood(bacteria* b) {
    std::vector<Actor*>::iterator p = m_a.begin();
    std::vector<Actor*>::iterator minPtr;
    int minIndex = -1;
    int count = 0;
    double min = 128;
    while (p != m_a.end()) {
        double dist = getDist(b, (*p));
        if ((*p)->isFood() && dist <= min) {
            minIndex = count;
            minPtr = p;
            min = dist;
        }
        count++;
        p++;
    }
    if (minIndex == -1)
        return -1;      //return a nonsense direction if we cannot find food object within 128 pixels
    double delX, delY;
    delY = (*minPtr)->getY() - b->getY();
    delX = (*minPtr)->getX() - b->getX();
    double pi = 4 * atan(1);
    double tangent = delY / delX;
    double dir =static_cast<double>(atan(tangent)*180/pi);
    if (delX > 0 && delY > 0)
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
    return direction;
}