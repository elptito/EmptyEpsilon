#include "scienceTask.h"
#include "gameGlobalInfo.h"

ScienceTask::ScienceTask() : parent(nullptr), target_id(-1), target_system(SYS_None), timeout(0){
    clear();
}

int ScienceTask::countTasks(ScienceTask tasks[], int size){
    int result = 0;
    for(int n = 0; n < size; n++){
        if (!tasks[n].empty)
            result++;
    }
    return result;
}


void ScienceTask::update(float delta){
    if (!empty){
        timeout -=delta;
    }
    P<SpaceObject> target = getObjectById(target_id);
    if (timeout <= 0.f || !target || target->canBeHackedBy(parent)){
        clear();
    }
}

void ScienceTask::setParent(SpaceShip *parent){
    assert(!this->parent);
    this->parent = parent;

    parent->registerMemberReplication(&empty);
    parent->registerMemberReplication(&target_id);
    parent->registerMemberReplication(&target_system);
    parent->registerMemberReplication(&timeout);
} 

void ScienceTask::complete(){
    P<SpaceShip> target = getObjectById(target_id);
    if (target && target->canBeHackedBy(parent)){
        parent->commandHackingFinished(target, target_system);
    }
    clear();
}

void ScienceTask::clear(){
    timeout = 0.f;
    target_id = -1;
    target_system = SYS_None;
    empty = true;
}

bool ScienceTask::init(P<SpaceShip> target, ESystem target_system){
    if (empty){
        empty = false;
        timeout = 10 * 60 * 1000;
        this->target_id = target->getMultiplayerId();
        this->target_system = target_system;
        return true;
    }
    return false;
}