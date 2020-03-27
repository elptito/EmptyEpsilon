#include <i18n.h>
#include "spaceship.h"
#include "mesh.h"
#include "shipTemplate.h"
#include "playerInfo.h"
#include "spaceObjects/beamEffect.h"
#include "factionInfo.h"
#include "spaceObjects/explosionEffect.h"
#include "electricExplosionEffect.h"
#include "particleEffect.h"
#include "spaceObjects/warpJammer.h"
#include "gameGlobalInfo.h"
#include "shipCargo.h"
#include "gui/colorConfig.h"

#include "scriptInterface.h"

REGISTER_SCRIPT_SUBCLASS_NO_CREATE(SpaceShip, ShipTemplateBasedObject)
{
    //[DEPRECATED]
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, isFriendOrFoeIdentified);
    //[DEPRECATED]
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, isFullyScanned);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, isFriendOrFoeIdentifiedBy);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, isFullyScannedBy);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, isFriendOrFoeIdentifiedByFaction);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, isFullyScannedByFaction);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, isDocked);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, isDockedWith);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, getTarget);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, getDockTarget);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, getWeaponStorage);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, getWeaponStorageMax);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, setWeaponStorage);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, setWeaponStorageMax);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, getCustomWeaponStorage);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, getCustomWeaponStorageMax);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, setCustomWeaponStorage);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, setCustomWeaponStorageMax);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, getShieldsFrequency);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, setShieldsFrequency);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, getOxygenRechargeRate);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, setPassagersCount);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, getPassagersCount);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, setMaxPassagersCount);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, getMaxPassagersCount);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, getBeamFrequency);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, getMaxEnergy);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, setMaxEnergy);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, getEnergy);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, setEnergy);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, hasSystem);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, getSystemHealth);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, setSystemHealth);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, getSystemHealthMax);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, setSystemHealthMax);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, getSystemHeat);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, setSystemHeat);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, getSystemPower);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, setSystemPower);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, getSystemCoolant);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, setSystemCoolant);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, getSystemHack);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, setSystemHack);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, getSystemEffectiveness);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, getImpulseMaxSpeed);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, setImpulseMaxSpeed);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, getRotationMaxSpeed);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, setRotationMaxSpeed);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, setCombatManeuver);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, hasReactor);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, setReactor);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, hasCloaking);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, setCloaking);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, hasJumpDrive);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, setJumpDrive);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, setJumpDriveRange);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, setJumpDriveChargeTime);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, setJumpDriveEnergy);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, getJumpDelay);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, getJumpDriveMaxDistance);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, getJumpDriveMinDistance);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, getJumpDriveCharge);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, getJumpDriveChargeTime);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, getJumpDriveEnergy);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, hasWarpDrive);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, setWarpDrive);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, getBeamWeaponArc);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, getBeamWeaponDirection);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, getBeamWeaponRange);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, getBeamWeaponTurretArc);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, getBeamWeaponTurretDirection);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, getBeamWeaponCycleTime);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, getBeamWeaponDamage);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, getBeamWeaponEnergyPerFire);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, getBeamWeaponHeatPerFire);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, setBeamWeapon);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, setBeamWeaponTurret);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, setBeamWeaponTexture);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, setBeamWeaponEnergyPerFire);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, setBeamWeaponHeatPerFire);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, setWeaponTubeCount);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, getWeaponTubeCount);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, getWeaponTubeLoadType);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, weaponTubeAllowMissle);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, weaponTubeDisallowMissle);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, setWeaponTubeExclusiveFor);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, setWeaponTubeDirection);
    /// Set the icon to be used for this ship on the radar.
    /// For example, ship:setRadarTrace("RadarBlip.png") will show a dot instead of an arrow for this ship.
    /// Note: Icon is only shown after scanning, before the ship is scanned it is always shown as an arrow.
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, setRadarTrace);
    /// Get the dynamic radar signature values for each component band.
    /// Returns a float.
    /// Example: obj:getDynamicRadarSignatureGravity()
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, getDynamicRadarSignatureGravity);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, getDynamicRadarSignatureElectrical);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, getDynamicRadarSignatureBiological);
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, addBroadcast);
    /// Set the scan state of this ship for every faction.
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, setScanState);
    /// Set the scane state of this ship for a particular faction.
    REGISTER_SCRIPT_CLASS_FUNCTION(SpaceShip, setScanStateByFaction);
}

SpaceShip::SpaceShip(string multiplayerClassName, float multiplayer_significant_range)
: ShipTemplateBasedObject(50, multiplayerClassName, multiplayer_significant_range)
{
    setCollisionPhysics(true, false);

    target_rotation = getRotation();
    impulse_request = 0;
    current_impulse = 0;
    has_warp_drive = true;
    warp_request = 0.0;
    current_warp = 0.0;
    has_jump_drive = true;
    has_reactor = true;
    has_cloaking = false;
    jump_drive_min_distance = 5000.0;
    jump_drive_max_distance = 50000.0;
    jump_drive_charge = jump_drive_max_distance;
    jump_drive_charge_time = 90.0;
    jump_drive_energy_per_km_charge = 4.0f;
    jump_distance = 0.0;
    jump_delay = 0.0;
    wormhole_alpha = 0.0;
    weapon_tube_count = 0;
    beam_weapons_count = 0;
    turn_speed = 10.0;
    impulse_max_speed = 600.0;
    warp_speed_per_warp_level = 1000.0;
    combat_maneuver_charge = 1.0;
    combat_maneuver_boost_request = 0.0;
    combat_maneuver_boost_active = 0.0;
    combat_maneuver_strafe_request = 0.0;
    combat_maneuver_strafe_active = 0.0;
    combat_maneuver_boost_speed = 0.0f;
    combat_maneuver_strafe_speed = 0.0f;
    target_id = -1;
    dock_target_id = -1;
    beam_frequency = irandom(0, max_frequency);
    beam_system_target = SYS_None;
    shield_frequency = irandom(0, max_frequency);
    docking_state = DS_NotDocking;
    landing_state = LS_NotLanding;
    impulse_acceleration = 20.0;
    energy_level = 1000;
    max_energy_level = 1000;
    turnSpeed = 0.0f;
    passagers_count = 6;
    max_passagers_count = 20;

    registerMemberReplication(&target_rotation, 1.5);
    registerMemberReplication(&turnSpeed, 0.1);
    registerMemberReplication(&impulse_request, 0.1);
    registerMemberReplication(&current_impulse, 0.5);
    registerMemberReplication(&has_warp_drive);
    registerMemberReplication(&warp_request, 0.1);
    registerMemberReplication(&current_warp, 0.1);
    registerMemberReplication(&has_jump_drive);
    registerMemberReplication(&jump_drive_charge, 0.5);
    registerMemberReplication(&jump_delay, 0.5);
    registerMemberReplication(&jump_drive_min_distance);
    registerMemberReplication(&jump_drive_max_distance);
    registerMemberReplication(&jump_drive_charge_time);
    registerMemberReplication(&jump_drive_energy_per_km_charge);
    registerMemberReplication(&has_reactor);
    registerMemberReplication(&has_cloaking);
    registerMemberReplication(&wormhole_alpha, 0.5);
    registerMemberReplication(&weapon_tube_count);
    registerMemberReplication(&beam_weapons_count);
    registerMemberReplication(&target_id);
    registerMemberReplication(&dock_target_id);
    registerMemberReplication(&landing_target_id);
    registerMemberReplication(&turn_speed);
    registerMemberReplication(&impulse_max_speed);
    registerMemberReplication(&impulse_acceleration);
    registerMemberReplication(&warp_speed_per_warp_level);
    registerMemberReplication(&shield_frequency);
    registerMemberReplication(&docking_state);
    registerMemberReplication(&beam_frequency);
    registerMemberReplication(&combat_maneuver_charge, 0.5);
    registerMemberReplication(&combat_maneuver_boost_request);
    registerMemberReplication(&combat_maneuver_boost_active, 0.2);
    registerMemberReplication(&combat_maneuver_strafe_request);
    registerMemberReplication(&combat_maneuver_strafe_active, 0.2);
    registerMemberReplication(&combat_maneuver_boost_speed);
    registerMemberReplication(&combat_maneuver_strafe_speed);
    registerMemberReplication(&radar_trace);
    registerMemberReplication(&passagers_count);
    registerMemberReplication(&max_passagers_count);


    // Initialize each subsystem to be powered with no coolant or heat.
    for(int n=0; n<SYS_COUNT; n++)
    {
        systems[n].health = 1.0;
        systems[n].health_max = 1.0;
        systems[n].power_level = 1.0;
        systems[n].power_request = 1.0;
        systems[n].coolant_level = 0.0;
        systems[n].coolant_request = 0.0;
        systems[n].heat_level = 0.0;
        systems[n].hacked_level = 0.0;
        systems[n].coolant_max = 1.0;

        if (n == SYS_Cloaking)
        {
            systems[n].power_level = 0.0;
            systems[n].power_request = 0.0;
        }

        registerMemberReplication(&systems[n].health);
        registerMemberReplication(&systems[n].health_max);
        registerMemberReplication(&systems[n].power_level);
        registerMemberReplication(&systems[n].power_request);
        registerMemberReplication(&systems[n].coolant_level);
        registerMemberReplication(&systems[n].coolant_request);
        registerMemberReplication(&systems[n].heat_level);
        registerMemberReplication(&systems[n].hacked_level);
        registerMemberReplication(&systems[n].coolant_max);
    }

    for(int n = 0; n < max_beam_weapons; n++)
    {
        beam_weapons[n].setParent(this);
    }

    for(int n = 0; n < max_weapon_tubes; n++)
    {
        weapon_tube[n].setParent(this);
        weapon_tube[n].setIndex(n);
    }

    for(int n = 0; n < MW_Count; n++)
    {
        weapon_storage[n] = 0;
        weapon_storage_max[n] = 0;
        registerMemberReplication(&weapon_storage[n]);
        registerMemberReplication(&weapon_storage_max[n]);
    }

    //for(auto &kv : custom_weapon_storage_max)
    for(auto &kv : CustomMissileWeaponRegistry::getCustomMissileWeapons())
    {
        custom_weapon_storage[kv.first] = 0;
        custom_weapon_storage_max[kv.first] = 0;
        registerMemberReplication(&custom_weapon_storage[kv.first]);
        registerMemberReplication(&custom_weapon_storage_max[kv.first]);
    }


    for(int n = 0; n < max_docks_count; n++)
    {
        docks[n].setParent(this);
        docks[n].setIndex(n);
    }
    scanning_complexity_value = -1;
    scanning_depth_value = -1;

	registerMemberReplication(&scanning_complexity_value);
	registerMemberReplication(&scanning_depth_value);

    // Ships can have dynamic signatures. Initialize a default baseline value
    // from which clients derive the dynamic signature on update.
    setRadarSignatureInfo(0.05, 0.2, 0.2);

    if (game_server)
        setCallSign(gameGlobalInfo->getNextShipCallsign());
}

