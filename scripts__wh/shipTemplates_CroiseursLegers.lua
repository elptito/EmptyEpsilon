--[[                  Croiseurs legers
Rappel des catégories : 
Vaisseau leger : 1 à 3 places ; aucun moteur Warp ou Jump possible ; shipTemplates_VaisseauxLegers.lua ;
Frégate : 4 à 9 places ; shipTemplates_Fregates.lua ;
Destroyer : 10 à 50 places ; shipTemplates_Escorteurs.lua ;
Croiseur leger : 50 à 100 places ; shipTemplates_CroiseursLegers.lua ;
Croiseur : 100 à 250 places ; shipTemplates_Croiseurs.lua ;
Cuirassé : 250 à 500 places ; shipTemplates_Cuirasse.lua ;
Porte-Vaisseaux : plus de 500 places; shipTemplates_Capitaux.lua ;
Drones et Créatures : N.C ; shipTemplates_Autres.lua ;
Stations;

Le croiseur leger est un vaisseau d'attaque rapide qui sacrifie une partie de sa puissance au profit de la rapidité.
----------------------------------------------------------]]

function couleur(template,name,model)
	for _, color in ipairs({"Blue", "Green", "Grey", "Red", "White", "Yellow"}) do
		variation = template:copy(name .. " " .. color):setModel(model .. color):setSecret(true)
	end
end

function PZ(template,name,model)
	for _, color in ipairs({"-IMP-","-CHAOS-","-ELDAR-","-TAU-","-TRADE-","-MECH-"}) do
		variation = template:copy(name .. " " .. color):setModel(model .. color):setSecret(true)
	end
end


--[[Croiseur leger Dauntless]]
template = ShipTemplate():setName("Croiseur leger Dauntless"):setClass("Croiseur leger", "Combat"):setModel("Navy_Light_Cruiser"):setType("playership")

template:setRadarTrace("radar_ktlitan_destroyer.png")
template:setDescription([[Le croiseur leger de classe Dauntless est le vaisseau emblematique de l'ensemble de la Flotte Imperiale, present dans tous les Segmentae Majoris.
Le Dauntless est un croiseur alliant la vitesse et la manoeuvrabilite d'une fregate a la puissance de feu superieure d'un croiseur.
Les vaisseaux de ce type sont reconnus et prises par tous les officiers de la Flotte car ils peuvent aussi bien patrouiller seuls sans soutien logistique pendant une longue duree que proteger les autres vaisseaux de lignes des escorteurs adverses.]])
template:setHull(900)
template:setShields(300, 300)
template:setSpeed(90, 20, 12)

template:setPublicName("Croiseur leger DauntlessMK3")


--                  Arc, Dir, Range, CycleTime, Dmg
template:setBeam(0, 10, 0, 3500.0, 30.0, 150)
template:setBeam(1, 100, 90, 2000.0, 7.0, 50)
template:setBeam(2, 100, -90, 2000.0, 7.0, 50)
template:setBeam(3, 100, 90, 1500.0, 1.5, 12)
template:setBeam(4, 100,-90, 1500.0, 1.5, 12)
--                              Arc, Dir, Rotate speed

--       Tubes
template:setTubes(5, 12.0)
template:setTubeDirection(0, 0)
template:setTubeDirection(1, 90)
template:setTubeDirection(2, -90)
template:setTubeDirection(3, 90)
template:setTubeDirection(4, -90)
--MGAA Skyreaper (homing trés rapide) fregrate et plus
template:setCustomWeapon("Homing", "SKYR", 0.75, 1000.0, 3, "Kinetic")
template:setCustomWeaponColor("SKYR", 255, 204, 0)
template:setCustomWeaponStorage("SKYR", 12)
--MGAA Flakburst (nuke faible dommage) 
template:setCustomWeapon("Nuke", "FLAK", 0.5, 500.0, 1, "Kinetic")
template:setCustomWeaponColor("FLAK", 220, 87, 20)
template:setCustomWeaponStorage("FLAK", 1)
--Macro-canon : (rafale, non homing) 
template:setCustomWeapon("HVLI", "MCAN", 3, 1000.0, 3, "Kinetic")
template:setCustomWeaponColor("MCAN", 255, 150, 103)
template:setCustomWeaponStorage("MCAN", 24)
--TG Seeker (homing) 
template:setCustomWeapon("Homing", "SEEK", 2.5, 600.0, 1, "Kinetic")
template:setCustomWeaponColor("SEEK", 255, 204, 0)
template:setCustomWeaponStorage("SEEK", 12)
--DockClasses
template:setDockClasses("Drone","Vaisseau leger")


