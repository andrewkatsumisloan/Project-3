#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp


//GraphObject(int imageID, double startX, double startY, Direction dir = 0, int depth = 0, double size = 1.0)
//    : m_imageID(imageID), m_x(startX), m_y(startY), m_destX(startX), m_destY(startY),
//      m_animationNumber(0), m_direction(dir), m_depth(depth), m_size(size)


// Implemented isDamageableObject that should be set true for everything that Socrates is able to damage with either flames or spray.  This includes goodies, fungus, all types of bacteria, pits, dirt, etc but does NOT include Socrates or other Projectiles
class Actor:public GraphObject
{
public:
    Actor(int imageID, double startX, double startY,
        Direction dir, int depth, StudentWorld* sw,
        bool isplayer=false, bool isFood=false, bool blocker=false):
            GraphObject(imageID, startX, startY, dir, depth), m_alive(true),
            m_world(sw), m_isplayer(isplayer), m_isFood(isFood), m_blocker(blocker) {}
    virtual void doSomething() = 0;
    StudentWorld* getWorld()
    {
        return m_world;
    }
    bool isAlive()
    {
       return m_alive;
    }
    void kill()
    {
        m_alive = false;
    }
    bool isPlayer() const
    {
        return m_isplayer;
    }
    bool isFood()
    {
        return m_isFood;
    }
    bool blockBacteria()
    {
        return m_blocker;
    }
    virtual bool takeDamage(int x)
    {
        return false;
    }
    virtual ~Actor() {}

    // You may add other public/private member functions and private data members to your
    // dirt class as you see fit so long as you used good object oriented programming style
    // (no duplication of functionality across classes).
private:
    bool m_alive;
    bool m_isplayer;
    bool m_isFood;
    bool m_blocker;
    StudentWorld* m_world;
};

class LifeForm: public Actor
{
public:
    LifeForm(int imageID, double startX, double startY, Direction dir,
        int depth, StudentWorld* sw, bool isplayer, bool isFood, int HP):
        Actor(imageID, startX, startY, dir, depth, sw, isplayer, false, false), m_HP(HP) {}
     void decrementHP(int x)
      {
          m_HP = m_HP - x;
      }
    virtual void resetHP()
    {
        m_HP = 100;
    }
    virtual int getHP()
    {
        return m_HP;
    }

private:
    int m_HP;
    
    
};
class Socrates: public LifeForm
{
public:
    Socrates(StudentWorld* sw): LifeForm(IID_PLAYER, 0, 128, 0, 0, sw, true, false, 100),
        m_flameCharges(5), m_sprayCharges(20), m_angle(-90) {}
    void doSomething();
    bool takeDamage(int x);
    void addFlames()
    {
        m_flameCharges += 5;
    }
    int getFlames()
    {
        return m_flameCharges;
    }
    int getSprays()
    {
        return m_sprayCharges;
    }
    
private:
    int m_flameCharges;
    int m_sprayCharges;
    double m_angle;
};


// Either alive or fully dead
class Dirt: public Actor
{
public:
    Dirt(double startX, double startY, StudentWorld* sw):
        Actor(IID_DIRT, startX, startY, 0, 1, sw, false, false, true) {}
    void doSomething(){} // Is it sloppy to define doSomething for dirt here and other functions in the .cpp file?
    virtual bool takeDamage(int x)
    {
        this->kill();
        return true;
    }
};
    
class Disease: public LifeForm
{
public:
    Disease(int imageID, double startX, double startY, StudentWorld* sw, int HP);
    ~Disease();
    virtual void doSomething()=0;
    void increaseFC()
    {
        m_foodCount++;
    }
    int getFC()
    {
        return m_foodCount;
    }
    void resetFC()
    {
        m_foodCount = 0;
    }
    int getMP()
    {
        return m_movePlan;
    }
    void decrementMP(int x)
    {
        m_movePlan -= x;
    }
    void setMP(int x)
    {
        m_movePlan = x;
    }
    void spawnSite(int &x, int &y);
    virtual bool takeDamage(int x)
    {
        decrementHP(x);
        return true;
    }
    // takes the hit amount for the disease 
    // returns true if infection was successful
    bool infect(int severity);

private:
    int m_movePlan;
    int m_foodCount;
};


class Salmonella: public Disease
{
public:
    Salmonella(int id, double startX, double startY, StudentWorld* sw, int HP):
        Disease(id, startX, startY, sw, HP) {}
    virtual void doSomething() = 0;
    void planMotion();
    bool takeDamage(int x);
    
private:
    
};