SpaceShip::~SpaceShip()
{
}

void SpaceShip::applyTemplateValues()
{
    for(int n=0; n<max_beam_weapons; n++)
    {
        beam_weapons[n].setPosition(ship_template->model_data->getBeamPosition(n));
        beam_weapons[n].setArc(ship_template->beams[n].getArc());
        beam_weapons[n].setDirection(ship_template->beams[n].getDirection());
        beam_weapons[n].setRange(ship_template->beams[n].getRange());
        beam_weapons[n].setTurretArc(ship_template->beams[n].getTurretArc());
        beam_weapons[n].setTurretDirection(ship_template->beams[n].getTurretDirection());
        beam_weapons[n].setTurretRotationRate(ship_template->beams[n].getTurretRotationRate());
        beam_weapons[n].setCycleTime(ship_template->beams[n].getCycleTime());
        beam_weapons[n].setDamage(ship_template->beams[n].getDamage());
        beam_weapons[n].setBeamTexture(ship_template->beams[n].getBeamTexture());
        beam_weapons[n].setEnergyPerFire(ship_template->beams[n].getEnergyPerFire());
        beam_weapons[n].setHeatPerFire(ship_template->beams[n].getHeatPerFire());
        beam_weapons[n].setIsValid(true);
    }
    weapon_tube_count = ship_template->weapon_tube_count;
    energy_level = max_energy_level = ship_template->energy_storage_amount;

    impulse_max_speed = ship_template->impulse_speed;
    impulse_acceleration = ship_template->impulse_acceleration;
    turn_speed = ship_template->turn_speed;
    combat_maneuver_boost_speed = ship_template->combat_maneuver_boost_speed;
    combat_maneuver_strafe_speed = ship_template->combat_maneuver_strafe_speed;
    has_warp_drive = ship_template->warp_speed > 0.0;
    warp_speed_per_warp_level = ship_template->warp_speed;
    has_jump_drive = ship_template->has_jump_drive;
    has_reactor = ship_template->has_reactor;
    has_cloaking = ship_template->has_cloaking;
    jump_drive_min_distance = ship_template->jump_drive_min_distance;
    jump_drive_max_distance = ship_template->jump_drive_max_distance;
    jump_drive_charge = ship_template->jump_drive_charge;
    jump_drive_charge_time = ship_template->jump_drive_charge_time;
    jump_drive_energy_per_km_charge = ship_template->jump_drive_energy_per_km_charge;
    for(int n=0; n<max_weapon_tubes; n++)
    {
        weapon_tube[n].setLoadTimeConfig(ship_template->weapon_tube[n].load_time);
        weapon_tube[n].setDirection(ship_template->weapon_tube[n].direction);
        weapon_tube[n].setSize(ship_template->weapon_tube[n].size);
        for(int m=0; m<MW_Count; m++)
        {
            if (ship_template->weapon_tube[n].type_allowed_mask & (1 << m))
                weapon_tube[n].allowLoadOf(EMissileWeapons(m));
            else
                weapon_tube[n].disallowLoadOf(EMissileWeapons(m));
        }
    }
    for(int n=0; n<MW_Count; n++)
        weapon_storage[n] = weapon_storage_max[n] = ship_template->weapon_storage[n];

    for(auto& kv : ship_template->custom_weapon_storage)
    {
        custom_weapon_storage[kv.first] = kv.second;
        custom_weapon_storage_max[kv.first] = kv.second;
    }

    ship_template->setCollisionData(this);
    model_info.setData(ship_template->model_data);

    int droneIdx = 0;
    for (int i = 0; droneIdx < max_docks_count && i < ship_template->launcher_dock_count; i++, droneIdx++)
        docks[droneIdx].setDockType(Dock_Launcher);
    for (int i = 0; droneIdx < max_docks_count && i < ship_template->energy_dock_count; i++, droneIdx++)
        docks[droneIdx].setDockType(Dock_Energy);
    for (int i = 0; droneIdx < max_docks_count && i < ship_template->weapons_dock_count; i++, droneIdx++)
        docks[droneIdx].setDockType(Dock_Weapons);
    for (int i = 0; droneIdx < max_docks_count && i < ship_template->thermic_dock_count; i++, droneIdx++)
        docks[droneIdx].setDockType(Dock_Thermic);
    for (int i = 0; droneIdx < max_docks_count && i < ship_template->repair_dock_count; i++, droneIdx++)
        //docks[droneIdx].setDockType(Dock_Repair);
        docks[droneIdx].setDockType(Dock_Maintenance);
    for (int i = 0; droneIdx < max_docks_count && i < ship_template->stock_dock_count; i++, droneIdx++)
        docks[droneIdx].setDockType(Dock_Stock);

    int maxActiveDockIndex = droneIdx;
    for (; droneIdx < max_docks_count; droneIdx++)
        docks[droneIdx].setDockType(Dock_Disabled);

    for (auto &droneTemplate : ship_template->drones) // access by reference to avoid copying
    {
        P<ShipTemplate> drone_ship_template = ShipTemplate::getTemplate(droneTemplate.template_name);
        // add drones one by one, assuming all drones are empty, and template is of drone type
        for (int i = 0; i < droneTemplate.count; i++)
        {
            Dock *dock = Dock::findOpenForDocking(docks, maxActiveDockIndex);
            if (!dock) { // no more available docks
                LOG(ERROR) << "Too many drones: " << template_name;
                break;
            }
            P<ShipCargo> cargo = new ShipCargo(drone_ship_template);
            dock->dock(cargo);
        }
    }
}

#if FEATURE_3D_RENDERING
void SpaceShip::draw3DTransparent()
{
    if (!ship_template) return;
    ShipTemplateBasedObject::draw3DTransparent();

    if ((has_jump_drive && jump_delay > 0.0f) ||
        (wormhole_alpha > 0.0f))
    {
        float delay = jump_delay;
        if (wormhole_alpha > 0.0f)
            delay = wormhole_alpha;
        float alpha = 1.0f - (delay / 10.0f);
        model_info.renderOverlay(textureManager.getTexture("electric_sphere_texture.png"), alpha);
    }
}
#endif//FEATURE_3D_RENDERING

