



--[[Gravitas]]

template = ShipTemplate():setName("Gravitas"):setType("playership"):setClass("Frigate", "Special"):setModel("transport_1_1")
template:setDescription([[Gravitas Covert Ops Ship.]])
template:setDockClasses("Drone")
template:setRadarTrace("radar_transport.png")
template:setHull(250)
template:setShields(200, 200)
template:setSpeed(90, 10, 20)
template:setJumpDrive(false)
template:setWarpSpeed(275)
template:setCombatManeuver(400, 250)

--                  Arc, Dir, Range, CycleTime, Dmg
template:setBeam(0,100, -20, 1500.0, 6.0, 8)
template:setBeam(1,100,  20, 1500.0, 6.0, 8)
template:setTubes(5, 8.0) 
template:setWeaponStorage("cruise", 8)
template:setWeaponStorage("torpedo", 8)
template:setWeaponStorage("heavy", 12)
template:setWeaponStorage("EMP", 6)
template:setWeaponStorage("Nuke", 2)
template:setWeaponStorage("HVLI", 30)
template:setWeaponStorage("Mine", 12)
template:setTubeDirection(0, -90)
template:setTubeDirection(1, -90)
template:setTubeDirection(2,  90)
template:setTubeDirection(3,  90)

template:setDocks(6, 2, 2, 2)
template:addDrones("L3 Mouse", 4)
template:addDrones("L3 Cat", 2)
template:addDrones("M19 Rino", 1)
template:setTractorBeam(200000, 100)

template:weaponTubeDisallowMissle(0, "Mine"):weaponTubeDisallowMissle(1, "Mine")
template:weaponTubeDisallowMissle(2, "Mine"):weaponTubeDisallowMissle(3, "Mine")
template:setTubeDirection(4, 180):setWeaponTubeExclusiveFor(4, "Mine")

--[[-----------------------Drones-----------------------]]
template = ShipTemplate():setName("L3 Mouse"):setType("drone"):setClass("Drone", "Beamer"):setModel("small_fighter_1")
template:setHasReactor(false)
template:setRadarTrace("radar_fighter.png")
template:setDescription([[Beams Drone Fabricated in great numbers by any number of corporations ever since Jin’Dau published the base design to the public, the Mouse (Often referred to as Rat Drone) - carries a handsome payload and has a sturdy chassis for just about any use in the battlefield and beyond.  The Federated Fleet most often employs squadrons of two to six such drones equipped with sensory arrays and Beam weapons for precision activity, usually operated from a nearby StarHammer II or a more distant Atlantis Command Battlecruiser.
]])
template:setEnergyStorage(200)
template:setHull(50)
-- template:setShields(20)
template:setSpeed(140, 50, 50)
template:setCombatManeuver(400, 250)
template:setDefaultAI('fighter')
--                  Arc, Dir, Range, CycleTime, Dmg
template:setBeam(0, 30, -15, 700.0, 5.0, 4)
template:setBeam(1, 30, 15, 700.0, 5.0, 4)
template:setJumpDrive(false)
template:setWarpSpeed(-1)
template:setRepairCrewCount(0)

template = ShipTemplate():setName("L3 Cat"):setType("drone"):setClass("Drone", "Swoosher"):setModel("AdlerLongRangeScoutRed")
template:setHasReactor(false)
template:setRadarTrace("radar_fighter.png")
template:setDescription([[High Velocity Light Interceptor Drone, the Cat is produced by commissioned firms all across the solar system. Lightweight and relatively sturdy around hard burn and turns - the L3 Cat garnered fame as a ferocious close-combat drone that pounces from powered-down hiding unto just about anything with a volley of hypervelocity shrapnel. What it lacks in staying power, the Cat has in quick booting sequence and short term speed velocity assaults. 
Hive-Linked Groups of these vicious little drones are often nicknamed ‘Assault Prides’.]])
template:setEnergyStorage(200)
template:setHull(50)
-- template:setShields(20)
template:setSpeed(140, 50, 50)
template:setCombatManeuver(400, 250)
template:setDefaultAI('fighter')
template:setTubes(1, 10.0)
template:setWeaponStorage("HVLI", 6)
template:setJumpDrive(false)
template:setWarpSpeed(-1)
template:setRepairCrewCount(0)

