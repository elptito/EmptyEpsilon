template = ShipTemplate():setName("Battle docker"):setLocaleName(_("Battle docker")):setClass(_("Corvette"), _("Destroyer")):setModel("battleship_destroyer_1_upgraded"):setType("playership")
template:setDescription(_([[The battle Docker can send drones to explore , fight and support a war field.]]))
template:setDockClasses("Drone")
template:setRadarTrace("radar_dread.png")
template:setHull(100)
template:setCombatManeuver(400, 250)
template:setShields(200, 200, 200, 200)
template:setSpeed(90, 10, 20)
template:setJumpDrive(true)
--                  Arc, Dir, Range, CycleTime, Dmg
template:setBeam(0,100, -20, 1500.0, 6.0, 8)
template:setBeam(1,100,  20, 1500.0, 6.0, 8)
template:setBeam(2,100, 180, 1500.0, 6.0, 8)
template:setTubes(5, 8.0) -- Amount of torpedo tubes, and loading time of the tubes.
template:weaponTubeDisallowMissle(0, "Mine"):weaponTubeDisallowMissle(1, "Mine")
template:weaponTubeDisallowMissle(2, "Mine"):weaponTubeDisallowMissle(3, "Mine")
template:setTubeDirection(4, 180):setWeaponTubeExclusiveFor(4, "Mine")
template:setWeaponStorage("Homing", 12)
template:setWeaponStorage("Nuke", 4)
template:setWeaponStorage("Mine", 8)
template:setWeaponStorage("EMP", 6)
template:setTubeDirection(0, 0)
template:setTubeDirection(1, 0)
template:setTubeDirection(2,  -90)
template:setTubeDirection(3,  90)
template:setDocks(2, 1, 1, 1, 1, 1)
template:addDrones("L3 Mouse", 1)
template:addDrones("L3 Cat", 1)
template:addDrones("H9 Owl", 1)
template:addDrones("M19 Rino", 1)
template:setTractorBeam(2000, 100)

template:addRoomSystem(1, 0, 2, 1, "Maneuver");
template:addRoomSystem(1, 1, 2, 1, "BeamWeapons");
template:addRoomSystem(2, 2, 2, 1, "Drones");
template:addRoomSystem(0, 3, 1, 2, "RearShield");
template:addRoomSystem(1, 3, 2, 2, "Reactor");
template:addRoomSystem(3, 3, 2, 2, "Warp");
template:addRoomSystem(5, 3, 1, 2, "JumpDrive");
template:addRoomTitle(6, 3, 2, 1, "Commands center");
template:addRoomSystem(6, 4, 2, 1, "Docks");
template:addRoomSystem(8, 3, 1, 2, "FrontShield");
template:addRoomTitle(2, 5, 2, 1, "Sick Bay");
template:addRoomSystem(1, 6, 2, 1, "MissileSystem");
template:addRoomSystem(1, 7, 2, 1, "Impulse");
template:addDoor(1, 1, true);
template:addDoor(2, 2, true);
template:addDoor(3, 3, true);
template:addDoor(1, 3, false);
template:addDoor(3, 4, false);
template:addDoor(3, 5, true);
template:addDoor(2, 6, true);
template:addDoor(1, 7, true);
template:addDoor(5, 3, false);
template:addDoor(6, 3, false);
template:addDoor(6, 4, false);
template:addDoor(8, 3, false);
template:addDoor(8, 4, false);

template:setHasOxygenGenerator(true);
template:setOxygenZone(0, "Test", 100.0, 0.1, 0.1);

-- To choose recorded ship template or not
template = ShipTemplate():setName("Not recorded ship"):setClass("Secret", "Secret"):setModel("AtlasHeavyFighterYellow"):setRecorded(false)

-- Station like ship
template = ShipTemplate():setName("Small Station"):setLocaleName(_("Small Station")):setModel("space_station_4"):setClass("Station", "Standart station")
template:setDescription(_([[Stations of this size are often used as research outposts, listening stations, and security checkpoints. Crews turn over frequently in a small station's cramped accommodatations, but they are small enough to look like ships on many long-range sensors, and organized raiders sometimes take advantage of this by placing small stations in nebulae to serve as raiding bases. They are lightly shielded and vulnerable to swarming assaults.]]))
template:setHull(150)
template:setShields(300)
template:setRadarTrace("radartrace_smallstation.png")

template = ShipTemplate():setName("Medium Station"):setLocaleName(_("Medium Station")):setModel("space_station_3"):setClass("Station", "Standart station")
template:setDescription(_([[Large enough to accommodate small crews for extended periods of times, stations of this size are often trading posts, refuelling bases, mining operations, and forward military bases. While their shields are strong, concerted attacks by many ships can bring them down quickly.]]))
template:setHull(400)
template:setShields(800)
template:setRadarTrace("radartrace_mediumstation.png")

template = ShipTemplate():setName("Large Station"):setLocaleName(_("Large Station")):setModel("space_station_2"):setClass("Station", "Standart station")
template:setDescription(_([[These spaceborne communities often represent permanent bases in a sector. Stations of this size can be military installations, commercial hubs, deep-space settlements, and small shipyards. Only a concentrated attack can penetrate a large station's shields, and its hull can withstand all but the most powerful weaponry.]]))
template:setHull(500)
template:setShields(1000, 1000, 1000)
template:setRadarTrace("radartrace_largestation.png")

template = ShipTemplate():setName("Huge Station"):setLocaleName(_("Huge Station")):setModel("space_station_1"):setClass("Station", "Standart station")
template:setDescription(_([[The size of a sprawling town, stations at this scale represent a faction's center of spaceborne power in a region. They serve many functions at once and represent an extensive investment of time, money, and labor. A huge station's shields and thick hull can keep it intact long enough for reinforcements to arrive, even when faced with an ongoing siege or massive, perfectly coordinated assault.]]))
template:setHull(800)
template:setShields(1200, 1200, 1200, 1200)
template:setRadarTrace("radartrace_hugestation.png")