RawRadarSignatureInfo SpaceShip::getDynamicRadarSignatureInfo()
{
    // Adjust radar_signature dynamically based on current state and activity.
    // radar_signature becomes the ship's baseline radar signature.
    RawRadarSignatureInfo signature_delta;

    // For each ship system ...
    for(int n = 0; n < SYS_COUNT; n++)
    {
        ESystem ship_system = static_cast<ESystem>(n);

        // ... increase the biological band based on system heat, offset by
        // coolant.
        signature_delta.biological += std::max(
            0.0f,
            std::min(
                1.0f,
                getSystemHeat(ship_system) - (getSystemCoolant(ship_system) / 10.0f)
            )
        );

        // ... adjust the electrical band if system power allocation is not
        // 100%.
        if (ship_system == SYS_JumpDrive && jump_drive_charge < jump_drive_max_distance)
        {
            // ... elevate electrical after a jump, since recharging jump
            // consumes energy.
            signature_delta.electrical += std::max(
                0.0f,
                std::min(
                    1.0f,
                    getSystemPower(ship_system) * (jump_drive_charge + 0.01f / jump_drive_max_distance)
                )
            );
        } else if (getSystemPower(ship_system) != 1.0f)
        {
            // For non-Jump systems, allow underpowered systems to reduce the
            // total electrical signal output.
            signature_delta.electrical += std::max(
                -1.0f,
                std::min(
                    1.0f,
                    getSystemPower(ship_system) - 1.0f
                )
            );
        }
    }

    // Increase the gravitational band if the ship is about to jump, or is
    // actively warping.
    if (jump_delay > 0.0f)
    {
        signature_delta.gravity += std::max(
            0.0f,
            std::min(
                (1.0f / jump_delay + 0.01f) + 0.25f,
                10.0f
            )
        );
    } else if (current_warp > 0.0f)
    {
        signature_delta.gravity += current_warp;
    }

    // Update the signature by adding the delta to its baseline.
    RawRadarSignatureInfo info = getRadarSignatureInfo();
    info += signature_delta;
    return info;
}

void SpaceShip::drawOnRadar(sf::RenderTarget& window, sf::Vector2f position, float scale, float rotation, bool long_range)
{
    // Draw beam arcs on short-range radar only, and only for fully scanned
    // ships.
    if (!long_range && (!my_spaceship || (getScannedStateFor(my_spaceship) == SS_FullScan)))
    {
        // For each beam ...
        for(int n = 0; n < max_beam_weapons; n++)
        {
            // Draw beam arcs only if the beam has a range. A beam with range 0
            // effectively doesn't exist; exit if that's the case.
            if (beam_weapons[n].getRange() == 0.0) continue;

            // Color beam arcs red.
            // TODO: Make this color configurable.
            sf::Color color = sf::Color::Red;

            // If the beam is cooling down, flash and fade the arc color.
            if (beam_weapons[n].getCooldown() > 0)
                color = sf::Color(255, 255 * (beam_weapons[n].getCooldown() / beam_weapons[n].getCycleTime()), 0);

            // Initialize variables from the beam's data.
            float beam_direction = beam_weapons[n].getDirection();
            float beam_arc = beam_weapons[n].getArc();
            float beam_range = beam_weapons[n].getRange();

            // Set the beam's origin on radar to its relative position on the
            // mesh.
            sf::Vector2f beam_offset = sf::rotateVector(ship_template->model_data->getBeamPosition2D(n) * scale, getRotation()-rotation);

            // Configure an array to hold each point of the arc. Each point in
            // the array draws a line to the next point. If the color between
            // points is different, it's drawn as a gradient from the origin
            // point's color to the destination point's.
            sf::VertexArray a(sf::LinesStrip, 3);
            a[0].color = color;
            a[1].color = color;
            a[2].color = sf::Color(color.r, color.g, color.b, 0);

            // Drop the pen onto the beam's origin.
            a[0].position = beam_offset + position;

            // Draw the beam's left bound.
            a[1].position = beam_offset + position + sf::vector2FromAngle(getRotation()-rotation + (beam_direction + beam_arc / 2.0f)) * beam_range * scale;
            a[2].position = beam_offset + position + sf::vector2FromAngle(getRotation()-rotation + (beam_direction + beam_arc / 2.0f)) * beam_range * scale * 1.3f;
            window.draw(a);

            // Draw the beam's right bound.
            a[1].position = beam_offset + position + sf::vector2FromAngle(getRotation()-rotation + (beam_direction - beam_arc / 2.0f)) * beam_range * scale;
            a[2].position = beam_offset + position + sf::vector2FromAngle(getRotation()-rotation + (beam_direction - beam_arc / 2.0f)) * beam_range * scale * 1.3f;
            window.draw(a);

            // Draw the beam's arc.
            int arcPoints = int(beam_arc / 10) + 1;
            sf::VertexArray arc_line(sf::LinesStrip, arcPoints);
            for(int i=0; i<arcPoints; i++)
            {
                arc_line[i].color = color;
                arc_line[i].position = beam_offset + position + sf::vector2FromAngle(getRotation()-rotation + (beam_direction - beam_arc / 2.0f + 10 * i)) * beam_range * scale;
            }
            arc_line[arcPoints-1].position = beam_offset + position + sf::vector2FromAngle(getRotation()-rotation + (beam_direction + beam_arc / 2.0f)) * beam_range * scale;
            window.draw(arc_line);

            // If the beam is turreted, draw the turret's arc. Otherwise, exit.
            if (beam_weapons[n].getTurretArc() == 0.0) continue;

            // Initialize variables from the turret data.
            float turret_arc = beam_weapons[n].getTurretArc();
            float turret_direction = beam_weapons[n].getTurretDirection();

            // Draw the turret's bounds, at half the transparency of the beam's.
            // TODO: Make this color configurable.
            a[0].color = sf::Color(color.r, color.g, color.b, color.a / 2);
            a[1].color = sf::Color(color.r, color.g, color.b, color.a / 2);

            // Drawn Bounds only if beam arc diff than 360
            if (turret_arc < 360.0)
            {
                // Draw the turret's left bound. (We're reusing the beam's origin.)
                a[1].position = beam_offset + position + sf::vector2FromAngle(getRotation()-rotation + (turret_direction + turret_arc / 2.0f)) * beam_range * scale;
                a[2].position = beam_offset + position + sf::vector2FromAngle(getRotation()-rotation + (turret_direction + turret_arc / 2.0f)) * beam_range * scale * 1.3f;
                window.draw(a);

                // Draw the turret's right bound.
                a[1].position = beam_offset + position + sf::vector2FromAngle(getRotation()-rotation + (turret_direction - turret_arc / 2.0f)) * beam_range * scale;
                a[2].position = beam_offset + position + sf::vector2FromAngle(getRotation()-rotation + (turret_direction - turret_arc / 2.0f)) * beam_range * scale * 1.3f;
                window.draw(a);
            }

            // Draw the turret's arc.
            int turret_points = int(turret_arc / 10) + 1;
            sf::VertexArray turret_line(sf::LinesStrip, turret_points);
            for(int i = 0; i < turret_points; i++)
            {
                turret_line[i].color = sf::Color(color.r, color.g, color.b, color.a / 2);
                turret_line[i].position = beam_offset + position + sf::vector2FromAngle(getRotation()-rotation + (turret_direction - turret_arc / 2.0f + 10 * i)) * beam_range * scale;
            }
            turret_line[turret_points-1].position = beam_offset + position + sf::vector2FromAngle(getRotation()-rotation + (turret_direction + turret_arc / 2.0f)) * beam_range * scale;
            window.draw(turret_line);
        }
    }
    // If not on long-range radar ...
    if (!long_range)
    {
        // ... and the ship being drawn is either not our ship or has been
        // scanned ...
        float scale_radius = 1.0;
        if (getRadius() > 1000)
            scale_radius = getRadius() * 2.0 / 1000.0;

        if (!my_spaceship || getScannedStateFor(my_spaceship) >= SS_SimpleScan)
        {
            // ... draw and show shield indicators on our radar.
            drawShieldsOnRadar(window, position, scale, scale_radius, true);
        } else {
            // Otherwise, draw the indicators, but don't show them.
            drawShieldsOnRadar(window, position, scale, scale_radius, false);
        }
    }

    // Set up the radar sprite for objects.
    sf::Sprite objectSprite;
    float sprite_scale = 0.1;
    float sprite_max = 10.0;
    float sprite_min = 0.5;

    // If the object is a ship that hasn't been scanned, draw the default icon.
    // Otherwise, draw the ship-specific icon.
    if (my_spaceship && (getScannedStateFor(my_spaceship) == SS_NotScanned || getScannedStateFor(my_spaceship) == SS_FriendOrFoeIdentified))
    {
		objectSprite.setColor(sf::Color(192, 192, 192));
        textureManager.setTexture(objectSprite, "RadarBlip.png");
    }
    else
    {
		objectSprite.setColor(factionInfo[getFactionId()]->gm_color);
        textureManager.setTexture(objectSprite, radar_trace);
        sprite_scale = std::max(sprite_min,std::min(sprite_max,scale * getRadius() * 2 / objectSprite.getTextureRect().width));
		if (long_range)
			sprite_scale = sprite_scale * 0.7;
    }

    objectSprite.setRotation(getRotation()-rotation);
    objectSprite.setPosition(position);
    objectSprite.setScale(sprite_scale, sprite_scale);
    window.draw(objectSprite);
}

