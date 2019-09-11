#ifndef HACKING_JOB_H
#define HACKING_JOB_H

#include "P.h"
#include "spaceObjects/spaceship.h"

class SpaceShip;

class HackingJob : public sf::NonCopyable
{
  public:
  static int countJobs(HackingJob jobs[], int size);

  protected:
    SpaceShip *parent;

  public:
    bool empty;
    int32_t target_id;
    ESystem target_system;
    float timeout;

    HackingJob();

    void update(float delta);
    void setParent(SpaceShip *parent);
    
    void clear();
    void complete();
    bool init(P<SpaceShip> target, ESystem target_system);
};

#endif //HACKING_JOB_H