--MGAA Flakburst (nuke faible dommage) 
template:setCustomWeapon("Nuke", "FLAK", 0.5, 500.0, 1, "Kinetic")
template:setCustomWeaponColor("FLAK", 220, 87, 20)
template:setCustomWeaponStorage("FLAK", 2)
-- PJ
template:setJumpDrive(true)
template:setCombatManeuver(250, 150)
template:setRepairCrewCount(8)
template:addRoom(1, 0, 6, 1)
template:addRoom(1, 5, 6, 1)
template:addRoomSystem(0, 1, 2, 2, "RearShield")
template:addRoomSystem(0, 3, 2, 2, "MissileSystem")
template:addRoomSystem(2, 1, 2, 2, "Beamweapons")
template:addRoomSystem(2, 3, 2, 2, "Reactor")
template:addRoomSystem(4, 1, 2, 2, "Warp")
template:addRoomSystem(4, 3, 2, 2, "JumpDrive")
template:addRoomSystem(6, 1, 2, 2, "Impulse")
template:addRoomSystem(6, 3, 2, 2, "Maneuver")
template:addRoomSystem(8, 2, 2, 2, "FrontShield")
template:addDoor(1, 1, true)
template:addDoor(3, 1, true)
template:addDoor(4, 1, true)
template:addDoor(6, 1, true)
template:addDoor(4, 3, true)
template:addDoor(5, 3, true)
template:addDoor(8, 2, false)
template:addDoor(8, 3, false)
template:addDoor(1, 5, true)
template:addDoor(2, 5, true)
template:addDoor(5, 5, true)
template:addDoor(6, 5, true)




--[[Croiseur leger Imperial Dauntless]]
template = ShipTemplate():setName("Croiseur leger Dauntless MK1"):setClass("Croiseur leger", "Combat"):setModel("Navy_Light_Cruiser")

template:setRadarTrace("radar_ktlitan_destroyer.png")
template:setDescription([[Le croiseur leger de classe Dauntless est le vaisseau emblematique de l'ensemble de la Flotte Imperiale, present dans tous les Segmentae Majoris.
Le Dauntless est un croiseur alliant la vitesse et la manoeuvrabilite d'une fregate a la puissance de feu superieure d'un croiseur.
Les vaisseaux de ce type sont reconnus et prises par tous les officiers de la Flotte car ils peuvent aussi bien patrouiller seuls sans soutien logistique pendant une longue duree que proteger les autres vaisseaux de lignes des escorteurs adverses.]])
template:setHull(900)
template:setShields(300, 300)
template:setSpeed(90, 10, 12)
--                  Arc, Dir, Range, CycleTime, Dmg
template:setBeam(0, 10, 0, 3500.0, 30.0, 150)
template:setBeam(1, 100, 90, 2000.0, 7.0, 50)
template:setBeam(2, 100, -90, 2000.0, 7.0, 50)
template:setBeam(3, 100, 90, 1500.0, 1.5, 12)
template:setBeam(4, 100,-90, 1500.0, 1.5, 12)
--                              Arc, Dir, Rotate speed

--       Tubes
template:setTubes(5, 12.0)
template:setTubeDirection(0, 0)
template:setTubeDirection(1, 90)
template:setTubeDirection(2, -90)
template:setTubeDirection(3, 90)
template:setTubeDirection(4, -90)
--MGAA Skyreaper (homing trés rapide) fregrate et plus
template:setCustomWeapon("Homing", "SKYR", 0.75, 1000.0, 3, "Kinetic")
template:setCustomWeaponColor("SKYR", 255, 204, 0)
template:setCustomWeaponStorage("SKYR", 12)
--MGAA Flakburst (nuke faible dommage) 
template:setCustomWeapon("Nuke", "FLAK", 0.5, 500.0, 1, "Kinetic")
template:setCustomWeaponColor("FLAK", 220, 87, 20)
template:setCustomWeaponStorage("FLAK", 1)
--Macro-canon : (rafale, non homing) 
template:setCustomWeapon("HVLI", "MCAN", 3, 1000.0, 3, "Kinetic")
template:setCustomWeaponColor("MCAN", 255, 150, 103)
template:setCustomWeaponStorage("MCAN", 24)
--TG Seeker (homing) 
template:setCustomWeapon("Homing", "SEEK", 2.5, 600.0, 1, "Kinetic")
template:setCustomWeaponColor("SEEK", 255, 204, 0)
template:setCustomWeaponStorage("SEEK", 12)
--DockClasses
template:setDockClasses("Drone","Vaisseau leger")