class Sal: public Salmonella
{
public:
    Sal(double startX, double startY, StudentWorld* sw):
        Salmonella(IID_SALMONELLA, startX, startY, sw, 4) {}
    void spawnAndFeed();
    void doSomething();
};

class AggSal: public Salmonella
{
public:
    AggSal(double startX, double startY, StudentWorld* sw):
        Salmonella(IID_SALMONELLA, startX, startY, sw, 10) {}
    void spawnAndFeed();
    void doSomething();
};

class Ecoli: public Disease
{
public:
    Ecoli(double startX, double startY, StudentWorld* sw):
        Disease(IID_ECOLI, startX, startY, sw, 5) {}
    void spawnAndFeed();
    void doSomething();
    bool takeDamage(int x);
};

class Pit: public Actor
{
public:
    Pit(double startX, double startY, StudentWorld* sw);
    ~Pit();    
    void doSomething(); // not yet defined in .cpp
private:
    int numSal;
    int numAggSal;
    int numEcoli;
    
};
class Projectile: public Actor
{
public:
    Projectile(int imageID, double startX, double startY, double dir,
    int depth, StudentWorld* sw, bool isplayer,
    int maxtraveldist, int distancetrav):
        Actor(imageID, startX, startY, dir, depth, sw, false, false),
        m_maxtravelDist(maxtraveldist), m_distancetrav(distancetrav){}
    virtual void doSomething(){}
    virtual void incrementTravel()
    {
        if (m_distancetrav >= m_maxtravelDist)
        {
            this->kill();
        }
        m_distancetrav += SPRITE_WIDTH;
    }
private:
    int m_maxtravelDist;
    int m_distancetrav;
};
   
class Flame: public Projectile
{
public:
    Flame(double startX,double startY, double dir, StudentWorld* sw):
        Projectile(IID_FLAME, startX, startY, dir, 1, sw, false, 32, 0) {}
    void doSomething();

};
   
class Spray: public Projectile
{
public:
    Spray(double startX, double startY, double dir,
        StudentWorld* sw):
            Projectile(IID_SPRAY, startX, startY, dir, 1, sw, false, 112, 0) {}
    void doSomething();
};


class Food: public Actor
{
public:
    Food(double startX, double startY, StudentWorld* sw): Actor(IID_FOOD, startX, startY, 0, 1, sw, false, true) {}
    void doSomething(){};
    
};


class Goodie: public Actor
{
public:
    Goodie(int imageID, double startX, double startY, Direction dir, int depth, StudentWorld* sw, int lifetime): Actor(imageID, startX, startY, dir, depth, sw, false, false), m_lifetime(lifetime), m_age(0) {};
    void doSomething();
    void incrementAge()
    {
        m_age++;
    }
    int getAge()
    {
        return m_age;
    }
    int getLifetime()
    {
        return m_lifetime;
    }
    bool takeDamage(int x)
    {
        this->kill();
        return true;
    }
    virtual void pickUp( Socrates *soc) = 0;
private:
    int m_lifetime;
    int m_age;
};
    
class HealthGoodie: public Goodie
{
public:
    HealthGoodie(double startX, double startY, StudentWorld* sw, int lifetime): Goodie(IID_RESTORE_HEALTH_GOODIE, startX, startY, 0, 1, sw, lifetime) {}
    void doSomething();
    void pickUp(Socrates *soc);
private:
};

class FlameGoodie: public Goodie
{
public:
    FlameGoodie(double startX, double startY, StudentWorld* sw, int lifetime): Goodie(IID_FLAME_THROWER_GOODIE, startX, startY, 0, 1, sw, lifetime) {}
    void doSomething();
    void pickUp(Socrates *soc);
private:
};

class ExtraLifeGoodie: public Goodie
{
public:
     ExtraLifeGoodie(double startX, double startY, StudentWorld* sw, int lifetime): Goodie(IID_EXTRA_LIFE_GOODIE, startX, startY, 0, 1, sw, lifetime){}
    void doSomething();
    void pickUp(Socrates *soc);
private:

};

class Fungi: public Goodie
{
public:
    Fungi(int startX, int startY, StudentWorld* sw, int lifetime): Goodie(IID_FUNGUS, startX, startY, 0, 1, sw, lifetime) {}
    
    void doSomething();
    void pickUp(Socrates *soc);
    bool takeDamage(int x)
    {
        this->kill();
        return true;
    }
    
};


// Things with lifespan: flamegoodie, lifegoodie, healthgoodie, fungus,
#endif // ACTOR_H_
