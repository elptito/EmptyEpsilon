#ifndef HACKING_JOB_H
#define HACKING_JOB_H

#include "P.h"
#include "SFML/System/NonCopyable.hpp"

class SpaceShip;

enum EHackJobStatus
{
    HJS_Pending,
    HJS_InProgress,
    HJS_Empty
};
class HackingJob : public sf::NonCopyable
{
  protected:
    SpaceShip *parent;

  public:
    EHackJobStatus status;
    int32_t target_id;
    string target_system;
    float timeout;

    HackingJob();

    void update(float delta);
    void setParent(SpaceShip *parent);
    
    void clear();
    void complete();
};

#endif //HACKING_JOB_H
