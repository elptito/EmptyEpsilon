#include <i18n.h>
#include "shipTemplate.h"
#include "spaceObjects/spaceObject.h"
#include "mesh.h"

#include "scriptInterface.h"

/// ShipTemplates are created when EmptyEpsilon is started.
/// And used to fill the ship starting statistics, and other information.
REGISTER_SCRIPT_CLASS(ShipTemplate)
{
    REGISTER_SCRIPT_CLASS_FUNCTION(ShipTemplate, setName);
    REGISTER_SCRIPT_CLASS_FUNCTION(ShipTemplate, setLocaleName);
    /// Set the class name, and subclass name for the ship. Used to divide ships into different classes.
    REGISTER_SCRIPT_CLASS_FUNCTION(ShipTemplate, setClass);
    /// Set is the ShipTemplate is secret (not shown in database), false by default
    REGISTER_SCRIPT_CLASS_FUNCTION(ShipTemplate, setSecret);
    /// Check if the ShipTemplate is secret (not shown in database)
    REGISTER_SCRIPT_CLASS_FUNCTION(ShipTemplate, getSecret);
    REGISTER_SCRIPT_CLASS_FUNCTION(ShipTemplate, setOSName);
    REGISTER_SCRIPT_CLASS_FUNCTION(ShipTemplate, getOSName);
    REGISTER_SCRIPT_CLASS_FUNCTION(ShipTemplate, getHackDiff);
    REGISTER_SCRIPT_CLASS_FUNCTION(ShipTemplate, setHackDiff);
    /// Set the description shown for this ship in the science database.
    REGISTER_SCRIPT_CLASS_FUNCTION(ShipTemplate, setDescription);
    /// Sets the type of template. Defaults to normal ships, so then it does not need to be set.
    /// Example: template:setType("ship"), template:setType("playership"), template:setType("station")
    REGISTER_SCRIPT_CLASS_FUNCTION(ShipTemplate, setType);
    /// Set the default AI behaviour. EE has 3 types of AI coded into the game right now: "default", "fighter", "missilevolley"
    REGISTER_SCRIPT_CLASS_FUNCTION(ShipTemplate, setDefaultAI);
    /// Set the 3D model to be used for this template. The model referers to data set in the model_data.lua file.
    REGISTER_SCRIPT_CLASS_FUNCTION(ShipTemplate, setModel);
    /// Supply a list of ship classes that can be docked to this ship. setDockClasses("Starfighter") will allow all small starfighter type ships to dock with this ship.
    REGISTER_SCRIPT_CLASS_FUNCTION(ShipTemplate, setDockClasses);
    /// Set the amount of energy available for this ship. Note that only player ships use energy. So setting this for anything else is useless.
    REGISTER_SCRIPT_CLASS_FUNCTION(ShipTemplate, setEnergyStorage);
    REGISTER_SCRIPT_CLASS_FUNCTION(ShipTemplate, setRepairCrewCount);
    /// Setup a beam weapon.
    REGISTER_SCRIPT_CLASS_FUNCTION(ShipTemplate, setBeam);
    /// Setup a beam weapon.
    REGISTER_SCRIPT_CLASS_FUNCTION(ShipTemplate, setBeamWeapon);
    /// Setup a beam's turret.
    REGISTER_SCRIPT_CLASS_FUNCTION(ShipTemplate, setBeamWeaponTurret);
    /// Setup a beam weapon texture
    REGISTER_SCRIPT_CLASS_FUNCTION(ShipTemplate, setBeamTexture);
    REGISTER_SCRIPT_CLASS_FUNCTION(ShipTemplate, setBeamWeaponEnergyPerFire);
    REGISTER_SCRIPT_CLASS_FUNCTION(ShipTemplate, setBeamWeaponHeatPerFire);
    
    /// Set the amount of missile tubes, limited to a maximum of 16.
    REGISTER_SCRIPT_CLASS_FUNCTION(ShipTemplate, setTubes);
    /// set the amount of docks (launcher, energy, weapon, thermic, mainteance, stock)
    REGISTER_SCRIPT_CLASS_FUNCTION(ShipTemplate, setDocks);
    REGISTER_SCRIPT_CLASS_FUNCTION(ShipTemplate, setTubeLoadTime);
    REGISTER_SCRIPT_CLASS_FUNCTION(ShipTemplate, weaponTubeAllowMissle);
    REGISTER_SCRIPT_CLASS_FUNCTION(ShipTemplate, weaponTubeDisallowMissle);
    REGISTER_SCRIPT_CLASS_FUNCTION(ShipTemplate, setWeaponTubeExclusiveFor);
    REGISTER_SCRIPT_CLASS_FUNCTION(ShipTemplate, setTubeDirection);
    REGISTER_SCRIPT_CLASS_FUNCTION(ShipTemplate, setReactor);
    ///Set tube size, this will increase damage and blast radius.
    ///Possible values are : Small, Medium or Large
    REGISTER_SCRIPT_CLASS_FUNCTION(ShipTemplate, setTubeSize);
    
    /// Set the amount of starting hull
    REGISTER_SCRIPT_CLASS_FUNCTION(ShipTemplate, setHull);
    /// Set the shield levels, amount of parameters defines the amount of shields. (Up to a maximum of 8 shields)
    /// Example: setShieldData(400) setShieldData(100, 80) setShieldData(100, 50, 50)
    REGISTER_SCRIPT_CLASS_FUNCTION(ShipTemplate, setShields);
    /// Sets by how much the shields recharge over time for all shields. Default value is 0.3. Value is a float.
    REGISTER_SCRIPT_CLASS_FUNCTION(ShipTemplate, setShieldRechargeRate);
    /// Set the impulse speed, rotation speed and impulse acceleration for this ship.
    /// Compare SpaceShip:setImpulseMaxSpeed, :setRotationMaxSpeed, :setAcceleration.
    REGISTER_SCRIPT_CLASS_FUNCTION(ShipTemplate, setSpeed);
    /// Sets the combat maneuver power of this ship.
    REGISTER_SCRIPT_CLASS_FUNCTION(ShipTemplate, setCombatManeuver);
    /// Set the warp speed for warp level 1 for this ship. Setting this will indicate that this ship has a warpdrive. (normal value is 1000)
    REGISTER_SCRIPT_CLASS_FUNCTION(ShipTemplate, setWarpSpeed);
    /// Set if this ship shares energy with docked ships. Example: template:setSharesEnergyWithDocked(false)
    REGISTER_SCRIPT_CLASS_FUNCTION(ShipTemplate, setSharesEnergyWithDocked);
    /// Set if this ship restocks scan probes on docked ships. Example: template:setRestocksScanProbes(false)
    REGISTER_SCRIPT_CLASS_FUNCTION(ShipTemplate, setRestocksScanProbes);
    /// Set if this ship restores missiles on docked cpuships. Example template:setRestocksMissilesDocked(false)
    REGISTER_SCRIPT_CLASS_FUNCTION(ShipTemplate, setRestocksMissilesDocked);
    /// Set if this ship has a jump drive. Example: template:setJumpDrive(true)
    REGISTER_SCRIPT_CLASS_FUNCTION(ShipTemplate, setJumpDrive);
    /// Set this ship's minimum and maximum jump drive distances.
    /// Example: template:setJumpDrive(5000.0, 50000.0)
    REGISTER_SCRIPT_CLASS_FUNCTION(ShipTemplate, setJumpDriveRange);
    REGISTER_SCRIPT_CLASS_FUNCTION(ShipTemplate, setJumpDriveChargeTime);
    REGISTER_SCRIPT_CLASS_FUNCTION(ShipTemplate, setJumpDriveEnergy);
    REGISTER_SCRIPT_CLASS_FUNCTION(ShipTemplate, setCloaking);
    REGISTER_SCRIPT_CLASS_FUNCTION(ShipTemplate, setWeaponStorage);
    REGISTER_SCRIPT_CLASS_FUNCTION(ShipTemplate, setCustomWeaponStorage);
    ///Set a custom weapon based on a regular one.
    ///6 mandatory Arguments : base name (Homing, HVLI etc.), new weapon name, damage multiplier, speed, number of shots successively fired, type of damage
    // Damage multiplier and speed are ratio from base missile (1 is the same as the base missile). It will not increase blast radius.
    // Damage type is Kinetic, EMP, Energy
    REGISTER_SCRIPT_CLASS_FUNCTION(ShipTemplate, setCustomWeapon);
    REGISTER_SCRIPT_CLASS_FUNCTION(ShipTemplate, setCustomWeaponColor);
    REGISTER_SCRIPT_CLASS_FUNCTION(ShipTemplate, addRoom);
    REGISTER_SCRIPT_CLASS_FUNCTION(ShipTemplate, addRoomSystem);
    REGISTER_SCRIPT_CLASS_FUNCTION(ShipTemplate, addDoor);
    REGISTER_SCRIPT_CLASS_FUNCTION(ShipTemplate, addDrones);
    REGISTER_SCRIPT_CLASS_FUNCTION(ShipTemplate, setRadarTrace);
    ///multiplies the damage applied to systems when hit
    REGISTER_SCRIPT_CLASS_FUNCTION(ShipTemplate, setSystemDamageRatio);
    ///if the hull has a higher percentage than this, no damage to systems occur
    REGISTER_SCRIPT_CLASS_FUNCTION(ShipTemplate, setSystemDamageHullThreshold);
    /// Apply a rate to energy decrease. Float, default is 1. Won't affect production of energy.
    REGISTER_SCRIPT_CLASS_FUNCTION(ShipTemplate, setEnergyConsumptionRatio);
    REGISTER_SCRIPT_CLASS_FUNCTION(ShipTemplate, setLongRangeRadarRange);
    REGISTER_SCRIPT_CLASS_FUNCTION(ShipTemplate, setShortRangeRadarRange);
    REGISTER_SCRIPT_CLASS_FUNCTION(ShipTemplate, setImpulseSoundFile);
    REGISTER_SCRIPT_CLASS_FUNCTION(ShipTemplate, setCanScan);
    REGISTER_SCRIPT_CLASS_FUNCTION(ShipTemplate, setCanHack);
    REGISTER_SCRIPT_CLASS_FUNCTION(ShipTemplate, setCanDock);
    REGISTER_SCRIPT_CLASS_FUNCTION(ShipTemplate, setCanCombatManeuver);
    REGISTER_SCRIPT_CLASS_FUNCTION(ShipTemplate, setCanSelfDestruct);
    REGISTER_SCRIPT_CLASS_FUNCTION(ShipTemplate, setCanLaunchProbe);
    /// Return a new template with the given name, which is an exact copy of this template.
    /// Used to make easy variations of templates.
    REGISTER_SCRIPT_CLASS_FUNCTION(ShipTemplate, copy);
}