void SpaceShip::drawOnGMRadar(sf::RenderTarget& window, sf::Vector2f position, float scale, float rotation, bool long_range)
{
    if (!long_range)
    {
        sf::RectangleShape bar(sf::Vector2f(60, 10));
        bar.setPosition(position.x - 30, position.y - 30);
        bar.setSize(sf::Vector2f(60 * hull_strength / hull_max, 5));
        bar.setFillColor(sf::Color(128, 255, 128, 128));
        window.draw(bar);
    }
}

void SpaceShip::update(float delta)
{
    ShipTemplateBasedObject::update(delta);

    if (game_server)
    {
        if (docking_state == DS_Docking)
        {
            if (energy_level == 0)
                energy_level += 5;
            if (!docking_target)
                docking_state = DS_NotDocking;
            else
                target_rotation = sf::vector2ToAngle(getPosition() - docking_target->getPosition());
            if (fabs(sf::angleDifference(target_rotation, getRotation())) < 10.0)
                impulse_request = -1.0;
            else
                impulse_request = 0.0;
        }

        if (landing_state == LS_Landing)
        {
            if (energy_level == 0)
                energy_level += 5;
            if (!landing_target)
                landing_state = LS_NotLanding;
            else
                target_rotation = sf::vector2ToAngle(getPosition() - landing_target->getPosition());
            if (fabs(sf::angleDifference(target_rotation, getRotation())) < 10.0)
                impulse_request = -1.0;
            else
                impulse_request = 0.0;
        }
        if (docking_state == DS_Docked)
        {
            if (!docking_target)
            {
                docking_state = DS_NotDocking;
            }else{
                setPosition(docking_target->getPosition() + sf::rotateVector(docking_offset, docking_target->getRotation()));
                target_rotation = sf::vector2ToAngle(getPosition() - docking_target->getPosition());

                P<ShipTemplateBasedObject> docked_with_template_based = docking_target;
                if (docked_with_template_based && docked_with_template_based->repair_docked)  //Check if what we are docked to allows hull repairs, and if so, do it.
                {
                    if (hull_strength < hull_max)
                    {
                        hull_strength += delta;
                        if (hull_strength > hull_max)
                            hull_strength = hull_max;
                    }
                }
            }
            impulse_request = 0.0;
        }
        if ((docking_state == DS_Docked) || (docking_state == DS_Docking))
            warp_request = 0.0;
        if (landing_state == LS_Landing)
            warp_request = 0.0;
    }

    float rotationDiff;
    if (fabs(turnSpeed) < 0.0005f) {
        rotationDiff = sf::angleDifference(getRotation(), target_rotation);
    } else {
        rotationDiff = turnSpeed;
    }

    if (rotationDiff > 1.0)
        setAngularVelocity(turn_speed * getSystemEffectiveness(SYS_Maneuver));
    else if (rotationDiff < -1.0)
        setAngularVelocity(-turn_speed * getSystemEffectiveness(SYS_Maneuver));
    else
        setAngularVelocity(rotationDiff * turn_speed * getSystemEffectiveness(SYS_Maneuver));

    if ((has_jump_drive && jump_delay > 0) || (has_warp_drive && warp_request > 0))
    {
        if (WarpJammer::isWarpJammed(getPosition()))
        {
            jump_delay = 0;
            warp_request = 0.0f;
        }
    }
    if (has_jump_drive && jump_delay > 0)
    {
        if (current_impulse > 0.0)
        {
            if (impulse_max_speed > 0)
                current_impulse -= delta * (impulse_acceleration / impulse_max_speed);
            if (current_impulse < 0.0)
                current_impulse = 0.0;
        }
        if (current_impulse < 0.0)
        {
            if (impulse_max_speed > 0)
                current_impulse += delta * (impulse_acceleration / impulse_max_speed);
            if (current_impulse > 0.0)
                current_impulse = 0.0;
        }
        if (current_warp > 0.0)
        {
            current_warp -= delta;
            if (current_warp < 0.0)
                current_warp = 0.0;
        }
        jump_delay -= delta * getSystemEffectiveness(SYS_JumpDrive);
        if (jump_delay <= 0.0)
        {
            executeJump(jump_distance);
            jump_delay = 0.0;
        }
    }else if (has_warp_drive && (warp_request > 0 || current_warp > 0))
    {
        if (current_impulse > 0.0)
        {
            if (impulse_max_speed > 0)
                current_impulse -= delta * (impulse_acceleration / impulse_max_speed);
            if (current_impulse < 0.0)
                current_impulse = 0.0;
        }else if (current_impulse > 0.0)
        {
            if (impulse_max_speed > 0)
                current_impulse += delta * (impulse_acceleration / impulse_max_speed);
            if (current_impulse > 0.0)
                current_impulse = 0.0;
        }else{
            if (current_warp < warp_request)
            {
                current_warp += delta / warp_charge_time;
                if (current_warp > warp_request)
                    current_warp = warp_request;
            }else if (current_warp > warp_request)
            {
                current_warp -= delta / warp_decharge_time;
                if (current_warp < warp_request)
                    current_warp = warp_request;
            }
        }
    }else{
        if (has_jump_drive)
        {
            float f = getJumpDriveRechargeRate() * getSystemEffectiveness(SYS_JumpDrive);
            if (f > 0)
            {
                if (jump_drive_charge < jump_drive_max_distance)
                {
                    float extra_charge = (delta / jump_drive_charge_time * jump_drive_max_distance) * f;
//                    if (useEnergy(extra_charge * jump_drive_energy_per_km_charge / 1000.0))
                    if (useEnergy(jump_drive_energy_per_km_charge / 1000.0))
                    {
                        jump_drive_charge += extra_charge;
                        if (jump_drive_charge >= jump_drive_max_distance)
                            jump_drive_charge = jump_drive_max_distance;
                    }
                }
            }else{
                jump_drive_charge += (delta / jump_drive_charge_time * jump_drive_max_distance) * f;
                if (jump_drive_charge < 0.0f)
                    jump_drive_charge = 0.0f;
            }
        }
        current_warp = 0.0;
        if (impulse_request > 1.0)
            impulse_request = 1.0;
        if (impulse_request < -1.0)
            impulse_request = -1.0;
        if (current_impulse < impulse_request)
        {
            if (impulse_max_speed > 0)
                current_impulse += delta * (impulse_acceleration / impulse_max_speed);
            if (current_impulse > impulse_request)
                current_impulse = impulse_request;
        }else if (current_impulse > impulse_request)
        {
            if (impulse_max_speed > 0)
                current_impulse -= delta * (impulse_acceleration / impulse_max_speed);
            if (current_impulse < impulse_request)
                current_impulse = impulse_request;
        }
    }

    // Add heat based on warp factor.
    addHeat(SYS_Warp, current_warp * delta * heat_per_warp);

    // Determine forward direction and velocity.
    sf::Vector2f forward = sf::vector2FromAngle(getRotation());
    setVelocity(forward * (current_impulse * impulse_max_speed * getSystemEffectiveness(SYS_Impulse) + current_warp * warp_speed_per_warp_level * getSystemEffectiveness(SYS_Warp)));

    if (combat_maneuver_boost_active > combat_maneuver_boost_request)
    {
        combat_maneuver_boost_active -= delta;
        if (combat_maneuver_boost_active < combat_maneuver_boost_request)
            combat_maneuver_boost_active = combat_maneuver_boost_request;
    }
    if (combat_maneuver_boost_active < combat_maneuver_boost_request)
    {
        combat_maneuver_boost_active += delta;
        if (combat_maneuver_boost_active > combat_maneuver_boost_request)
            combat_maneuver_boost_active = combat_maneuver_boost_request;
    }
    if (combat_maneuver_strafe_active > combat_maneuver_strafe_request)
    {
        combat_maneuver_strafe_active -= delta;
        if (combat_maneuver_strafe_active < combat_maneuver_strafe_request)
            combat_maneuver_strafe_active = combat_maneuver_strafe_request;
    }
    if (combat_maneuver_strafe_active < combat_maneuver_strafe_request)
    {
        combat_maneuver_strafe_active += delta;
        if (combat_maneuver_strafe_active > combat_maneuver_strafe_request)
            combat_maneuver_strafe_active = combat_maneuver_strafe_request;
    }

    // If the ship is making a combat maneuver ...
    if (combat_maneuver_boost_active != 0.0 || combat_maneuver_strafe_active != 0.0)
    {
        // ... consume its combat maneuver boost.
        combat_maneuver_charge -= fabs(combat_maneuver_boost_active) * delta / combat_maneuver_boost_max_time;
        combat_maneuver_charge -= fabs(combat_maneuver_strafe_active) * delta / combat_maneuver_strafe_max_time;

        // Use boost only if we have boost available.
        if (combat_maneuver_charge <= 0.0)
        {
            combat_maneuver_charge = 0.0;
            combat_maneuver_boost_request = 0.0;
            combat_maneuver_strafe_request = 0.0;
        }else
        {
            setVelocity(getVelocity() + forward * combat_maneuver_boost_speed * combat_maneuver_boost_active);
            setVelocity(getVelocity() + sf::vector2FromAngle(getRotation() + 90) * combat_maneuver_strafe_speed * combat_maneuver_strafe_active);
        }
    // If the ship isn't making a combat maneuver, recharge its boost.
    }else if (combat_maneuver_charge < 1.0)
    {
        combat_maneuver_charge += (delta / combat_maneuver_charge_time) * (getSystemEffectiveness(SYS_Maneuver) + getSystemEffectiveness(SYS_Impulse)) / 2.0;
        if (combat_maneuver_charge > 1.0)
            combat_maneuver_charge = 1.0;
    }

    // Add heat to systems consuming combat maneuver boost.
    if (combat_maneuver_boost_speed > 0.0)
        addHeat(SYS_Impulse, fabs(combat_maneuver_boost_active) * delta * heat_per_combat_maneuver_boost);
    if (combat_maneuver_strafe_speed > 0.0)
        addHeat(SYS_Maneuver, fabs(combat_maneuver_strafe_active) * delta * heat_per_combat_maneuver_strafe);

    beam_weapons_count = 0;
    for(int n = 0; n < max_beam_weapons; n++)
    {
        beam_weapons[n].update(delta);
        if (beam_weapons[n].getRange() > 0.0)
            beam_weapons_count += 1;
    }

    for(int n=0; n<max_weapon_tubes; n++)
    {
        weapon_tube[n].update(delta);
    }

    for(int n = 0; n < max_docks_count; n++)
    {
        docks[n].update(delta);
    }
    RawRadarSignatureInfo signature_delta;

    for(int n=0; n<SYS_COUNT; n++)
    {
        systems[n].hacked_level = std::max(0.0f, systems[n].hacked_level - delta / unhack_time);
    }

    model_info.engine_scale = std::min(1.0f, (float) std::max(fabs(getAngularVelocity() / turn_speed), fabs(current_impulse)));
    if (has_jump_drive && jump_delay > 0.0f)
        model_info.warp_scale = (10.0f - jump_delay) / 10.0f;
    else
        model_info.warp_scale = 0.0;

    for(int n = 0; n < oxygen_zones; n++)
        addOxygenPoints(getOxygenRechargeRate(n) * delta, n);

    if (has_cloaking)
    {
        setTransparency(getCloakingDegree());
        float factor_heat_cloaking = 0.1 * getTransparency() * delta;

        addHeat(SYS_Cloaking , 0.1 * getSystemEffectiveness(SYS_Cloaking) * factor_heat_cloaking);

        for(int n = 0; n < 2; n++)
        {
            ESystem system = ESystem(rand()%SYS_COUNT);
            addHeat(system , getSystemEffectiveness(system) * factor_heat_cloaking);
        }
    }
}

