#include "hackingJob.h"
#include "gameGlobalInfo.h"

HackingJob::HackingJob() : parent(nullptr), target_id(-1), target_system(SYS_None), timeout(0){
    clear();
}

int HackingJob::countJobs(HackingJob jobs[], int size){
    int result = 0;
    for(int n = 0; n < size; n++){
        if (!jobs[n].empty)
            result++;
    }
    return result;
}


void HackingJob::update(float delta){
    if (!empty){
        timeout -=delta;
    }
    P<SpaceObject> target = getObjectById(target_id);
    if (timeout <= 0.f || !target || target->canBeHackedBy(parent)){
        clear();
    }
}

void HackingJob::setParent(SpaceShip *parent){
    assert(!this->parent);
    this->parent = parent;

    parent->registerMemberReplication(&empty);
    parent->registerMemberReplication(&target_id);
    parent->registerMemberReplication(&target_system);
    parent->registerMemberReplication(&timeout);
} 

void HackingJob::complete(){
    P<SpaceShip> target = getObjectById(target_id);
    if (target && target->canBeHackedBy(parent)){
        parent->commandHackingFinished(target, target_system);
    }
    clear();
}

void HackingJob::clear(){
    timeout = 0.f;
    target_id = -1;
    target_system = SYS_None;
    empty = true;
}

bool HackingJob::init(P<SpaceShip> target, ESystem target_system){
    if (empty){
        empty = false;
        timeout = 10 * 60 * 1000;
        this->target_id = target->getMultiplayerId();
        this->target_system = target_system;
        return true;
    }
    return false;
}