std::unordered_map<string, P<ShipTemplate> > ShipTemplate::templateMap;

ShipTemplate::ShipTemplate()
{
    if (game_server) { LOG(ERROR) << "ShipTemplate objects can not be created during a scenario."; destroy(); return; }

    type = Ship;
    class_name = tr("No class");
    sub_class_name = tr("No sub-class");
    shares_energy_with_docked = false;
    secret = false;
    hack_diff = 2;

    int os_name_counter = irandom(30, 65);
    int os_name_letter = irandom(0, 9);
    switch(os_name_letter)
    {
    case 0: os_name = "iou" + string(os_name_counter); break;
    case 1: os_name = "ase" + string(os_name_counter); break;
    case 2: os_name = "lon" + string(os_name_counter); break;
    case 3: os_name = "tre" + string(os_name_counter); break;
    case 4: os_name = "kou" + string(os_name_counter); break;
    case 5: os_name = "win" + string(os_name_counter); break;
    case 6: os_name = "ion" + string(os_name_counter); break;
    case 7: os_name = "pon" + string(os_name_counter); break;
    case 8: os_name = "hub" + string(os_name_counter); break;
    case 9: os_name = "zer" + string(os_name_counter); break;
    }
    
    repair_docked = false;
    restocks_scan_probes = false;
    restocks_missiles_docked = false;
    energy_storage_amount = 1000;
    repair_crew_count = 3;
    weapon_tube_count = 0;
    for(int n=0; n<max_weapon_tubes; n++)
    {
        weapon_tube[n].load_time = 8.0;
        weapon_tube[n].type_allowed_mask = (1 << MW_Count) - 1;
        weapon_tube[n].direction = 0;
        weapon_tube[n].size = MS_Medium;
    }
    hull = 70;
    shield_count = 0;
    for(int n=0; n<max_shield_count; n++)
        shield_level[n] = 0.0;
    shield_recharge_rate = 0.3f;
    impulse_speed = 500.0;
    impulse_acceleration = 20.0;
    turn_speed = 0.0;
    combat_maneuver_boost_speed = 0.0f;
    combat_maneuver_strafe_speed = 0.0f;
    warp_speed = 0.0;
    has_jump_drive = false;
    jump_drive_min_distance = 5000.0;
    jump_drive_max_distance = 50000.0;
    jump_drive_charge = jump_drive_max_distance;
    jump_drive_charge_time = 90.0;
    jump_drive_energy_per_km_charge = 4.0f;
    has_cloaking = false;
    for(int n=0; n<MW_Count; n++)
        weapon_storage[n] = 0;
    radar_trace = "RadarArrow.png";
    has_reactor = true;
    has_cloaking = false;
    launcher_dock_count = 0;
    energy_dock_count = 0;
    weapons_dock_count = 0;
    thermic_dock_count = 0;
    repair_dock_count = 0;
    stock_dock_count = 0;
    system_damage_ratio =1.0f;
    system_damage_hull_threshold = 0.0f;
    energy_consumption_ratio = 1.0f;
    impulse_sound_file = "sfx/engine.wav";
}