template = ShipTemplate():setName("M19 Rino"):setType("drone"):setClass("Drone", "Swoosher"):setModel("LindwurmFighterYellow")
template:setHasReactor(false)
template:setRadarTrace("radar_fighter.png")
template:setDescription([[Heavy duty, mass-cargo drone-squad used to handle dangerous materiel. The Rino (not to be confused with the Rhino model of Dreadnaughts) model is an Earth-Federated commissioned piece of equipment that takes up to three standard Drone Arrays. 
In fact, a single Rino model is an array of three to seven  hive-linked drones working to tow a row of specially designed mines or other sort s of fielded equipment and quickly disseminate it to target location with minimal needed guidance and according to plan. Point and click really.
]])
template:setEnergyStorage(300)
template:setHull(150)
template:setSpeed(60, 50, 20)
template:setCombatManeuver(400, 250)
template:setTubes(1, 15.0)
template:setWeaponStorage("Mine", 2)
template:setDefaultAI('fighter')
template:setJumpDrive(false)
template:setWarpSpeed(-1)
template:setRepairCrewCount(0)

template = ShipTemplate():setName("L3 Mouse "):setClass("Drone", "Beamer"):setModel("small_fighter_1")
template:setHasReactor(false)
template:setRadarTrace("radar_fighter.png")
template:setDescription([[Beams Drone Fabricated in great numbers by any number of corporations ever since Jin’Dau published the base design to the public, the Mouse (Often referred to as Rat Drone) - carries a handsome payload and has a sturdy chassis for just about any use in the battlefield and beyond.  The Federated Fleet most often employs squadrons of two to six such drones equipped with sensory arrays and Beam weapons for precision activity, usually operated from a nearby StarHammer II or a more distant Atlantis Command Battlecruiser.
]])
template:setEnergyStorage(200)
template:setHull(50)
-- template:setShields(20)
template:setSpeed(140, 50, 50)
template:setCombatManeuver(400, 250)
template:setDefaultAI('fighter')
--                  Arc, Dir, Range, CycleTime, Dmg
template:setBeam(0, 30, -15, 700.0, 5.0, 4)
template:setBeam(1, 30, 15, 700.0, 5.0, 4)
template:setJumpDrive(false)
template:setWarpSpeed(-1)
template:setRepairCrewCount(0)

template = ShipTemplate():setName("L3 Cat "):setClass("Drone", "Rockets"):setModel("AdlerLongRangeScoutRed")
template:setHasReactor(false)
template:setRadarTrace("radar_fighter.png")
template:setDescription([[High Velocity Light Interceptor Drone, the Cat is produced by commissioned firms all across the solar system. Lightweight and relatively sturdy around hard burn and turns - the L3 Cat garnered fame as a ferocious close-combat drone that pounces from powered-down hiding unto just about anything with a volley of hypervelocity shrapnel. What it lacks in staying power, the Cat has in quick booting sequence and short term speed velocity assaults. 
Hive-Linked Groups of these vicious little drones are often nicknamed ‘Assault Prides’.]])
template:setEnergyStorage(200)
template:setHull(50)
-- template:setShields(20)
template:setSpeed(140, 50, 50)
template:setCombatManeuver(400, 250)
template:setDefaultAI('fighter')
template:setTubes(1, 10.0)
template:setWeaponStorage("HVLI", 6)
template:setJumpDrive(false)
template:setWarpSpeed(-1)
template:setRepairCrewCount(0)

