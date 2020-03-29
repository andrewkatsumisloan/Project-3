#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

//const int KEY_PRESS_LEFT   = 1000;
//const int KEY_PRESS_RIGHT  = 1001;
//const int KEY_PRESS_UP     = 1002;
//const int KEY_PRESS_DOWN   = 1003;
//const int KEY_PRESS_SPACE  = ' ';
//const int KEY_PRESS_TAB    = '\t';
//const int KEY_PRESS_ENTER  = '\r';
//const int KEY_PRESS_ESCAPE = '\x1b';

// Current problem
// Breaks, "BAD ACESS" when hit pizza (sometimes, not all of the time
// Does not change direction when reaching edge of circle.


void Socrates::doSomething()
{   if (!isAlive())
    {
        return;
    }
    if(m_sprayCharges <=19)
    {
        m_sprayCharges++;
    }
    double degToRad = M_PI/180.0;
    int key;
    getWorld()->getKey(key);
    double x;
    double y;
    
    // Left should be counterclockwise, Right should be clockwise
    if (key == KEY_PRESS_LEFT)
    {
        m_angle -= 5;
        x = sin(m_angle * degToRad) * VIEW_WIDTH/2+VIEW_WIDTH/2;
        y = cos(m_angle * degToRad) * VIEW_WIDTH/2+VIEW_WIDTH/2;
        moveTo(x, y);
        setDirection(360-(m_angle+90));
    }
    else if (key == KEY_PRESS_RIGHT)
    {
        m_angle += 5;
        // Compute the x and y components according where upper left is 0,0
        x = sin(m_angle * degToRad) * VIEW_WIDTH/2+VIEW_WIDTH/2;
        y = cos(m_angle * degToRad) * VIEW_WIDTH/2+VIEW_WIDTH/2;
        moveTo(x, y);
        setDirection(360-(m_angle+90));
    }
    else if (key == KEY_PRESS_UP)
    {
        
    }
    else if (key == KEY_PRESS_DOWN)
    {
        
    }
    else if (key == KEY_PRESS_SPACE)
    {
        if(m_sprayCharges > 0)
        {
            x = sin((180-(getDirection()+90)) * degToRad) * SPRITE_WIDTH * 2 + getX();
            y = cos((180-(getDirection()+90)) * degToRad) * SPRITE_WIDTH * 2 + getY();
            getWorld()->addActor(new Spray(x, y, getDirection(), getWorld()));
            m_sprayCharges--;
            getWorld()->playSound(SOUND_PLAYER_SPRAY);
        }
    }
    else if (key == KEY_PRESS_TAB)
    {
        
    }
    else if (key == KEY_PRESS_ENTER)
    {
        
        if(m_flameCharges >= 1)
        {
            for( int j = 0; j < 360; j+=22)
            {
                x = sin((180-(j+90)) * degToRad) * SPRITE_WIDTH + getX();
                y = cos((180-(j+90)) * degToRad) * SPRITE_WIDTH + getY();
                getWorld()->addActor(new Flame(x, y, j, getWorld()));
            }
            m_flameCharges--;
            getWorld()->playSound(SOUND_PLAYER_FIRE);
        }
    }
    else if (key == KEY_PRESS_ESCAPE)
    {
        
    }
}

bool Socrates::takeDamage(int x)
{
   decrementHP(x);
   if (getHP() <= 0)
   {
       this->kill();
       getWorld()->playSound(SOUND_PLAYER_DIE);
   }
   else
   {
        getWorld()->playSound(SOUND_PLAYER_HURT);
   }
   return true;
}

Disease::Disease(int imageID, double startX, double startY, StudentWorld* sw, int HP):
        LifeForm(imageID, startX, startY, 90, 0, sw, false, false, HP),
            m_movePlan(0), m_foodCount(0) 
{
    getWorld()->incrementPredators();
}
Disease::~Disease()
{
    getWorld()->decrementPredators();
}

void Disease::spawnSite(int &newx, int &newy)
{
    newx = this->getX();
    newy = this->getY();
    if (this->getX() < VIEW_WIDTH/2)
    {
        newx = newx + SPRITE_WIDTH/2;
    }
    else if (this->getX() > VIEW_WIDTH/2)
    {
        newx = newx - SPRITE_WIDTH/2;
    }
    if (this->getY() < VIEW_HEIGHT/2)
    {
        newy = newy + SPRITE_WIDTH/2;
    }
    else if (this->getY() > VIEW_HEIGHT/2)
    {
        newy = newy - SPRITE_WIDTH/2;
    }
}
bool Disease::infect(int severity)
{
    Socrates *soc = getWorld()->getOverlappingSocrates(this);
    if(soc != nullptr)
    {
        return soc->takeDamage(severity);
    }
    return false;
}