void ShipTemplate::setBeamTexture(int index, string texture)

{
    if (index >= 0 && index < max_beam_weapons)
    {
        beams[index].setBeamTexture(texture);
    }
}

void ShipTemplate::setTubes(int amount, float load_time)
{
    weapon_tube_count = std::min(max_weapon_tubes, amount);
    for(int n=0; n<max_weapon_tubes; n++)
        weapon_tube[n].load_time = load_time;
}

void ShipTemplate::setTubeLoadTime(int index, float load_time)
{
    if (index < 0 || index >= max_weapon_tubes)
        return;
    weapon_tube[index].load_time = load_time;
}

void ShipTemplate::weaponTubeAllowMissle(int index, EMissileWeapons type)
{
    if (index < 0 || index >= max_weapon_tubes)
        return;
    weapon_tube[index].type_allowed_mask |= (1 << type);
}

void ShipTemplate::weaponTubeDisallowMissle(int index, EMissileWeapons type)
{
    if (index < 0 || index >= max_weapon_tubes)
        return;
    weapon_tube[index].type_allowed_mask &=~(1 << type);
}

void ShipTemplate::setWeaponTubeExclusiveFor(int index, EMissileWeapons type)
{
    if (index < 0 || index >= max_weapon_tubes)
        return;
    weapon_tube[index].type_allowed_mask = (1 << type);
}