--[[Croiseur leger Corsaire]]
template = ShipTemplate():setName("Croiseur leger Corsaire"):setClass("Croiseur leger", "Combat"):setModel("Eldar_Corsair_Light_Cruiser")
template:setRadarTrace("radar_ktlitan_destroyer.png")
template:setDescription([[Le Croiseur leger Corsair est le plus grand des vaisseaux qui composent generalement les flottes des Eldars. Comme beaucoup de vaisseaux Xenos, le Croiseur leger Corsair est souvent confondu avec d'autres classes de croiseur et de nombreuses attaques sont ainsi attribuees faussement a leurs cousins Eldars.
Certains enregistrements pretent aux Croiseur leger Corsair des similitudes avec les classes Eclipse et Shadow alors que d'autres experts xenologues les considerent comme des amalgames de diverses classes. Les nombreux rapports contradictoires qui concernent le croiseur en font un appareil difficile a classer pour l'Imperium. Certains recits rapportent meme des combats entre vaisseaux Eldars, ce qui ajoute encore plus de confusion aux effort de categorisation imperiale.]])
template:setHull(700)
template:setShields(100, 100)
template:setSpeed(110, 15, 20)
--                  Arc, Dir, Range, CycleTime, Dmg

template:setBeam(0, 10, 0, 3500.0, 40.0, 150)
template:setBeam(1, 45, 0, 2000.0, 7.0, 50)
template:setBeam(2, 45, 0, 2000.0, 7.0, 50)
template:setBeam(3, 90, 90, 1500.0, 1.5, 12)
template:setBeam(4, 90,-90, 1500.0, 1.5, 12)
--                              Arc, Dir, Rotate speed

--       Tubes
template:setTubes(5, 12.0)
template:setTubeDirection(0, 0)
template:setTubeDirection(1, 90)
template:setTubeDirection(2, -90)
template:setTubeDirection(3, 90)
template:setTubeDirection(4, -90)
--Macro-canon : (rafale, non homing) 
template:setCustomWeapon("HVLI", "MCAN", 3, 1000.0, 3, "Kinetic")
template:setCustomWeaponColor("MCAN", 255, 150, 103)
template:setCustomWeaponStorage("MCAN", 32)
--TG Seeker (homing) 
template:setCustomWeapon("Homing", "SEEK", 2.5, 600.0, 1, "Kinetic")
template:setCustomWeaponColor("SEEK", 255, 204, 0)
template:setCustomWeaponStorage("SEEK", 24)
--MGAA Flakburst (nuke faible dommage) 
template:setCustomWeapon("Nuke", "FLAK", 0.5, 500.0, 1, "Kinetic")
template:setCustomWeaponColor("FLAK", 220, 87, 20)
template:setCustomWeaponStorage("FLAK", 5)
--TG fusionmelta (Nuke homing) 
template:setCustomWeapon("Nuke", "MELT", 2, 300.0, 1, "Kinetic")
template:setCustomWeaponColor("MELT", 204, 0, 0)
template:setCustomWeaponStorage("MELT", 1)
template:setWeaponStorage("EMP", 4)

--DockClasses
template:setDockClasses("Drone","Vaisseau leger")