template = ShipTemplate():setName("M19 Rino "):setClass("Drone", "Mines Layer"):setModel("LindwurmFighterYellow")
template:setHasReactor(false)
template:setRadarTrace("radar_fighter.png")
template:setDescription([[Heavy duty, mass-cargo drone-squad used to handle dangerous materiel. The Rino (not to be confused with the Rhino model of Dreadnaughts) model is an Earth-Federated commissioned piece of equipment that takes up to three standard Drone Arrays. 
In fact, a single Rino model is an array of three to seven  hive-linked drones working to tow a row of specially designed mines or other sort s of fielded equipment and quickly disseminate it to target location with minimal needed guidance and according to plan. Point and click really.
]])
template:setEnergyStorage(300)
template:setHull(150)
template:setSpeed(60, 50, 20)
template:setCombatManeuver(400, 250)
template:setTubes(1, 15.0)
template:setWeaponStorage("Mine", 2)
template:setDefaultAI('fighter')
template:setJumpDrive(false)
template:setWarpSpeed(-1)
template:setRepairCrewCount(0)


--[[-----------------------Fighters-----------------------]]
template = ShipTemplate():setName("MT52 Hornet"):setClass("Starfighter", "Interceptor"):setModel("WespeScoutYellow")
template:setRadarTrace("radar_fighter.png")
template:setDescription([[The MT52 is a standing staple of anyone that wants a solid aerospace presence. Capable of fighting even in relative low orbit (though not in actual atmosphere) - squadrons of this Ophelia-Hertzog Inc. designed machines roam the space lanes as security detail - or more often, as flights stored within larger, carrier-cruisers.
It's old, reliable and have been retro-fitted more times then ever, commonly referred to as "Stinger" when upgraded to match modern standards or "Bumblebee" when still carrying the noisy, silhouette-breaking seeker arrays.
]])
template:setHull(30)
template:setShields(20)
template:setSpeed(120, 30, 30)
template:setCombatManeuver(600, 100)
template:setDefaultAI('fighter')
--                  Arc, Dir, Range, CycleTime, Dmg
template:setBeam(0, 30, 0, 700.0, 4.0, 4)
template:setCombatManeuver(200, 100)
template:setWarpSpeed(500)

template = ShipTemplate():setName("MU52 Super-Hornet"):setClass("Starfighter", "Interceptor"):setModel("WespeScoutRed")
template:setRadarTrace("radar_fighter.png")
template:setDescription([[The MU52, designed years later then the still-common MT52 - was supposed to be everything that the Hornet wasn't. Cheap, streamlined and vicious-punching aerospace superiority fighter.
Slightly larger then it's predecessor, each fighter also carries two slaved mini-Drones to allow wider scopes of attack and defense , making the MU52 the most sophisticated single-person weapon system in widespread use.
]])
template:setHull(40)
template:setShields(30)
template:setSpeed(130, 35, 30)
template:setCombatManeuver(600, 100)
template:setDefaultAI('fighter')
--                  Arc, Dir, Range, CycleTime, Dmg
template:setBeam(0, 30, 0, 700.0, 4.0, 4)
template:setBeam(1, 30, 0, 700.0, 4.0, 4)
template:setTubes(1, 15.0)
template:setWeaponStorage("HVLI", 6)
template:setCombatManeuver(300, 200)
template:setWarpSpeed(100)