float SpaceShip::getCloakingDegree()
{
    return (std::tanh(getSystemEffectiveness(SYS_Cloaking))-std::tanh(0))/(std::tanh(3)-std::tanh(0));
}

float SpaceShip::getOxygenRechargeRate(int index)
{
    float rate = (oxygen_rate[index] / 100.0 * oxygen_max[index]) / 100.0;
    // Diminution de l'oxygene si Hull trop base
    if (hull_strength / hull_max < 0.9)
        rate -= (0.9 - hull_strength / hull_max) * 2.0f;

    // Modifs selon Reacteur
    if (getSystemEffectiveness(SYS_Reactor) < 0.8f)
        rate -= (0.8f - std::max(0.0f,getSystemEffectiveness(SYS_Reactor)));
    else
        rate += (getSystemEffectiveness(SYS_Reactor) - 0.8f);

    // Modifs selon nombre de passagers
    float rate_passagers = std::max(0.0f,(float) getPassagersCount()/getMaxPassagersCount() - 0.3f);
    rate -= rate_passagers * 3.0f;

    return rate;
}

float SpaceShip::getShieldRechargeRate(int shield_index)
{
    float rate = ShipTemplateBasedObject::getShieldRechargeRate(shield_index);
    rate *= getSystemEffectiveness(getShieldSystemForShieldIndex(shield_index));
    if (docking_state == DS_Docked)
    {
        P<SpaceShip> docked_with_ship = docking_target;
        if (!docked_with_ship)
            rate *= 4.0;
    }
    return rate;
}

P<SpaceObject> SpaceShip::getTarget()
{
    if (game_server)
        return game_server->getObjectById(target_id);
    return game_client->getObjectById(target_id);
}

P<SpaceObject> SpaceShip::getDockTarget()
{
    if (game_server)
        return game_server->getObjectById(dock_target_id);
    return game_client->getObjectById(dock_target_id);
}

P<SpaceObject> SpaceShip::getLandingTarget()
{
    if (game_server)
        return game_server->getObjectById(landing_target_id);
    return game_client->getObjectById(landing_target_id);
}

void SpaceShip::executeJump(float distance)
{
    float f = systems[SYS_JumpDrive].health;
    if (f <= 0.0)
        return;

    distance = (distance * f) + (distance * (1.0 - f) * random(0.5, 1.5));
    sf::Vector2f target_position = getPosition() + sf::vector2FromAngle(getRotation()) * distance;
    if (WarpJammer::isWarpJammed(target_position))
        target_position = WarpJammer::getFirstNoneJammedPosition(getPosition(), target_position);

//    P<ElectricExplosionEffect> e1 = new ElectricExplosionEffect();
//    e1->setSize((distance / 10.0) * random(0.8, 1.2));
//    e1->setPosition(getPosition());
//    e1->setOnRadar(true);

    setPosition(target_position);
    addHeat(SYS_JumpDrive, jump_drive_heat_per_jump);

//    P<ElectricExplosionEffect> e2 = new ElectricExplosionEffect();
//    e2->setSize((distance / 10.0) * random(0.8, 1.2));;
//    e2->setPosition(getPosition());
//    e2->setOnRadar(true);
}

bool SpaceShip::canBeDockedBy(P<SpaceObject> obj)
{
    if (isEnemy(obj) || !ship_template)
        return false;
    P<SpaceShip> ship = obj;
    if (!ship || !ship->ship_template)
        return false;
    // return ship_template->can_be_docked_by_class.count(ship->ship_template->getClass()) > 0;
    return true;
}

void SpaceShip::collide(Collisionable* other, float force)
{
    if (docking_state == DS_Docking && fabs(sf::angleDifference(target_rotation, getRotation())) < 10.0)
    {
        P<SpaceObject> dock_object = P<Collisionable>(other);
        if (dock_object == docking_target)
        {
            docking_state = DS_Docked;
            docking_offset = sf::rotateVector(getPosition() - other->getPosition(), -other->getRotation());
            float length = sf::length(docking_offset);
            docking_offset = docking_offset / length * (length + 2.0f);
        }
    }

    if (landing_state == LS_Landing && fabs(sf::angleDifference(target_rotation, getRotation())) < 10.0)
    {
        P<SpaceShip> land_object = P<Collisionable>(other);
        if (land_object && (land_object == landing_target))
        {
            Dock* dock = Dock::findOpenForDocking(land_object->docks, max_docks_count);
            if (dock)
            {
                landing_state = LS_Landed;
                P<ShipCargo> cargo = new ShipCargo(this); //should keep current parameters
                dock->dock(cargo);
                for(int n=0; n<GameGlobalInfo::max_player_ships; n++)
                {

                    P<PlayerSpaceship> ship = gameGlobalInfo->getPlayerShip(n);
                    if(ship)
                        ship->addToShipLog("Atterrissage de " + getCallSign() + " dans un dock de " + land_object->getCallSign(),colorConfig.log_generic,"docks");
                }
                destroy();

            }
            else
            {
                //TODO put a message
            }
        }
    }
}

