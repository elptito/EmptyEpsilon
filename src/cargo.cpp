#include "cargo.h"

Cargo::Cargo(string multiplayerClassIdentifier) : MultiplayerObject(multiplayerClassIdentifier), energy_level(0), heat(0)
{
    registerMemberReplication(&heat);
    registerMemberReplication(&energy_level);
}

Cargo::Entries Cargo::getEntries()
{
    Cargo::Entries result;
    result.push_back(std::make_tuple("gui/icons/energy", "energie", int(energy_level)));
    result.push_back(std::make_tuple("gui/icons/heat", "surchauffe", string(heat, 2)));
    //result.push_back(std::make_tuple("gui/icons/hull", "carlingue", string(int(100 * getHealth() / getMaxHealth())) + "%"));

    return result;
}