template = ShipTemplate():setName("Adder MK5"):setClass("Starfighter", "Gunship"):setModel("AdlerLongRangeScoutYellow")
template:setRadarTrace("radar_fighter.png")
template:setDescription([[FSN Issued advanced fighter gunship, can also be seen in corporate security forces.
The Adder MK5 is heavy a gunship-model fighter with a crew of 3 personnel, capable of multi-tasking as a short-distance bomber with it's increased cargo hold. With every model of this series, the FSN had integrated more engine power and stronger support systems to allow these machines achieve higher velocity of delivery - achieving what is essentially a three-dimensional dive-bomber capable of delivering multitude of munitions at extreme precision. 
Most of it's other systems are defensive in nature, point-defense cannons and other flak and chaff to secure a deadly delivery.
]])
template:setHull(50)
template:setShields(40)
template:setSpeed(110, 25, 25)
template:setBeam(0, 35, 0, 900, 5.0, 4.0)
template:setBeam(1, 70, 35, 600, 5.0, 4.0)
template:setBeam(2, 70, -35, 600, 5.0, 4.0)
template:setCombatManeuver(400, 250)
template:setTubes(2, 12.0)
template:setWeaponStorage("HVLI", 8)
template:setWeaponStorage("Homing", 4)
template:setTubeDirection(0, 0)
template:setTubeDirection(1, 90)
template:setTubeDirection(2, -90)
template:setWarpSpeed(80)

--[[-----------------------Corvette-----------------------]]
template = ShipTemplate():setName("Phobos T3"):setClass("Corvette", "Cruiser"):setModel("small_frigate_3")
template:setDescription([[Known as Project 29 of Komurat-Enzin Inc. wholly designed, developed and constructed by the same corporation and sold out under strict patent laws. These Corvettes are known for their high grade materials and high end systems, but also for their quirky design and substandard living conditions making them hated for peacekeeper forces while being highly valued as combat vessels.
The Phobos is uniquely coated with stealth materials and has an array of unique stealth drones with very high levels of autonomy, allowing it to act as a 'sniper' and as a 'hunter' system against targets up to the size of a Destroyer.
It however has dastardly cargo holds and needs constant resupply.
]])
template:setDockClasses("Drone")
template:setHull(80)
template:setShields(50, 40)
template:setSpeed(80, 10, 10)
template:setCombatManeuver(250, 150)
template:setBeamWeapon(0, 90, -15, 1200, 8, 6)
template:setBeamWeapon(1, 90,  15, 1200, 8, 6)
template:setTubes(2, 60.0)
template:setWeaponStorage("HVLI", 12)
template:setWeaponStorage("Homing", 6)
template:setTubeDirection(0, 1)
template:setTubeDirection(1, 1)
template:setWarpSpeed(150)


template = ShipTemplate():setName("Centurion"):setClass("Corvette", "Cruiser"):setModel("small_frigate_2")
template:setDescription([[They are the largest corvette warships in the federated naval fleet. Although classified as small "corvettes" due to their small size and crew of only 87, their weaponry and speed are almost comparable to that of a frigate. They are equipped with targeting arrays, torpedoes, drone launchers, electronic warfare capabilities and decoys, point defense mounts, and a fighter pad for a squadron of two.
]])
template:setDockClasses("Drone")
template:setHull(120)
template:setShields(80, 60)
template:setSpeed(90, 10, 10)
template:setCombatManeuver(250, 150)
template:setBeamWeapon(0, 90, -15, 1200, 6, 6)
template:setBeamWeapon(1, 90,  15, 1200, 6, 6)
template:setTubes(2, 60.0)
template:setWeaponStorage("HVLI", 20)
template:setWeaponStorage("Homing", 8)
template:setWeaponStorage("EMP", 4)
template:setTubeDirection(0, -1)
template:setTubeDirection(1,  1)
template:setWarpSpeed(200)


--[[-----------------------Frigate------------------------]]

template = ShipTemplate():setName("Havoc MK3"):setClass("Frigate", "Cruiser"):setModel("LightCorvetteRed")
template:setDescription([[Havoc MK3 class is of Shield-Role frigates was developed by a Martian conglomerate. With the design based heavily on the earlier 'Hammerhead' models of the same consortium. Heavy duty fleet defense platforms with heavy shielding, reinforced bulkheads and high-yield Point-Defense arrays as well as swarm-drones for close quarters. Their offensive capabilities are somewhat limited to their class, but a staying power that is still respected in many an outer-rim fleets.
]])
template:setDockClasses("Drone")
template:setHull(120)
template:setShields(100, 80)
template:setSpeed(80, 10, 10)
template:setCombatManeuver(250, 150)
template:setBeamWeapon(0, 35, -15, 1200, 6, 6)
template:setBeamWeapon(1, 35,  15, 1200, 6, 6)
template:setBeamWeapon(2, 15,  0, 1600, 8, 8)
template:setTubes(2, 60.0)
template:setWeaponStorage("HVLI", 20)
template:setWeaponStorage("Homing", 10)
template:setWeaponStorage("EMP", 6)
template:setTubeDirection(0, -1)
template:setTubeDirection(1,  1)
template:setWarpSpeed(200)

