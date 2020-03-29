#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <cstdlib>
#include <stdio.h>
using namespace std;


// No two pits overlap one another
// Food can not overlap with other food or pits
// No dirt objects may overlap with previously placed food objects or pits, they may overlap with themselves


GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath), m_socrates(nullptr), m_level(1), m_predators(0){}

StudentWorld::~StudentWorld()
{
    cleanUp();
}

bool StudentWorld::overlaps(Actor* a, double x, double y) const
{
    return distance(a->getX(), a->getY(), x, y ) < SPRITE_WIDTH/2;
}

bool StudentWorld::overlaps(Actor* a, Actor* b) const
{
    return distance(a->getX(), a->getY(), b->getX(), b->getY()) < SPRITE_WIDTH/2;
}

int StudentWorld::getLevel() const
{
    return m_level;
}

double StudentWorld::distance(int x1, int y1, int x2, int y2) const
{
    double diffX = x2 - x1;
    double diffY = y2 - y1;
    return sqrt(((diffX)*(diffX))+(diffY)*(diffY));
}
double StudentWorld::angle(int x1, int y1, int x2, int y2) const
{
    double dx = x2 - x1;
    double dy = y2 - y1;
    
    double rad_to_deg = 180/M_PI;
    double anglerad = atan(dy/dx);
    double tmp = rad_to_deg * anglerad;
    if( dx >= 0.0)
        return tmp;
    else
        return tmp+180;
}

Actor *StudentWorld::getPlayer() const
{
    return m_socrates;;
}

// Add an actor to the world.
void StudentWorld::addActor(Actor* a)
{
    m_actors.push_back(a);
}

// If actor a ovelaps some live actor, damage that live actor by the
// indicated amount of damage and return true; otherwise, return false.
bool StudentWorld::damageOneActor(Actor* a, int damage)
{
    std::vector<Actor*>::size_type size = m_actors.size();
    for (std::vector<Actor*>::size_type i = 0; i < size; ++i)
    {
        if (m_actors[i] != a && m_actors[i] != m_socrates)
        {
            if( distance(m_actors[i]->getX(), m_actors[i]->getY(), a->getX(), a->getY()) < SPRITE_WIDTH)
            {
                return m_actors[i]->takeDamage(damage);
            }
        }
    }
    return false;
}
  // Is bacterium a blocked from moving to the indicated location?
bool StudentWorld::isBacteriumMovementBlockedAt(Actor* a, double x, double y) const
{
    vector<Actor*>::const_iterator ai = m_actors.begin();
    while ((ai != m_actors.end()))
    {
        if ((*ai) != a && ((*ai)->blockBacteria()))
        {
            if(overlaps((*ai), x, y))
            {
                return true;
            }
        }
        ai++;
    }
    if(distance(x,y, VIEW_WIDTH/2, VIEW_WIDTH/2) > VIEW_WIDTH/2)
    {
        return true;
    }
    return false;
}
  // If actor a overlaps this world's socrates, return a pointer to the
  // socrates; otherwise, return nullptr
Socrates* StudentWorld::getOverlappingSocrates(Actor* a, double dist) const
{
    if(distance(a->getX(), a->getY(), m_socrates->getX(), m_socrates->getY()) < dist)
    {
        return m_socrates;
    }
    return nullptr;
}
  // If actor a overlaps a living edible object, return a pointer to the
  // edible object; otherwise, return nullptr
Actor* StudentWorld::getOverlappingEdible(Actor* a) const
{
    vector<Actor*>::const_iterator ai = m_actors.begin();
    while ((ai != m_actors.end()))
    {
        if ((*ai)->isFood())
        {
            if(overlaps((*ai), a))
            {
                return (*ai);
            }
        }
        ai++;
    }
    return nullptr;
}
  // Return true if this world's socrates is within the indicated distance
  // of actor a; otherwise false.  If true, angle will be set to the
  // direction from actor a to the socrates.
bool StudentWorld::getAngleToNearbySocrates(Actor* a, int dist, int& angle) const
{
    if(distance(m_socrates->getX(), m_socrates->getY(), a->getX(), a->getY()) < dist)
    {
        angle = this->angle(a->getX(), a->getY(), m_socrates->getX(), m_socrates->getY());
        return true;
    }
    return false;
}
  // Return true if there is a living edible object within the indicated
  // distance from actor a; otherwise false.  If true, angle will be set
  // to the direction from actor a to the edible object nearest to it.
bool StudentWorld::getAngleToNearestNearbyEdible(Actor* a, int dist, int& angle) const
{
    vector<Actor*>::const_iterator ai = m_actors.begin();
    double shortest = 100000.00;
    while ((ai != m_actors.end()))
    {
        if ((*ai)->isFood())
        {
            double test = distance((*ai)->getX(), (*ai)->getY(), a->getX(), a->getY());
            if(test < dist)
            {
                if(test < shortest)
                {
                    angle = this->angle(a->getX(), a->getY(), (*ai)->getX(), (*ai)->getY());
                    shortest = test;
                }
            }
        }
        ai++;
    }
    if( shortest < dist)
    {
        return true;
    }
    return false;
}

// Set x and y to the position on the circumference of the Petri dish
// at the indicated angle from the center.  (The circumference is
// where socrates and goodies are placed.)
void StudentWorld::getPositionOnCircumference(int angle, double& x, double& y) const
{
}


double StudentWorld::distanceFromCenter(int x1, int y1)
{
    return distance(x1, y1, VIEW_WIDTH/2, VIEW_HEIGHT/2);
}