--[[Croiseur leger du chaos Slaughter]]
template = ShipTemplate():setName("Croiseur leger du chaos Slaughter"):setClass("Croiseur leger", "Combat"):setModel("Chaos_Light_Cruiser")
template:setRadarTrace("radar_ktlitan_destroyer.png")
template:setDescription([[Le Croiseur Slaughter est un vaisseau de ligne caracterise par sa capacite a atteindre des vitesses importantes, rivalisant avec celles des escorteurs plus petits voire meme des vaisseaux Eldars.
Cette vitesse exceptionnelle pour un vaisseau de ce tonnage est obtenue grace a des moteurs utilisant des Bobines de Scartix, une Archeo-Technologie uniquement connue du Monde Forge de Sethelan.
Cette specificite se revela etre le point faible de la strategie imperiale de production de croiseur Slaughter lorsque l'un d'entre eux, le Dutiful se rebella et bombarda les installations de Sethelan, provoquant la perte des plans des Bobines Scartix.]])
template:setHull(900)
template:setShields(150, 150)
template:setSpeed(95, 10, 20)
--                  Arc, Dir, Range, CycleTime, Dmg
template:setBeam(0, 100, 0, 100.0, 30.0, 10)
template:setBeam(1, 100, 90, 2000.0, 7.0, 50)
template:setBeam(2, 100, -90, 2000.0, 7.0, 50)
template:setBeam(3, 100, 90, 1500.0, 1.5, 12)
template:setBeam(4, 100,-90, 1500.0, 1.5, 12)
--                              Arc, Dir, Rotate speed

--       Tubes
template:setTubes(5, 12.0)
template:setTubeDirection(0, 0)
template:setTubeDirection(1, 90)
template:setTubeDirection(2, -90)
template:setTubeDirection(3, 90)
template:setTubeDirection(4, -90)
--MGAA Skyreaper (homing trés rapide) fregrate et plus
template:setCustomWeapon("Homing", "SKYR", 0.75, 1000.0, 3, "Kinetic")
template:setCustomWeaponColor("SKYR", 255, 204, 0)
template:setCustomWeaponStorage("SKYR", 12)
--MGAA Flakburst (nuke faible dommage) 
template:setCustomWeapon("Nuke", "FLAK", 0.5, 500.0, 1, "Kinetic")
template:setCustomWeaponColor("FLAK", 220, 87, 20)
template:setCustomWeaponStorage("FLAK", 1)
--Macro-canon : (rafale, non homing) 
template:setCustomWeapon("HVLI", "MCAN", 3, 1000.0, 3, "Kinetic")
template:setCustomWeaponColor("MCAN", 255, 150, 103)
template:setCustomWeaponStorage("MCAN", 24)
--TG Seeker (homing) 
template:setCustomWeapon("Homing", "SEEK", 2.5, 600.0, 1, "Kinetic")
template:setCustomWeaponColor("SEEK", 255, 204, 0)
template:setCustomWeaponStorage("SEEK", 12)
--DockClasses
template:setDockClasses("Drone","Vaisseau leger")

--[Croiseur leger Slaughter]]
variation = variation:copy("Croiseur leger Slaughter"):setType("playership")
variation:setPublicName("Croiseur leger Slaughter")
-- PJ
variation:setWarpSpeed(500)
variation:setCombatManeuver(250, 150)
variation:setRepairCrewCount(8)
variation:addRoom(1, 0, 6, 1)
variation:addRoom(1, 5, 6, 1)
variation:addRoomSystem(0, 1, 2, 2, "RearShield")
variation:addRoomSystem(0, 3, 2, 2, "MissileSystem")
variation:addRoomSystem(2, 1, 2, 2, "Beamweapons")
variation:addRoomSystem(2, 3, 2, 2, "Reactor")
variation:addRoomSystem(4, 1, 2, 2, "Warp")
variation:addRoomSystem(4, 3, 2, 2, "JumpDrive")
variation:addRoomSystem(6, 1, 2, 2, "Impulse")
variation:addRoomSystem(6, 3, 2, 2, "Maneuver")
variation:addRoomSystem(8, 2, 2, 2, "FrontShield")
variation:addDoor(1, 1, true)
variation:addDoor(3, 1, true)
variation:addDoor(4, 1, true)
variation:addDoor(6, 1, true)
variation:addDoor(4, 3, true)
variation:addDoor(5, 3, true)
variation:addDoor(8, 2, false)
variation:addDoor(8, 3, false)
variation:addDoor(1, 5, true)
variation:addDoor(2, 5, true)
variation:addDoor(5, 5, true)
variation:addDoor(6, 5, true)