#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include <string>
#include <vector>

using namespace std;
// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    // Write setLevel() function
    ~StudentWorld();
    int getLevel() const;
    double distance(int x1, int y1, int x2, int y2) const;
    double angle(int x1, int y1, int x2, int y2) const;
    double distanceFromCenter(int x1, int y1);
    bool notOverlap(int x1, int y1, vector<Actor*> b);
    void RandomPerimeterPos(double& posx, double& posy);
    vector<Actor*> & getActors() {return m_actors;}
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    Actor *getPlayer() const;
    bool overlaps(Actor* a, Actor* b) const;
    bool overlaps(Actor* a, double x, double y) const;
      // Add an actor to the world.
    void addActor(Actor* a);
   
    // "Predator count" is  a way for threats to tally themselves so that we can move to
    // the next level when the count goes to zero.
    int getPredators(){ return m_predators;}
    void incrementPredators(){ m_predators++;}
    void decrementPredators(){ m_predators--;}
    

      // If actor a overlaps some live actor, damage that live actor by the
      // indicated amount of damage and return true; otherwise, return false.
    bool damageOneActor(Actor* a, int damage);

      // Is bacterium a blocked from moving to the indicated location?
    bool isBacteriumMovementBlockedAt(Actor* a, double x, double y) const;

      // If actor a overlaps this world's socrates, return a pointer to the
      // socrates; otherwise, return nullptr
    Socrates* getOverlappingSocrates(Actor* a, double dist=SPRITE_WIDTH/2) const;

      // If actor a overlaps a living edible object, return a pointer to the
      // edible object; otherwise, return nullptr
    Actor* getOverlappingEdible(Actor* a) const;

      // Return true if this world's socrates is within the indicated distance
      // of actor a; otherwise false.  If true, angle will be set to the
      // direction from actor a to the socrates.
    bool getAngleToNearbySocrates(Actor* a, int dist, int& angle) const;

      // Return true if there is a living edible object within the indicated
      // distance from actor a; otherwise false.  If true, angle will be set
      // to the direction from actor a to the edible object nearest to it.
    bool getAngleToNearestNearbyEdible(Actor* a, int dist, int& angle) const;

      // Set x and y to the position on the circumference of the Petri dish
      // at the indicated angle from the center.  (The circumference is
      // where socrates and goodies are placed.)
    void getPositionOnCircumference(int angle, double& x, double& y) const;

private:
    vector<Actor*> m_actors;
    Socrates* m_socrates;
    int m_level;
    int m_predators;
    
};

#endif // STUDENTWORLD_H_