bool StudentWorld::notOverlap(int x1, int y1, vector<Actor*> b)
{
    vector<Actor*>::iterator bv = b.begin();
    for (; bv != b.end(); bv++)
    {
        int x2 = ((*bv)->getX());
        int y2 = ((*bv)->getY());
        double c = distance(x1, y1, x2, y2);
        if (c < SPRITE_WIDTH)
        {
            return false;
        }
    }
    return true;
}

void StudentWorld::RandomPerimeterPos(double& posx, double& posy)
{
    int a = randInt(0, 360);
    double rad = a*(M_PI/180.0);
    posx = cos(rad) * VIEW_WIDTH/2 + VIEW_WIDTH/2;
    posy = sin(rad) * VIEW_WIDTH/2 + VIEW_WIDTH/2;
}

int StudentWorld::init()
{
    m_predators = 0;
    // Every time a level starts or restarts, allocate and insert a new Socrates object 
    // into the game world. Fully initialized (with 20, 5 spray/flame charges).
    m_socrates = new Socrates(this);
    m_actors.push_back(m_socrates);
    
    for (int i = 0; i < m_level; i++)
    {
        int x = randInt(0, VIEW_WIDTH);
        int y = randInt(0, VIEW_HEIGHT);
            
        // If pit does not overlap with any other pits.
        while ( !notOverlap(x, y, m_actors) ||
                distanceFromCenter(x, y) > VIEW_WIDTH/2 -8)
        {
            x = randInt(0, VIEW_WIDTH);
            y = randInt(0, VIEW_HEIGHT);
        }
        Pit *p = new Pit(x, y, this);
        m_actors.push_back(p);
    }
    
    // Initialize the food
    int min = randInt(5*m_level, 25);
    for (int i = 0; i < min; i++)
    {
        int x = randInt(0, VIEW_WIDTH);
        int y = randInt(0, VIEW_HEIGHT);
        // Condition so that food is not touching outside of circle
        if (distanceFromCenter(x, y) < VIEW_WIDTH/2 -8)
        {
            // Condition so that no food is touching
            if (notOverlap(x, y, m_actors)) // Condition so that not overlapping with food or pits
            {
                Food *f = new Food(x, y, this);
                m_actors.push_back(f); // Then make a new food
            }

        }

    }
    
    // Max function finds the largest of two values, for levels 1-8 it would compute 
    // # of dirt by the first expresssion.
   int mx = max(180-20*getLevel(), 20);
   for (int i = 0; i < mx; i++)
   {
       int x = randInt(0, VIEW_WIDTH);
       int y = randInt(0, VIEW_HEIGHT);
       // Condition so that dirt is not touching outside of circle
       if (distanceFromCenter(x, y) < VIEW_WIDTH/2 - 8)
       {
           // Check to make sure that dirt is not overlapping food or pit
           // Okay for dirt to overlap dirt
           if (notOverlap(x, y, m_actors))
           {
               m_actors.push_back(new Dirt(x, y, this));
           }
          
       }
       
   }
    // Then make a noOfPit new pits
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    
    // decLives();
    
    //  Not necessary anymore since wrote noOverlap() function;
    std::vector<Actor*>::size_type size = m_actors.size();
    for (std::vector<Actor*>::size_type i = 0; i < size; ++i)
    {
        if( m_actors[i]->isAlive())
        {
            m_actors[i]->doSomething();
        }
    }
    if(!(m_socrates->isAlive()))
    {
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }

    if(getPredators() <=0 )
    {
        m_level++;
        playSound(SOUND_FINISHED_LEVEL);
        return GWSTATUS_FINISHED_LEVEL;
    }
    vector<Actor*>::iterator ai = m_actors.begin();
    int ChanceGoodie = max(510-m_level*10, 250);
    int goodieChance = randInt(0, ChanceGoodie);
    if (goodieChance == 0)
    {
        int typeOfGoodie = randInt(0, 9);
        double x, y;
        if (typeOfGoodie <= 5)
        {
            RandomPerimeterPos(x, y);
            m_actors.push_back(new HealthGoodie(x, y, this, max(randInt(0, 300-10*m_level-1), 50)));
        
        } // Create restore health goodie
        if ((typeOfGoodie > 5) && (typeOfGoodie <= 8))
        {
            RandomPerimeterPos(x, y);
            m_actors.push_back(new FlameGoodie(x, y, this, max(randInt(0, 300-10*m_level-1), 50)));
            
        } // Create flamethrower goodie
        if (typeOfGoodie > 8)
        {
            RandomPerimeterPos(x, y);
            m_actors.push_back(new ExtraLifeGoodie(x, y, this, max(randInt(0, 300-10*m_level-1), 50)));
            
        } // Create extra life goodie
        
    }
    
    string gstats = "SCORE: ";
    char sc[8];
    sprintf(sc, "%06d", getScore());
    gstats+=string(sc);
    gstats+="  Level: ";
    gstats+=to_string(getLevel());
    gstats+="  Lives: ";
    gstats+=to_string(getLives());
    gstats+="  Health: ";
    gstats+=to_string(m_socrates->getHP());
    gstats+="  Sprays: ";
    gstats+=to_string(m_socrates->getSprays());
    gstats+="  Flames: ";
    gstats+=to_string(m_socrates->getFlames());
    
    setGameStatText(gstats);
    
    ai = m_actors.begin();
    while (ai != m_actors.end())
    {
        if (!((*ai)->isAlive()))
        {
            delete(*ai);
            (*ai) = nullptr;
            ai = m_actors.erase(ai);
        }
        else
        {
            ai++;
        }
    }

    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    while (m_actors.size())
    {
        delete m_actors.back();         // Deallocates memory
        m_actors.back() = nullptr;      // Makes the pointer safe 
        m_actors.pop_back();            // Removes position in vector
    }
    m_socrates = nullptr;
}