// Necessary to skip to the fifth
void Salmonella::planMotion()
{
    if (getMP() > 0)
    {
        decrementMP(1);
        double threex, threey;
        getPositionInThisDirection(getDirection(), 3, threex, threey);
        if (getWorld()->isBacteriumMovementBlockedAt(this, threex, threey))
        {
            setDirection(randInt(0,359));
            setMP(10);
        }
        else
        {
            moveTo(threex, threey);
        }
        return;
    }
    else
    {
        int angle = 0;
        double threex, threey;
        if(getWorld()->getAngleToNearestNearbyEdible(this, VIEW_WIDTH/2, angle))
        {
            setDirection(angle);
            getPositionInThisDirection(getDirection(), 3, threex, threey);
            if (getWorld()->isBacteriumMovementBlockedAt(this, threex, threey))
            {
                setDirection(randInt(0,359));
                setMP(10);
                return;
            }
            else
            {
                moveTo(threex, threey);
            }
        }
        else
        {
            setDirection(randInt(0,359));
            setMP(10);
            return;
        }
    }
}

void Sal::spawnAndFeed()
{
    bool skip = false;
    if (this->getFC() == 3)
    {
        int newx, newy;
        this->spawnSite(newx, newy);
        this->resetFC();
        getWorld()->getActors().push_back(new Sal(newx, newy, getWorld()));
        skip = true;
    }
    if(!skip)
    {
        Actor *food = getWorld()->getOverlappingEdible(this);
        
        if(food != nullptr)
        {
            this->increaseFC();
            food->kill();
        }
    }
}

void AggSal::spawnAndFeed()
{
    bool skip = false;
    if (this->getFC() == 3)
    {
        int newx, newy;
        this->spawnSite(newx, newy);
        this->resetFC();
        getWorld()->getActors().push_back(new AggSal(newx, newy, getWorld()));
        skip = true;
    }
    if(!skip)
    {
        Actor *food = getWorld()->getOverlappingEdible(this);
        
        if(food != nullptr)
        {
            this->increaseFC();
            food->kill();
        }
    }
}

void Ecoli::spawnAndFeed()
{
    bool skip = false;
    if (this->getFC() == 3)
    {
        int newx, newy;
        this->spawnSite(newx, newy);
        this->resetFC();
        getWorld()->getActors().push_back(new Ecoli(newx, newy, getWorld()));
        skip = true;
    }
    if(!skip)
    {
        Actor *food = getWorld()->getOverlappingEdible(this);
        
        if(food != nullptr)
        {
            this->increaseFC();
            food->kill();
        }
    }
}

bool Ecoli::takeDamage(int x)
{
    this->decrementHP(x);
    if (getHP() <= 0)
    {
        int a = randInt(0,1);
        if(a ==0)
        {
            getWorld()->addActor(new Food(getX(), getY(), getWorld()));
        }
        this->kill();
        getWorld()->increaseScore(100);
        getWorld()->playSound(SOUND_ECOLI_DIE);
    }
    else
    {
        getWorld()->playSound(SOUND_ECOLI_HURT);
    }
    return true;
}

bool Salmonella::takeDamage(int x)
{
    this->decrementHP(x);
    if (getHP() <= 0)
    {
        int a = randInt(0,1);
        if(a ==0)
        {
            getWorld()->addActor(new Food(getX(), getY(), getWorld()));
        }
        this->kill();
        getWorld()->increaseScore(100);
        getWorld()->playSound(SOUND_SALMONELLA_DIE);
    }
    else
    {
        getWorld()->playSound(SOUND_SALMONELLA_HURT);
    }
    return true;
}

void Sal::doSomething()
{
    if (!isAlive())
    {
        return;
    }
    if (! infect(1))
    {
        spawnAndFeed();
    }
    planMotion();
}

void AggSal::doSomething()
{
    if (!isAlive())
    {
        return;
    }
    int angle = 0;
    double threex, threey;
    if(getWorld()->getAngleToNearbySocrates(this, 72, angle))
    {
        setDirection(angle);
        getPositionInThisDirection(getDirection(), 3, threex, threey);
    }
    if(!infect(2))
    {
        spawnAndFeed();
    }
    planMotion();
}

