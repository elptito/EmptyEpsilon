#include "hackingJob.h"
#include "spaceObjects/spaceship.h"
#include "gameGlobalInfo.h"

HackingJob::HackingJob() : parent(nullptr), target_id(-1), target_system(""), timeout(0){
    clear();
}

void HackingJob::update(float delta){
    if (status == HJS_Pending){
        timeout -=delta;
        if (timeout <= 0.f){
            timeout = 0.f;
            clear();
        }
    }
}

void HackingJob::setParent(SpaceShip *parent){
    assert(!this->parent);
    this->parent = parent;

    parent->registerMemberReplication(&status);
    parent->registerMemberReplication(&target_id);
    parent->registerMemberReplication(&target_system);
    parent->registerMemberReplication(&timeout);
} 

void HackingJob::complete(){
    P<SpaceShip> target = getObjectById(target_id);
    if (target)
        parent->commandHackingFinished(target, target_system);
    clear();
}

void HackingJob::clear(){
    timeout = 0.f;
    target_id = -1;
    target_system = "";
    status = HJS_Empty;
}