template = ShipTemplate():setName("Longbow MK5"):setClass("Frigate", "Heavy Cruiser"):setModel("LightCorvetteGrey")
template:setDescription([[Vessels of this class were built primarily for with long-range space combat, although they can perform anti-surface and anti-fighter missions if pressed. Their sensor arrays can track up any number of hostiles across any given battlefield. 
As a rule these vessels represent a significant improvement over the original Longboat MK4 for several reasons. The Longbow MK5 has a heavier main gunnery array, improved CIWS, better sensors, and more drone swarms. In addition, the HQ-167 fitted on Longboat 5 have an improved range and can attack targets from all angles.
]])
template:setDockClasses("Drone")
template:setHull(150)
template:setShields(80, 60)
template:setSpeed(90, 10, 10)
template:setCombatManeuver(350, 150)
template:setBeamWeapon(0, 45, 0, 1200, 6, 6)
template:setBeamWeapon(1, 30, 180, 800, 6, 6)
template:setBeamWeapon(2, 15,  0, 2500, 10, 10)
template:setTubes(3, 10.0)
template:setWeaponStorage("HVLI", 20)
template:setWeaponStorage("Homing", 8)
template:setWeaponStorage("EMP", 4)
template:setTubeDirection(0, -1)
template:setTubeDirection(1,  1)
template:setTubeDirection(1,  180)
template:setWarpSpeed(250)


--[[-----------------------Destroyer----------------------]]

template = ShipTemplate():setName("Starhammer II"):setClass("Destroyer", "Heavy Assault"):setModel("battleship_destroyer_4_upgraded")
template:setDescription([[Designed and produced by MetaExperis and allied corporations - The mighty StarHammer II has learned the lessons of the Actium Secundus policing action, having much to improve upon the Starhammer Prime model. Faster, with thicker armor and still the same battery of famous Promachos rapid-fire assault cannons. A formation of four such vessels is the golden standard for firepower and heavy staying force between Sol and the Oort Cloud.
]])
template:setRadarTrace("radar_dread.png")
template:setDockClasses("Drone")
template:setHull(200)
template:setShields(250, 250)
template:setSpeed(80, 6, 10)
--                  Arc, Dir, Range, CycleTime, Dmg
template:setBeam(0, 60, -10, 2000.0, 8.0, 11)
template:setBeam(1, 60,  10, 2000.0, 8.0, 11)
template:setBeam(2, 60, -20, 1500.0, 8.0, 11)
template:setBeam(3, 60,  20, 1500.0, 8.0, 11)
template:setTubes(2, 10.0)
template:setWeaponStorage("HVLI", 20)
template:setWeaponStorage("Homing", 12)
template:setWeaponStorage("EMP", 6)
template:setWarpSpeed(250)
template:setCombatManeuver(350, 150)


--[[---------------------BattleCruiser--------------------]]