void SpaceShip::initializeJump(float distance)
{
    if (docking_state != DS_NotDocking)
        return;
    if (jump_drive_charge < jump_drive_max_distance) // You can only jump when the drive is fully charged
        return;
    if (jump_delay <= 0.0)
    {
        jump_distance = distance;
        jump_delay = 100 * distance / (1000 * 1000);
        jump_drive_charge -= distance;
    }
}

void SpaceShip::requestDock(P<SpaceObject> target)
{
    if (!target || docking_state != DS_NotDocking || !target->canBeDockedBy(this))
        return;
    if (sf::length(getPosition() - target->getPosition()) > 1000 + target->getRadius())
        return;
    if (!canStartDocking())
        return;

    docking_state = DS_Docking;
    docking_target = target;
//    dock_target_id = target->getMultiplayerId();
    warp_request = 0.0;
}

void SpaceShip::requestLanding(P<SpaceObject> target)
{
    if (!target || landing_state != LS_NotLanding || !target->canBeLandedOn(this))
        return;
    if (sf::length(getPosition() - target->getPosition()) > 1000 + target->getRadius())
        return;
    if (!canStartLanding())
        return;
    P<SpaceShip> ship = target;
    if(!ship)
        return;
    if(!Dock::findOpenForDocking(ship->docks, max_docks_count))
        return;

    landing_state = LS_Landing;
    landing_target = target;
    warp_request = 0.0;
}

void SpaceShip::requestUndock()
{
    if (docking_state == DS_Docked)
    {
        docking_state = DS_NotDocking;
        if (getSystemEffectiveness(SYS_Impulse) > 0.1){
            impulse_request = 0.5;
        }
    }
}

void SpaceShip::abortDock()
{
    if (docking_state == DS_Docking)
    {
        docking_state = DS_NotDocking;
        impulse_request = 0.0;
        warp_request = 0.0;
        target_rotation = getRotation();
    }
}


void SpaceShip::abortLanding()
{
    if (landing_state == LS_Landing)
    {
        landing_state = LS_NotLanding;
        impulse_request = 0.0;
        warp_request = 0.0;
        target_rotation = getRotation();
    }
}

int SpaceShip::scanningComplexity(P<SpaceObject> other)
{
    if (scanning_complexity_value > -1)
        return scanning_complexity_value;
    switch(gameGlobalInfo->scanning_complexity)
    {
    case SC_None:
        return 0;
    case SC_Simple:
        return 1;
    case SC_Normal:
        if (getScannedStateFor(other) == SS_SimpleScan)
            return 2;
        return 1;
    case SC_Advanced:
        if (getScannedStateFor(other) == SS_SimpleScan)
            return 3;
        return 2;
    }
    return 0;
}

int SpaceShip::scanningChannelDepth(P<SpaceObject> other)
{
    if (scanning_depth_value > -1)
        return scanning_depth_value;
    switch(gameGlobalInfo->scanning_complexity)
    {
    case SC_None:
        return 0;
    case SC_Simple:
        return 1;
    case SC_Normal:
        return 2;
    case SC_Advanced:
        return 2;
    }
    return 0;
}

void SpaceShip::scannedBy(P<SpaceObject> other)
{
    switch(getScannedStateFor(other))
    {
    case SS_NotScanned:
    case SS_FriendOrFoeIdentified:
        setScannedStateFor(other, SS_SimpleScan);
        break;
    case SS_SimpleScan:
        setScannedStateFor(other, SS_FullScan);
        break;
    case SS_FullScan:
        break;
    }
}

void SpaceShip::setScanState(EScannedState state)
{
    for(unsigned int faction_id = 0; faction_id < factionInfo.size(); faction_id++)
    {
        setScannedStateForFaction(faction_id, state);
    }
}

void SpaceShip::setScanStateByFaction(string faction_name, EScannedState state)
{
    setScannedStateForFaction(FactionInfo::findFactionId(faction_name), state);
}

bool SpaceShip::isFriendOrFoeIdentified()
{
    LOG(WARNING) << "Deprecated \"isFriendOrFoeIdentified\" function called, use isFriendOrFoeIdentifiedBy or isFriendOrFoeIdentifiedByFaction.";
    for(unsigned int faction_id = 0; faction_id < factionInfo.size(); faction_id++)
    {
        if (getScannedStateForFaction(faction_id) > SS_NotScanned)
            return true;
    }
    return false;
}

bool SpaceShip::isFullyScanned()
{
    LOG(WARNING) << "Deprecated \"isFullyScanned\" function called, use isFullyScannedBy or isFullyScannedByFaction.";
    for(unsigned int faction_id = 0; faction_id < factionInfo.size(); faction_id++)
    {
        if (getScannedStateForFaction(faction_id) >= SS_FullScan)
            return true;
    }
    return false;
}

bool SpaceShip::isFriendOrFoeIdentifiedBy(P<SpaceObject> other)
{
    return getScannedStateFor(other) >= SS_FriendOrFoeIdentified;
}

bool SpaceShip::isFullyScannedBy(P<SpaceObject> other)
{
    return getScannedStateFor(other) >= SS_FullScan;
}

bool SpaceShip::isFriendOrFoeIdentifiedByFaction(int faction_id)
{
    return getScannedStateForFaction(faction_id) >= SS_FriendOrFoeIdentified;
}

bool SpaceShip::isFullyScannedByFaction(int faction_id)
{
    return getScannedStateForFaction(faction_id) >= SS_FullScan;
}

bool SpaceShip::canBeHackedBy(P<SpaceObject> other)
{
    return (!(this->isFriendly(other)) && this->isFriendOrFoeIdentifiedBy(other)) ;
}

std::vector<std::pair<string, float>> SpaceShip::getHackingTargets()
{
    std::vector<std::pair<string, float>> results;
    for(unsigned int n=0; n<SYS_COUNT; n++)
    {
        if (n != SYS_Reactor && hasSystem(ESystem(n)))
        {
            results.emplace_back(getSystemName(ESystem(n)), systems[n].hacked_level);
        }
    }
    return results;
}

void SpaceShip::hackFinished(P<SpaceObject> source, string target)
{
    for(unsigned int n=0; n<SYS_COUNT; n++)
    {
        if (hasSystem(ESystem(n)))
        {
            if (target == getSystemName(ESystem(n)))
            {
                systems[n].hacked_level = std::min(1.0f, systems[n].hacked_level + 0.5f);
                return;
            }
        }
    }
    LOG(WARNING) << "Unknown hacked target: " << target;
}

float SpaceShip::getShieldDamageFactor(DamageInfo& info, int shield_index)
{
    float frequency_damage_factor = 1.0;
    if (info.type == DT_Energy && gameGlobalInfo->use_beam_shield_frequencies)
    {
        frequency_damage_factor = frequencyVsFrequencyDamageFactor(info.frequency, shield_frequency);
    }
    ESystem system = getShieldSystemForShieldIndex(shield_index);

    //Shield damage reduction curve. Damage reduction gets slightly exponetial effective with power.
    // This also greatly reduces the ineffectiveness at low power situations.
    float shield_damage_exponent = 1.6;
    float shield_damage_divider = 7.0;
    float shield_damage_factor = 1.0 + powf(1.0, shield_damage_exponent) / shield_damage_divider-powf(getSystemEffectiveness(system), shield_damage_exponent) / shield_damage_divider;

    return shield_damage_factor * frequency_damage_factor;
}

void SpaceShip::didAnOffensiveAction()
{
    //We did an offensive action towards our target.
    // Check for each faction. If this faction knows if the target is an enemy or a friendly, it now knows if this object is an enemy or a friendly.
    for(unsigned int faction_id=0; faction_id<factionInfo.size(); faction_id++)
    {
        if (getScannedStateForFaction(faction_id) == SS_NotScanned)
        {
            if (getTarget() && getTarget()->getScannedStateForFaction(faction_id) != SS_NotScanned)
                setScannedStateForFaction(faction_id, SS_FriendOrFoeIdentified);
        }
    }
}