void ShipTemplate::setTubeDirection(int index, float direction)
{
    if (index < 0 || index >= max_weapon_tubes)
        return;
    weapon_tube[index].direction = direction;
}

void ShipTemplate::setTubeSize(int index, EMissileSizes size)
{
    if (index < 0 || index >= max_weapon_tubes)
        return;
    weapon_tube[index].size = size;
}

void ShipTemplate::setType(TemplateType type)
{
    if (radar_trace == "RadarArrow.png" && type == Station)
    {
        radar_trace = "RadarBlip.png";
    }
    if (type == Station)
        repair_docked = true;
    this->type = type;
}

void ShipTemplate::setName(string name)
{
    if (templateMap.find(name) != templateMap.end())
    {
        LOG(ERROR) << "Duplicate ship template definition: " << name;
    }

    templateMap[name] = this;
    if (name.startswith("Joueur "))
        name = name.substr(7);
    this->name = name;
    if (this->locale_name == "")
        this->locale_name = name;
}

void ShipTemplate::setLocaleName(string name)
{
    this->locale_name = name;
}

void ShipTemplate::setClass(string class_name, string sub_class_name)
{
    this->class_name = class_name;
    this->sub_class_name = sub_class_name;
}

void ShipTemplate::setBeam(int index, float arc, float direction, float range, float cycle_time, float damage)
{
    setBeamWeapon(index, arc, direction, range, cycle_time, damage);
}

void ShipTemplate::setBeamWeapon(int index, float arc, float direction, float range, float cycle_time, float damage)
{
    if (index < 0 || index > max_beam_weapons)
        return;
    beams[index].setDirection(direction);
    beams[index].setArc(arc);
    beams[index].setRange(range);
    beams[index].setCycleTime(cycle_time);
    beams[index].setDamage(damage);
}



void ShipTemplate::setBeamWeaponTurret(int index, float arc, float direction, float rotation_rate)
{
    if (index < 0 || index > max_beam_weapons)
        return;
    beams[index].setTurretArc(arc);
    beams[index].setTurretDirection(direction);
    beams[index].setTurretRotationRate(rotation_rate);
}

