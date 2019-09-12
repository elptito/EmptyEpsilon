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
  static void addHackTask(ScienceTask tasks[], int size, uint32_t target_id, ESystem target_system);
  static void addScanTask(ScienceTask tasks[], int size, uint32_t target_id);

  protected:
    PlayerSpaceship *parent;
    int index_at_parent;

  public:
    EScienceTaskType type;
    uint32_t target_id;
    ESystem target_system;
    float timeout;

    ScienceTask();

    void update(float delta);
    void setParent(PlayerSpaceship *parent);
    void setIndex(int index) { this->index_at_parent = index; }
    
    // void complete();
    string getDescription();
    void clear();
private: 
    bool orderHack(uint32_t target_id, ESystem targetSystem);
    bool orderScan(uint32_t target_id);
};

#endif //SCIENCE_TASK_H
