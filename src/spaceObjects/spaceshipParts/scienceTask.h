#ifndef SCIENCE_TASK_H
#define SCIENCE_TASK_H

#include "P.h"
#include "engine.h"
#include "shipTemplate.h"

class SpaceShip;
class PlayerSpaceship;

enum EScienceTaskType {
  STT_Empty,
  STT_Hack,
  STT_Scan,
};
string getScienceTaskTypeName(EScienceTaskType taskType);

REGISTER_MULTIPLAYER_ENUM(EScienceTaskType);

class ScienceTask : public sf::NonCopyable
{
  public:

  static int countTasks(ScienceTask tasks[], int size);

  protected:
    PlayerSpaceship *parent;
    int index_at_parent;

  public:
    EScienceTaskType type;
    int32_t target_id;
    ESystem target_system;
    float timeout;

    ScienceTask();

    void update(float delta);
    void setParent(PlayerSpaceship *parent);
    void setIndex(int index) { this->index_at_parent = index; }
    
    // void complete();
    string getDescription();
    bool orderHack(P<SpaceShip> target, ESystem targetSystem);
    bool orderScan(P<SpaceObject> target);
    void clear();
};

#endif //SCIENCE_TASK_H