sf::Vector2i ShipTemplate::interiorSize()
{
    sf::Vector2i min_pos(1000, 1000);
    sf::Vector2i max_pos(0, 0);
    for(unsigned int n=0; n<rooms.size(); n++)
    {
        min_pos.x = std::min(min_pos.x, rooms[n].position.x);
        min_pos.y = std::min(min_pos.y, rooms[n].position.y);
        max_pos.x = std::max(max_pos.x, rooms[n].position.x + rooms[n].size.x);
        max_pos.y = std::max(max_pos.y, rooms[n].position.y + rooms[n].size.y);
    }
    if (min_pos != sf::Vector2i(1, 1))
    {
        sf::Vector2i offset = sf::Vector2i(1, 1) - min_pos;
        for(unsigned int n=0; n<rooms.size(); n++)
            rooms[n].position += offset;
        for(unsigned int n=0; n<doors.size(); n++)
            doors[n].position += offset;
        max_pos += offset;
    }
    max_pos += sf::Vector2i(1, 1);
    return max_pos;
}

ESystem ShipTemplate::getSystemAtRoom(sf::Vector2i position)
{
    for(unsigned int n=0; n<rooms.size(); n++)
    {
        if (rooms[n].position.x <= position.x && rooms[n].position.x + rooms[n].size.x > position.x && rooms[n].position.y <= position.y && rooms[n].position.y + rooms[n].size.y > position.y)
            return rooms[n].system;
    }
    return SYS_None;
}

void ShipTemplate::setCollisionData(P<SpaceObject> object)
{
    model_data->setCollisionData(object);
}

void ShipTemplate::setShields(std::vector<float> values)
{
    shield_count = std::min(max_shield_count, int(values.size()));
    for(int n=0; n<shield_count; n++)
    {
        shield_level[n] = values[n];
    }
}

P<ShipTemplate> ShipTemplate::getTemplate(string name)
{
    if (templateMap.find(name) == templateMap.end())
    {
        LOG(ERROR) << "Failed to find ship template: " << name;
        return nullptr;
    }
    return templateMap[name];
}

std::vector<string> ShipTemplate::getAllTemplateNames()
{
    std::vector<string> ret;
    for(std::unordered_map<string, P<ShipTemplate> >::iterator i = templateMap.begin(); i != templateMap.end(); i++)
        ret.push_back(i->first);
    return ret;
}

std::vector<string> ShipTemplate::getTemplateNameList(TemplateType type)
{
    std::vector<string> ret;
    for(std::unordered_map<string, P<ShipTemplate> >::iterator i = templateMap.begin(); i != templateMap.end(); i++)
        if (i->second->getType() == type)
            ret.push_back(i->first);
    return ret;
}

std::vector<string> ShipTemplate::getTemplateClassList(TemplateType type)
{
    std::vector<string> ret;
    std::vector<string>::iterator ip;
    for(std::unordered_map<string, P<ShipTemplate> >::iterator i = templateMap.begin(); i != templateMap.end(); i++)
        if (i->second->getType() == type)
            ret.push_back(i->second->getClass());
    std::sort(ret.begin(), ret.end());
    auto last = std::unique(ret.begin(), ret.end());
    ret.erase(last, ret.end());
    return ret;
}

string getSystemName(ESystem system)
{
    switch(system)
    {
    case SYS_Reactor: return "Reacteur";
    case SYS_Cloaking: return "Boldgen Occulteur alpha";
    case SYS_BeamWeapons: return "Faisceau laser";
    case SYS_MissileSystem: return "Systeme de missiles";
    case SYS_Maneuver: return "Manoeuvres";
    case SYS_Impulse: return "Moteur subluminique";
    case SYS_Warp: return "Moteur WARP";
    case SYS_JumpDrive: return "Moteur JUMP";
    case SYS_FrontShield: return "Generateur Bouclier";
    case SYS_RearShield: return "Generateur Bouclier Ar.";
    case SYS_Docks: return "Gestion du Cargo";
    case SYS_Drones: return "Drones et radar";
    case SYS_Door: return "Sas exterieur";
    default:
        return "UNKNOWN";
    }
}