void SpaceShip::takeHullDamage(float damage_amount, DamageInfo& info)
{
    if (gameGlobalInfo->use_system_damage
        && (damage_amount / hull_max) > system_damage_hull_threshold)
    {
        if (info.system_target != SYS_None)
        {
            //Target specific system

            float system_damage = (damage_amount / hull_max) * 2.0 * system_damage_ratio;
            if (info.type == DT_Energy)
                system_damage *= 3.0;   //Beam weapons do more system damage, as they penetrate the hull easier.
            systems[info.system_target].health -= system_damage;
            if (systems[info.system_target].health < -1.0)
                systems[info.system_target].health = -1.0;

            for(int n=0; n<2; n++)
            {
                ESystem random_system = SYS_None;
                bool find_system = true;
                while (find_system)
                {
                    random_system = ESystem(irandom(0, SYS_COUNT - 1));
                    if (hasSystem(random_system))
                        find_system = false;
                }
                //Damage the system compared to the amount of hull damage you would do. If we have less hull strength you get more system damage.
                float system_damage = (damage_amount / hull_max) * 1.0 * system_damage_ratio;
                systems[random_system].health -= system_damage;
                if (systems[random_system].health < -1.0)
                    systems[random_system].health = -1.0;
            }

            if (info.type == DT_Energy)
                damage_amount *= 0.02;
            else
                damage_amount *= 0.5;
        }else{
            ESystem random_system = SYS_None;
            bool find_system = true;
            while (find_system)
            {
                random_system = ESystem(irandom(0, SYS_COUNT - 1));
                if (hasSystem(random_system))
                    find_system = false;
            }

            //Damage the system compared to the amount of hull damage you would do. If we have less hull strength you get more system damage.
            float system_damage = (damage_amount / hull_max) * 3.0 * system_damage_ratio;
            if (info.type == DT_Energy)
                system_damage *= 2.5;   //Beam weapons do more system damage, as they penetrate the hull easier.
            systems[random_system].health -= system_damage;
            if (systems[random_system].health < -1.0)
                systems[random_system].health = -1.0;
        }
    }

    ShipTemplateBasedObject::takeHullDamage(damage_amount, info);
}

void SpaceShip::destroyedByDamage(DamageInfo& info)
{
    ExplosionEffect* e = new ExplosionEffect();
    e->setSize(getRadius() * 1.5);
    e->setPosition(getPosition());
    e->setRadarSignatureInfo(0.0, 0.2, 0.2);

    if (info.instigator)
    {
        float points = hull_max * 0.1f;
        for(int n=0; n<shield_count; n++)
            points += shield_max[n] * 0.1f;
        if (isEnemy(info.instigator))
            info.instigator->addReputationPoints(points);
        else
            info.instigator->removeReputationPoints(points);
    }
}

bool SpaceShip::hasSystem(ESystem system)
{
    switch(system)
    {
    case SYS_None:
    case SYS_COUNT:
        return false;
    case SYS_Warp:
        return has_warp_drive;
    case SYS_JumpDrive:
        return has_jump_drive;
    case SYS_MissileSystem:
        return weapon_tube_count > 0;
    case SYS_FrontShield:
        return shield_count > 0;
    case SYS_RearShield:
        return shield_count > 1;
    case SYS_Reactor:
        return has_reactor;
    case SYS_Cloaking:
        return has_cloaking;
    case SYS_BeamWeapons:
        return beam_weapons_count > 0;
    case SYS_Maneuver:
        return turn_speed > 0.0;
    case SYS_Impulse:
        return impulse_max_speed > 0.0;
    case SYS_Drones:
        return true;
    case SYS_Docks:
    case SYS_Door:
        return docks[0].dock_type != Dock_Disabled;
    }
    return true;
}

int SpaceShip::countSystems()
{
    int count_system = 0;
    for(int n=0; n<SYS_COUNT; n++)
        if (hasSystem(ESystem(n)))
            count_system += 1;
    return count_system;
}

float SpaceShip::getSystemEffectiveness(ESystem system)
{
    float power = systems[system].power_level;
    
    // Substract the hacking from the power, making double hacked systems run at 25% efficiency.
    power = std::max(0.0f, power - systems[system].hacked_level * 0.75f);

    // Degrade all systems except the reactor once energy level drops below 10.
    if (system != SYS_Reactor)
    {
        if (energy_level < 10.0 && energy_level > 0.0 && power > 0.0)
            power = std::min(power * energy_level / 10.0f, power);
        else if (energy_level <= 0.0 || power <= 0.0)
            power = 0.0f;
    }

    // Degrade damaged systems.
    if (gameGlobalInfo && gameGlobalInfo->use_system_damage)
        return std::max(0.0f, power * systems[system].health);

    // If a system cannot be damaged, excessive heat degrades it.
    return std::max(0.0f, power * (1.0f - systems[system].heat_level));
}

void SpaceShip::setWeaponTubeCount(int amount)
{
    weapon_tube_count = std::max(0, std::min(amount, max_weapon_tubes));
    for(int n=weapon_tube_count; n<max_weapon_tubes; n++)
    {
        weapon_tube[n].forceUnload();
    }
}

int SpaceShip::getWeaponTubeCount()
{
    return weapon_tube_count;
}

string SpaceShip::getWeaponTubeLoadType(int index)
{
    if (index < 0 || index >= weapon_tube_count)
        return MW_None;
    if (!weapon_tube[index].isLoaded())
        return MW_None;
    return weapon_tube[index].getLoadType();
}

EMissileSizes SpaceShip::getWeaponTubeSize(int index)
{
    if (index < 0 || index >= weapon_tube_count)
        return MS_Small;
    return weapon_tube[index].getSize();
}

void SpaceShip::setWeaponTubeSize(int index, EMissileSizes size)
{
    if (index < 0 || index >= weapon_tube_count)
        return;
    weapon_tube[index].setSize(size);
}

void SpaceShip::weaponTubeAllowMissle(int index, EMissileWeapons type)
{
    if (index < 0 || index >= weapon_tube_count)
        return;
    weapon_tube[index].allowLoadOf(type);
}

void SpaceShip::weaponTubeDisallowMissle(int index, EMissileWeapons type)
{
    if (index < 0 || index >= weapon_tube_count)
        return;
    weapon_tube[index].disallowLoadOf(type);
}

void SpaceShip::setWeaponTubeExclusiveFor(int index, EMissileWeapons type)
{
    if (index < 0 || index >= weapon_tube_count)
        return;
    for(int n=0; n<MW_Count; n++)
        weapon_tube[index].disallowLoadOf(EMissileWeapons(n));
    weapon_tube[index].allowLoadOf(type);
}

void SpaceShip::setWeaponTubeDirection(int index, float direction)
{
    if (index < 0 || index >= weapon_tube_count)
        return;
    weapon_tube[index].setDirection(direction);
}

void SpaceShip::addBroadcast(int threshold, string message)
{
    if ((threshold < 0) || (threshold > 2))     //if an invalid threshold is defined, alert and default to ally only
    {
        LOG(ERROR) << "Invalid threshold: " << threshold;
        threshold = 0;
    }

    message = this->getCallSign() + " : " + message; //append the callsign at the start of broadcast

    sf::Color color = sf::Color(255, 204, 51); //default : yellow, should never be seen

    for(int n=0; n<GameGlobalInfo::max_player_ships; n++)
    {
        bool addtolog = 0;
        P<PlayerSpaceship> ship = gameGlobalInfo->getPlayerShip(n);
        if (ship)
        {
            if (this->isFriendly(ship))
            {
                color = sf::Color(154,255,154); //ally = light green
                addtolog = 1;
            }
            else if ((factionInfo[this->getFactionId()]->states[ship->getFactionId()] == FVF_Neutral) && ((threshold >= FVF_Neutral)))
            {
                color = sf::Color(128,128,128); //neutral = grey
                addtolog = 1;
            }
            else if ((this->isEnemy(ship)) && (threshold == FVF_Enemy))
            {
                color = sf::Color(255,102,102); //enemy = light red
                addtolog = 1;
            }

            if (addtolog && id_galaxy == ship->id_galaxy)
            {
                ship->addToShipLog(message, color, "generic");
            }
        }
    }
}

std::unordered_map<string, string> SpaceShip::getGMInfo()
{
    std::unordered_map<string, string> ret;
    ret = ShipTemplateBasedObject::getGMInfo();
    return ret;
}