template = ShipTemplate():setName("Atlantis X23"):setClass("BattleCruiser", "Spearhead"):setModel("battleship_destroyer_1_upgraded")
template:setDescription([[A battlecruiser is a frightful sight when it enters engagement range, shadowing over entire city blocks. The Atlantis, much like it’s namesake, can sink remarkably low in orbit to deliver bombardment at uncanny precision. Speedy and quick to change direction - the Atlantis is the ballistic operations center, with advanced communication and computation arrays to serve at fleet-wide capacity.
]])
template:setRadarTrace("radar_dread.png")
template:setHull(250)
template:setShields(200, 200, 200, 200)
template:setSpeed(60, 5, 7)
template:setWarpSpeed(250)
--                  Arc, Dir, Range, CycleTime, Dmg
template:setBeam(0,100, -20, 1500.0, 6.0, 8)
template:setBeam(1,100,  20, 1500.0, 6.0, 8)
template:setBeam(2,100, 180, 1500.0, 6.0, 8)
template:setTubes(4, 10.0)
template:setWeaponStorage("HVLI", 20)
template:setWeaponStorage("Homing", 12)
template:setWeaponStorage("EMP", 6)
template:setWeaponStorage("Nuke", 2)
template:setTubeDirection(0, -90)
template:setTubeDirection(1, -90)
template:setTubeDirection(2,  90)
template:setTubeDirection(3,  90)

--[[----------------------BattleShip----------------------]]

template = ShipTemplate():setName("Nimitz X53"):setClass("BattleShip", "Flagship"):setModel("battleship_destroyer_4_upgraded")
template:setDescription([[Crafted exclusively on Lunar dockyards for the Federation by the Callisto Munitions Corporations - the X53 is the largest Nimitz class Battleship available, capable of standing pound for pound with anything out there. With three coaxial rail accelerators, hardened armor and improved compartmentalization for tough hits, this leviathan contains more trade secrets and nasty wallop than any other in it’s class. The Nimitz takes the battlefields of tomorrow, today!
>> An excerpt from Callisto Munitions Advert during the Teeth of the Heavens convention on Luna.]])
template:setRadarTrace("radar_dread.png")
template:setHull(450)
template:setShields(300, 300, 300, 300)
template:setSpeed(60, 5, 7)
template:setWarpSpeed(250)
--                  Arc, Dir, Range, CycleTime, Dmg
template:setBeam(0,100, -20, 1500.0, 6.0, 8)
template:setBeam(1,100,  20, 1500.0, 6.0, 8)
template:setBeam(2,100, 180, 1500.0, 6.0, 8)
template:setBeam(3,30, 0, 3000.0, 16.0, 26)
template:setTubes(4, 10.0)
template:setWeaponStorage("HVLI", 20)
template:setWeaponStorage("Homing", 16)
template:setWeaponStorage("EMP", 6)
template:setWeaponStorage("Nuke", 2)
template:setTubeDirection(0, -90)
template:setTubeDirection(1, -90)
template:setTubeDirection(2,  90)
template:setTubeDirection(3,  90)


--[[-----------------------Freighters---------------------]]

template = ShipTemplate():setName("Handymax "):setClass("Freighter", "Light Freighter"):setModel("transport_2_1")
template:setDescription([[Small shipping freighter with little luxury on board. With auster crew compartments, it’s little more then two loading Spars for space-borne cargo and a landing mechanism with an engine strapped on. The Handymax is favored by anyone that likes flexibility and prefers speed to bulk.
]])
template:setHull(100)
template:setShields(50, 50)
template:setSpeed(70, 7, 10)
template:setRadarTrace("radar_transport.png")

template = ShipTemplate():setName("Supramax "):setClass("Freighter", "Medium Freighter"):setModel("transport_2_2")
template:setDescription([[The workhorse of most trading companies - the Supermax has three Spars for bulk loading and includes two enclosed compartments for travelers as well as a crew compartment. Retrofitted to fill any number of roles, the Supermax Medium Freighter has been updated for years now and the solid layout of the ship had made it a favorite among shipping fleets and cruise liners alike.
]])
template:setHull(125)
template:setShields(60, 60)
template:setSpeed(60, 7, 10)
template:setRadarTrace("radar_transport.png")

