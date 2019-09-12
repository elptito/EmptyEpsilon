#include "scienceTask.h"
#include "gameGlobalInfo.h"

ScienceTask::ScienceTask() : parent(nullptr), target_id(-1), target_system(SYS_None), timeout(0){
    clear();
}

void ScienceTask::addHackTask(ScienceTask tasks[], int size, uint32_t target_id, ESystem target_system){
    for(int n = 0; n < size; n++)
        if (tasks[n].type == STT_Hack && tasks[n].target_id == target_id && tasks[n].target_system == target_system)
            return;
    for(int n = 0; n < size; n++)
        if (tasks[n].orderHack(target_id, target_system))
            return;
}

void ScienceTask::addScanTask(ScienceTask tasks[], int size, uint32_t target_id){
    for(int n = 0; n < size; n++)
        if (tasks[n].type == STT_Scan && tasks[n].target_id == target_id)
            return;
    for(int n = 0; n < size; n++)
        if (tasks[n].orderScan(target_id))
            return;
}

int ScienceTask::countTasks(ScienceTask tasks[], int size){
    int result = 0;
    for(int n = 0; n < size; n++){
        if (tasks[n].type != STT_Empty)
            result++;
    }
    return result;
}

void ScienceTask::update(float delta){
    if (game_server && type != STT_Empty){
        timeout -=delta;
        P<SpaceShip> target = getObjectById(target_id);
        if (timeout <= 0.f || !target 
            || (type == STT_Hack && !(target->canBeHackedBy(parent) && target->hasSystem(target_system)))
            || (type == STT_Scan && !target->canBeScannedBy(parent))){
            clear();
        }
    }
}

string ScienceTask::getDescription(){
    P<SpaceShip> target = getObjectById(target_id);
    if (target){
        string res = getScienceTaskTypeName(type) + " " + target->getCallSign();
        if (type == STT_Hack){
            res += "'s " + getSystemName(target_system);
        }
        return res;
    } else {
        return "";
    }
}

void ScienceTask::setParent(PlayerSpaceship *parent){
    assert(!this->parent);
    this->parent = parent;

    parent->registerMemberReplication(&type);
    parent->registerMemberReplication(&target_id);
    parent->registerMemberReplication(&target_system);
    parent->registerMemberReplication(&timeout);
} 

// void ScienceTask::complete(){
//     parent->commandCompleteScienceTask(index_at_parent);
// }

void ScienceTask::clear(){
    timeout = 0.f;
    target_id = -1;
    target_system = SYS_None;
    type = STT_Empty;
}

bool ScienceTask::orderScan(uint32_t target_id){
    if (type == STT_Empty){
        type = STT_Scan;
        timeout = 10 * 60 * 1000;
        this->target_id = target_id;
        return true;
    }
    return false;
}

bool ScienceTask::orderHack(uint32_t target_id, ESystem targetSystem){
    if (type == STT_Empty){
        type = STT_Hack;
        timeout = 10 * 60 * 1000;
        this->target_id = target_id;
        this->target_system = targetSystem;
        return true;
    }
    return false;
}

string getScienceTaskTypeName(EScienceTaskType taskType){
    switch(taskType){
        case STT_Empty: return "None";
        case STT_Hack: return "Hack";
        case STT_Scan: return "Scan";
    }
    return "unknown";
};