string getLocaleSystemName(ESystem system)
{
    switch(system)
    {
    case SYS_Reactor: return tr("system", "Reactor");
    case SYS_BeamWeapons: return tr("system", "Beam Weapons");
    case SYS_MissileSystem: return tr("system", "Missile System");
    case SYS_Maneuver: return tr("system", "Maneuvering");
    case SYS_Impulse: return tr("system", "Impulse Engines");
    case SYS_Warp: return tr("system", "Warp Drive");
    case SYS_JumpDrive: return tr("system", "Jump Drive");
    case SYS_FrontShield: return tr("system", "Front Shield Generator");
    case SYS_RearShield: return tr("system", "Rear Shield Generator");
    default:
        return "UNKNOWN";
    }
}

void ShipTemplate::setDescription(string description)
{
    this->description = description;
}

void ShipTemplate::setModel(string model_name)
{
    this->model_data = ModelData::getModel(model_name);
}

void ShipTemplate::setDefaultAI(string default_ai_name)
{
    this->default_ai_name = default_ai_name;
}

void ShipTemplate::setDockClasses(std::vector<string> classes)
{
    can_be_docked_by_class = std::unordered_set<string>(classes.begin(), classes.end());
}

void ShipTemplate::setSpeed(float impulse, float turn, float acceleration)
{
    impulse_speed = impulse;
    turn_speed = turn;
    impulse_acceleration = acceleration;
}

void ShipTemplate::setCombatManeuver(float boost, float strafe)
{
    combat_maneuver_boost_speed = boost;
    combat_maneuver_strafe_speed = strafe;
}

void ShipTemplate::setWarpSpeed(float warp)
{
    warp_speed = warp;
}

void ShipTemplate::setSharesEnergyWithDocked(bool enabled)
{
    shares_energy_with_docked = enabled;
}

void ShipTemplate::setRepairDocked(bool enabled)
{
    repair_docked = enabled;
}

void ShipTemplate::setReactor(bool enabled)
{
    has_reactor = enabled;
}

void ShipTemplate::setRestocksScanProbes(bool enabled)
{
    restocks_scan_probes = enabled;
}

void ShipTemplate::setRestocksMissilesDocked(bool enabled)
{
    restocks_missiles_docked = enabled;
}

void ShipTemplate::setJumpDrive(bool enabled)
{
    has_jump_drive = enabled;
}

void ShipTemplate::setCloaking(bool enabled)
{
    has_cloaking = enabled;
}

void ShipTemplate::setWeaponStorage(EMissileWeapons weapon, int amount)
{
    if (weapon != MW_None)
    {
        weapon_storage[weapon] = amount;
    }
}

void ShipTemplate::setCustomWeaponStorage(string weapon, int amount)
{
   custom_weapon_storage[weapon] = amount;
}


void ShipTemplate::setCustomWeapon(EMissileWeapons base, string weapon_name, float damage_multiplier, float speed, int fire_count, EDamageType dt)
{
    CustomMissileWeaponRegistry::createMissileWeapon(base, weapon_name, damage_multiplier, speed, fire_count, dt);

}

void ShipTemplate::setCustomWeaponColor(string weapon_name, char color_r, char color_g, char color_b)
{
    CustomMissileWeaponRegistry::getMissileWeapon(weapon_name).color = sf::Color(color_r, color_g, color_b);
}

void ShipTemplate::addRoom(sf::Vector2i position, sf::Vector2i size)
{
    rooms.push_back(ShipRoomTemplate(position, size, SYS_None));
}

void ShipTemplate::addRoomSystem(sf::Vector2i position, sf::Vector2i size, ESystem system)
{
    rooms.push_back(ShipRoomTemplate(position, size, system));
}

void ShipTemplate::addDoor(sf::Vector2i position, bool horizontal)
{
    doors.push_back(ShipDoorTemplate(position, horizontal));
}

void ShipTemplate::addDrones(string template_name, int count)
{
    drones.push_back(DroneTemplate(template_name, count));
}

void ShipTemplate::setDocks(int launchers, int energy, int weapons, int thermic, int repair, int stock){
    launcher_dock_count = launchers;
    energy_dock_count = energy;
    weapons_dock_count = weapons;
    thermic_dock_count = thermic;
    repair_dock_count = repair;
    stock_dock_count = stock;
}

void ShipTemplate::setRadarTrace(string trace)
{
    radar_trace = trace;
}