template = ShipTemplate():setName("VLCC"):setClass("Freighter", "Heavy Freighter"):setModel("transport_2_3")
template:setDescription([[Huge, oval and very much a lumbering beast. VLCC is built almost like a rocket, with minimal crew and maximum automation to allow an ‘aim and forget’ sort of a mission, loading and unloading them sometimes encounters higher-then-normal accident rate. 
Remember soldier, Safety is an Order!
]])
template:setHull(150)
template:setShields(75, 75)
template:setSpeed(50, 6, 10)
template:setRadarTrace("radar_transport.png")

template = ShipTemplate():setName("Armored VLCC"):setClass("Freighter", "Armored Freighter"):setModel("transport_3_3")
template:setDescription([[Originally used and developed to move military material and personnel between around the solar system, this model had been retired to Civilian service after newer craft had been developed. Now considered to be top-of-the-line craft for private corporations and organisations that value shielding and studiness to bulk of cargo.
While severely under-armed and under-armored compared to other vessels of its size, the Armored VLCC can and will take place in a battle line when pressed, and recent retro-fitting sightings had shown that with some expertise, it can be upgraded to be a Ship of the Line to a worrisome degree.
]])
template:setHull(250)
template:setShields(150, 150)
template:setSpeed(40, 5, 10)
template:setRadarTrace("radar_transport.png")

--[[-----------------------Stations---------------------]]

template = ShipTemplate():setName("Small Station"):setModel("space_station_4"):setType("station")
template:setDescription([[stening stations, and security checkpoints. Crews turn over frequently in a small station's cramped accommodatations, but they are small enough to look like ships on many long-range sensors, and organized raiders sometimes take advantage of this by placing small stations in nebulae to serve as raiding bases. They are lightly shielded and vulnerable to swarming assaults.]])
template:setHull(150)
template:setShields(300)
template:setRadarTrace("radartrace_smallstation.png")
-- template:setRepairDocked(false)

template = ShipTemplate():setName("Medium Station"):setModel("space_station_3"):setType("station")
template:setDescription([[Large enough to accommodate small crews for extended periods of times, stations of this size are often trading posts, refuelling bases, mining operations, and forward military bases. While their shields are strong, concerted attacks by many ships can bring them down quickly.]])
template:setHull(400)
template:setShields(800)
template:setRadarTrace("radartrace_mediumstation.png")
-- template:setRepairDocked(false)

template = ShipTemplate():setName("Large Station"):setModel("space_station_2"):setType("station")
template:setDescription([[These spaceborne communities often represent permanent bases in a sector. Stations of this size can be military installations, commercial hubs, deep-space settlements, and small shipyards. Only a concentrated attack can penetrate a large station's shields, and its hull can withstand all but the most powerful weaponry.]])
template:setHull(500)
template:setShields(1000, 1000, 1000)
template:setRadarTrace("radartrace_largestation.png")
-- template:setRepairDocked(false)

template = ShipTemplate():setName("Huge Station"):setModel("space_station_1"):setType("station")
template:setDescription([[The size of a sprawling town, stations at this scale represent a faction's center of spaceborne power in a region. They serve many functions at once and represent an extensive investment of time, money, and labor. A huge station's shields and thick hull can keep it intact long enough for reinforcements to arrive, even when faced with an ongoing siege or massive, perfectly coordinated assault.]])
template:setHull(800)
template:setShields(1200, 1200, 1200, 1200)
template:setRadarTrace("radartrace_hugestation.png")
-- template:setRepairDocked(false)

template = ShipTemplate():setName("Relay Station"):setModel("space_station_4"):setType("station")
template:setDescription([[stening stations, and security checkpoints. Crews turn over frequently in a small station's cramped accommodatations, but they are small enough to look like ships on many long-range sensors, and organized raiders sometimes take advantage of this by placing small stations in nebulae to serve as raiding bases. They are lightly shielded and vulnerable to swarming assaults.]])
template:setHull(150)
template:setShields(300)
template:setRadarTrace("radartrace_smallstation.png")
-- template:setRepairDocked(false)