string SpaceShip::getScriptExportModificationsOnTemplate()
{
    // Exports attributes common to ships as Lua script function calls.
    // Initialize the exported string.
    string ret = "";

    // If traits don't differ from the ship template, don't bother exporting
    // them.
    if (getTypeName() != ship_template->getName())
        ret += ":setTypeName(" + getTypeName() + ")";
    if (hull_max != ship_template->hull)
        ret += ":setHullMax(" + string(hull_max, 0) + ")";
    if (hull_strength != ship_template->hull)
        ret += ":setHull(" + string(hull_strength, 0) + ")";
    if (impulse_max_speed != ship_template->impulse_speed)
        ret += ":setImpulseMaxSpeed(" + string(impulse_max_speed, 1) + ")";
    if (turn_speed != ship_template->turn_speed)
        ret += ":setRotationMaxSpeed(" + string(turn_speed, 1) + ")";
    if (has_jump_drive != ship_template->has_jump_drive)
        ret += ":setJumpDrive(" + string(has_jump_drive ? "true" : "false") + ")";
    if (has_warp_drive != (ship_template->warp_speed > 0))
        ret += ":setWarpDrive(" + string(has_warp_drive ? "true" : "false") + ")";
    if(system_damage_ratio != ship_template->system_damage_ratio)
    {
        ret +=":setSystemDamageRatio(" + string(system_damage_ratio) + ")";
    }
    if(system_damage_hull_threshold != ship_template->system_damage_hull_threshold)
    {
        ret +=":setSystemDamageHullThreshold(" + string(system_damage_hull_threshold) + ")";
    }
    // Shield data
    // Determine whether to export shield data.
    bool add_shields_max_line = getShieldCount() != ship_template->shield_count;
    bool add_shields_line = getShieldCount() != ship_template->shield_count;

    // If shield max and level don't differ from the template, don't bother
    // exporting them.
    for(int n = 0; n < getShieldCount(); n++)
    {
        if (getShieldMax(n) != ship_template->shield_level[n])
            add_shields_max_line = true;
        if (getShieldLevel(n) != ship_template->shield_level[n])
            add_shields_line = true;
    }

    // If we're exporting shield max ...
    if (add_shields_max_line)
    {
        ret += ":setShieldsMax(";

        // ... for each shield, export the shield max.
        for(int n = 0; n < getShieldCount(); n++)
        {
            if (n > 0)
                ret += ", ";

            ret += string(getShieldMax(n));
        }

        ret += ")";
    }

    // If we're exporting shield level ...
    if (add_shields_line)
    {
        ret += ":setShields(";

        // ... for each shield, export the shield level.
        for(int n = 0; n < getShieldCount(); n++)
        {
            if (n > 0)
                ret += ", ";

            ret += string(getShieldLevel(n));
        }

        ret += ")";
    }

    ///Missile weapon data
    if (weapon_tube_count != ship_template->weapon_tube_count)
        ret += ":setWeaponTubeCount(" + string(weapon_tube_count) + ")";

    for(int n=0; n<weapon_tube_count; n++)
    {
        WeaponTube& tube = weapon_tube[n];
        if (tube.getDirection() != ship_template->weapon_tube[n].direction)
            ret += ":setWeaponTubeDirection(" + string(n) + ", " + string(tube.getDirection(), 0) + ")";
        //TODO: Weapon tube "type_allowed_mask"
        //TODO: Weapon tube "load_time"
    }
    for(int n=0; n<MW_Count; n++)
    {
        if (weapon_storage_max[n] != ship_template->weapon_storage[n])
            ret += ":setWeaponStorageMax(\"" + getMissileWeaponName(EMissileWeapons(n)) + "\", " + string(weapon_storage_max[n]) + ")";
        if (weapon_storage[n] != ship_template->weapon_storage[n])
            ret += ":setWeaponStorage(\"" + getMissileWeaponName(EMissileWeapons(n)) + "\", " + string(weapon_storage[n]) + ")";
    }

    for(auto& kv : ship_template->custom_weapon_storage)
    {
        ret += ":setCustomWeaponStorage(\"" + kv.first + "\", " + string(kv.second) + ")";
        ret += ":setCustomWeaponStorageMax(\"" + kv.first + "\", " + string(kv.second) + ")";
    }



    ///Beam weapon data
    for(int n=0; n<max_beam_weapons; n++)
    {
        if (beam_weapons[n].getArc() != ship_template->beams[n].getArc()
         || beam_weapons[n].getDirection() != ship_template->beams[n].getDirection()
         || beam_weapons[n].getRange() != ship_template->beams[n].getRange()
         || beam_weapons[n].getTurretArc() != ship_template->beams[n].getTurretArc()
         || beam_weapons[n].getTurretDirection() != ship_template->beams[n].getTurretDirection()
         || beam_weapons[n].getTurretRotationRate() != ship_template->beams[n].getTurretRotationRate()
         || beam_weapons[n].getCycleTime() != ship_template->beams[n].getCycleTime()
         || beam_weapons[n].getDamage() != ship_template->beams[n].getDamage())
        {
            ret += ":setBeamWeapon(" + string(n) + ", " + string(beam_weapons[n].getArc(), 0) + ", " + string(beam_weapons[n].getDirection(), 0) + ", " + string(beam_weapons[n].getRange(), 0) + ", " + string(beam_weapons[n].getCycleTime(), 1) + ", " + string(beam_weapons[n].getDamage(), 1) + ")";
            ret += ":setBeamWeaponTurret(" + string(n) + ", " + string(beam_weapons[n].getTurretArc(), 0) + ", " + string(beam_weapons[n].getTurretDirection(), 0) + ", " + string(beam_weapons[n].getTurretRotationRate(), 0) + ")";
        }
    }

    return ret;
}

bool SpaceShip::tryDockDrone(SpaceShip* other){
    if (other->ship_template->getType() == ShipTemplate::TemplateType::Drone){
        Dock* dock = Dock::findOpenForDocking(docks, max_docks_count);
        if (dock){
            P<ShipCargo> cargo = new ShipCargo(other);
            dock->dock(cargo);
            return true;
        }
    }
    return false;
}

namespace
{
    bool isNumber(const std::string& s)
    {
       return !s.empty() && s.find_first_not_of("-.0123456789") == std::string::npos;
    }

}
string getMissileWeaponName(const EMissileWeapons& missile)
{
    switch(missile)
    {
    case MW_None:
        return "-";
    case MW_Homing:
        return "TCC";
    case MW_Nuke:
        return "TCN";
    case MW_Mine:
        return "Mine";
    case MW_EMP:
        return "TCI";
    case MW_HVLI:
        return "TBHV";
    default:
        return "UNK: " + string(int(missile));
    }
}

string getMissileWeaponName(const string& missile)
{
    if(isNumber(missile))
    {
        EMissileWeapons num = (EMissileWeapons)std::stoi(missile);
        return getMissileWeaponName(num);
    }
    else return missile;

}

string getLocaleMissileWeaponName(const string& missile)
{
    if(isNumber(missile))
    {
        EMissileWeapons num = (EMissileWeapons)std::stoi(missile);
        return getLocaleMissileWeaponName(num);
    }
    else return missile; //Non localise pour le moment
}

string getLocaleMissileWeaponName(const EMissileWeapons& missile)
{
    switch(missile)
    {
    case MW_None:
        return "-";
    case MW_Homing:
        return tr("missile","Homing");
    case MW_Nuke:
        return tr("missile","Nuke");
    case MW_Mine:
        return tr("missile","Mine");
    case MW_EMP:
        return tr("missile","EMP");
    case MW_HVLI:
        return tr("missile","HVLI");
    default:
        return "UNK: " + string(int(missile));
    }
}


float frequencyVsFrequencyDamageFactor(int beam_frequency, int shield_frequency)
{
    if (beam_frequency < 0 || shield_frequency < 0)
        return 1.0;

    float diff = abs(beam_frequency - shield_frequency);
    float f1 = sinf(Tween<float>::linear(diff, 0, SpaceShip::max_frequency, 0, M_PI * (1.2 + shield_frequency * 0.05)) + M_PI / 2);
    f1 = f1 * Tween<float>::easeInCubic(diff, 0, SpaceShip::max_frequency, 1.0, 0.1);
    f1 = Tween<float>::linear(f1, 1.0, -1.0, 0.5, 1.5);
    return f1;
}

string frequencyToString(int frequency)
{
    return string(400 + (frequency * 20)) + "THz";
}

#ifndef _MSC_VER
#include "spaceship.hpp"
#endif