void ShipTemplate::setLongRangeRadarRange(float range)
{
    range = std::max(range, 100.0f);
    long_range_radar_range = range;
    short_range_radar_range = std::min(short_range_radar_range, range);
}

void ShipTemplate::setShortRangeRadarRange(float range)
{
    range = std::max(range, 100.0f);
    short_range_radar_range = range;
    long_range_radar_range = std::max(long_range_radar_range, range);
}

void ShipTemplate::setImpulseSoundFile(string sound)
{
    impulse_sound_file = sound;
}

P<ShipTemplate> ShipTemplate::copy(string new_name)
{
    P<ShipTemplate> result = new ShipTemplate();
    result->setName(new_name);

    result->locale_name = locale_name;
    result->description = description;
    result->class_name = class_name;
    result->sub_class_name = sub_class_name;
    result->type = type;
    result->model_data = model_data;
    result->hack_diff= hack_diff;
    result->os_name = os_name;

    result->can_be_docked_by_class = can_be_docked_by_class;
    result->energy_storage_amount = energy_storage_amount;
    result->repair_crew_count = repair_crew_count;

    result->can_scan = can_scan;
    result->can_hack = can_hack;
    result->can_dock = can_dock;
    result->can_combat_maneuver = can_combat_maneuver;
    result->can_self_destruct = can_self_destruct;
    result->can_launch_probe = can_launch_probe;

    result->default_ai_name = default_ai_name;
    for(int n=0; n<max_beam_weapons; n++)
        result->beams[n] = beams[n];
    result->weapon_tube_count = weapon_tube_count;
//    for(int n=0; n<max_beam_weapons; n++)
    for(int n=0; n<weapon_tube_count; n++)
        result->weapon_tube[n] = weapon_tube[n];
    result->hull = hull;
    result->shield_count = shield_count;
    for(int n=0; n<max_shield_count; n++)
        result->shield_level[n] = shield_level[n];
    result->impulse_speed = impulse_speed;
    result->turn_speed = turn_speed;
    result->warp_speed = warp_speed;
    result->impulse_acceleration = impulse_acceleration;
    result->combat_maneuver_boost_speed = combat_maneuver_boost_speed;
    result->combat_maneuver_strafe_speed = combat_maneuver_strafe_speed;
    result->shares_energy_with_docked = shares_energy_with_docked;
    result->repair_docked = repair_docked;
    result->restocks_scan_probes = restocks_scan_probes;
    result->restocks_missiles_docked = restocks_missiles_docked;
    result->has_jump_drive = has_jump_drive;
    result->has_reactor = has_reactor;
    result->jump_drive_min_distance = jump_drive_min_distance;
    result->jump_drive_max_distance = jump_drive_max_distance;
    result->jump_drive_charge_time = jump_drive_charge_time;
    result->jump_drive_energy_per_km_charge = jump_drive_energy_per_km_charge;
    result->has_cloaking = has_cloaking;
    for(int n=0; n<MW_Count; n++)
        result->weapon_storage[n] = weapon_storage[n];
    result->custom_weapon_storage = custom_weapon_storage;
    result->system_damage_ratio = system_damage_ratio;
    result->system_damage_hull_threshold = system_damage_hull_threshold;
    result->radar_trace = radar_trace;

    result->rooms = rooms;
    result->doors = doors;
    result->drones = drones;
    result->launcher_dock_count = launcher_dock_count;
    result->energy_dock_count = energy_dock_count;
    result->weapons_dock_count = weapons_dock_count;
    result->thermic_dock_count = thermic_dock_count;
    result->repair_dock_count = repair_dock_count;
    result->stock_dock_count = stock_dock_count;
    return result;
}

void ShipTemplate::setEnergyStorage(float energy_amount)
{
    this->energy_storage_amount = energy_amount;
}

void ShipTemplate::setRepairCrewCount(int amount)
{
    this->repair_crew_count = amount;
}

string ShipTemplate::getName()
{
    return this->name;
}

string ShipTemplate::getLocaleName()
{
    return this->locale_name;
}

string ShipTemplate::getDescription()
{
    return this->description;
}

string ShipTemplate::getClass()
{
    return this->class_name;
}

string ShipTemplate::getSubClass()
{
    return this->sub_class_name;
}

ShipTemplate::TemplateType ShipTemplate::getType()
{
    return type;
}

#ifndef _MSC_VER
#include "shipTemplate.hpp"
#endif /* _MSC_VER */