void Ecoli::doSomething()
{
    if (!isAlive())
    {
        return;
    }
    if(! infect(4))
    {
        spawnAndFeed();
    }
    int angle = 0;
    double dx, dy;
    int tries = 0;
    if(getWorld()->getAngleToNearbySocrates(this, 256, angle))
    {
        while(tries < 10)
        {
            setDirection(angle);
            getPositionInThisDirection(getDirection(), 2, dx, dy);
            if(getWorld()->isBacteriumMovementBlockedAt(this, dx,dy))
            {
                setDirection((getDirection()+10)%360);
            }
            else
            {
                moveTo(dx, dy);
                return;
            }
            tries++;    
        }
    }
    return;
}


Pit::Pit(double startX, double startY, StudentWorld* sw):
        Actor(IID_PIT, startX, startY, 0, 1, sw, false, false), numSal(5), numAggSal(3), numEcoli(2) 
{
    getWorld()->incrementPredators();
}

Pit::~Pit()
{
    getWorld()->decrementPredators();
}

void Pit::doSomething()
{
    if (!isAlive())
    {
        return;
    }
    if (numSal <= 0 && numAggSal <= 0 && numEcoli <= 0)
    {
        // Tell world that it has no more bacteria.
        this->kill();
    }
    int a = randInt(1, 50);
    if (a == 1)
    {
        int b  = randInt(0, 2);
        if ((b == 0) && numSal > 0)
        {
            getWorld()->getActors().push_back(new Sal(this->getX(), this->getY(), getWorld()));
            getWorld()->playSound(SOUND_BACTERIUM_BORN);
            numSal--;
        }
        
        else
        {
            b = 1;
        }
        if ((b == 1) && numAggSal > 0)
        {
            getWorld()->getActors().push_back(new AggSal(this->getX(), this->getY(), getWorld()));
            getWorld()->playSound(SOUND_BACTERIUM_BORN);
            numAggSal--;
        }
        else
        {
            b = 2;
        }
        if ((b == 2) && numEcoli > 0)
        {
            getWorld()->getActors().push_back(new Ecoli(this->getX(), this->getY(), getWorld()));
            getWorld()->playSound(SOUND_BACTERIUM_BORN);
            numEcoli--;
        }
        
    }
}

void Flame::doSomething()
{
    if (!(isAlive()))
    {
        return;
    }

    if (getWorld()->damageOneActor(this, 5))
    {
        this->kill();
        return;
    }
    else // If there is nothing that is not a player within sprite width of the flame...
    {
        double x, y;
        getPositionInThisDirection(getDirection(), SPRITE_WIDTH, x, y);

        // This checks to see if the distance traveled is >= the max traveldistance, 
        // and increments the distance traveled if it is not.p
        incrementTravel(); 

        moveTo(x, y); // This is what actually moves the location if not exceed max travel dist.
    }
}

void Spray::doSomething()
{
    if (!(isAlive()))
    {
        return;
    }
    if (getWorld()->damageOneActor(this, 5))
    {
        this->kill();
        return;
    }
    else // If there is nothing that is not a player within sprite width of the flame...
    {
        double x, y;
        getPositionInThisDirection(getDirection(), SPRITE_WIDTH, x, y);

        // This checks to see if the distance traveled is >= the max traveldistance, 
        // and increments the distance traveled if it is not.
        incrementTravel();

        // This is what actually moves the location if not exceed max travel dist.
        moveTo(x, y); 
    }
}

void Goodie::doSomething()
{
    int a = getAge();
    if (a == m_lifetime)
    {
        this->kill();
    }
    this->incrementAge();
    Socrates* soc = getWorld()->getOverlappingSocrates(this, SPRITE_WIDTH);
    if (!(soc == nullptr))
    {
        pickUp(soc);
    }
}
void HealthGoodie::doSomething()
{
    Goodie::doSomething();
}

void FlameGoodie::doSomething()
{
    Goodie::doSomething();
}

void ExtraLifeGoodie::doSomething()
{
    Goodie::doSomething();
}

void Fungi::doSomething()
{
    Goodie::doSomething();
    Socrates *a = getWorld()->getOverlappingSocrates(this);
    if(a == nullptr)
    {
        return;
    }
    else
    {
        a->takeDamage(20);
    }
}

void HealthGoodie::pickUp(Socrates *soc)
{
        this->kill();
        soc->resetHP();
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->increaseScore(250);
}

void FlameGoodie::pickUp(Socrates *soc)
{
        this->kill();
        soc->addFlames();
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->increaseScore(300);
}

void ExtraLifeGoodie::pickUp(Socrates *soc)
{
        this->kill();
        getWorld()->incLives();
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->increaseScore(500);
}

void Fungi::pickUp(Socrates *soc)
{
        this->kill();
        getWorld()->incLives();
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->increaseScore(-50);
}


