--[[                  Vaisseaux légers
Rappel des catégories : 
Vaisseau léger : 1 à 3 places ; aucun moteur Warp ou Jump possible ; shipTemplates_VaisseauxLegers.lua ;
Frégate : 4 à 9 places ; shipTemplates_Fregates.lua ;
Destroyer : 10 à 50 places ; shipTemplates_Escorteurs.lua ;
Croiseur Léger : 50 à 100 places ; shipTemplates_CroiseursLegers.lua ;
Croiseur : 100 à 250 places ; shipTemplates_Croiseurs.lua ;
Cuirassé : 250 à 500 places ; shipTemplates_Cuirasse.lua ;
Porte-Vaisseaux : plus de 500 places; shipTemplates_Capitaux.lua ;
Drones et Créatures : N.C ; shipTemplates_Autres.lua ;
Stations;

Les frégates sont à une taille au-dessus des chasseurs stellaires.
Ils ont généralement 2 sections de bouclier ou plus, mais rarement plus de 4.

Cette classe de navires n'est normalement pas équipée de commandes de saut ou de chaîne. Mais dans certains cas, les navires sont modifiés pour les inclure, ou pour certains rôles dans lesquels il est intégré.

Ils sont divisés en sous-classes différentes:
* Combat: frégates armées, axées sur le combat. Ceux-ci viennent dans différents rôles.
* Transport léger: Petits transports, comme transporter jusqu'à 50 soldats dans des conditions spartiates ou quelques diplomates dans le luxe. Selon le rôle peut avoir des armes.
* Support: Les types de support sont disponibles dans de nombreuses variantes. Ils sont simplement une coque de frégate équipée de tout ce qui était nécessaire. Tout, des couches de mines aux navires scientifiques.
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


--[[Frégate Imperial Sword]]
template = ShipTemplate():setName("Fregate Imperial Sword"):setClass("Fregate", "Combat"):setModel("Navy_Frigate")
template:setDescription([[Les fregates de classe Sword sont des vaisseaux dont la conception remonte a des milliers d'annee et qui servent la Flotte Imperiale depuis lors sans faillir.
Les nombreux conflits auxquels les fregates Sword prirent part permirent de valider l'ensemble des composantes de ce type de vaisseau.]])
template:setHull(650)
template:setShields(250, 250)
template:setSpeed(95, 10, 12)
--                  Arc, Dir, Range, CycleTime, Dmg
template:setBeam(0, 100, 90, 1200.0, 1.5, 12)
template:setBeam(1, 100,-90, 1200.0, 1.5, 12)
--                                Arc, Dir, Rotate speed
--       Tubes
template:setTubes(3, 12.0)
template:setTubeDirection(0, 0)
template:setTubeDirection(1, 90)
template:setTubeDirection(2, -90)
--Macro-canon : (rafale, non homing) 
template:setCustomWeapon("HVLI", "MCAN", 3, 1000.0, 3, "Kinetic")
template:setCustomWeaponColor("MCAN", 255, 150, 103)
template:setCustomWeaponStorage("MCAN", 30)
--TG Seeker ( homing) 
template:setCustomWeapon("Homing", "SEEK", 2.5, 600.0, 1, "Kinetic")
template:setCustomWeaponColor("SEEK", 255, 204, 0)
template:setCustomWeaponStorage("SEEK", 12)
--DockClasses
template:setDockClasses("Drone")

--[[Frégate Imperial Firestorm]]
variation = template:copy("Fregate Imperial Firestorm"):setModel("Navy_Frigate")
variation:setLocaleName("Fregate Imperial Firestorm")
variation:setDescription([[La fregate de classe Firestorm est une variante recente de la Fregate Sword destine a associer la rapidite de manœuvre de l'escorteur a la puissance de feu superieure d'une Piece d'artillerie navale.
La fregate Sword fut donc modifiee pour pouvoir diriger une partie du flux laser du vaisseau vers le canon imposant monte sous sa proue.]])
variation:setHull(800)
variation:setShields(250, 250)
variation:setSpeed(95, 12, 15)
--                  Arc, Dir, Range, CycleTime, Dmg
variation:setBeam(2,30, 0, 3500.0, 30.0, 150)

--[[Frégate Firestorm]]
variation = variation:copy("Fregate Firestorm"):setType("playership")
variation:setLocaleName("Fregate Firestorm")
variation:setHull(800)
variation:setShields(125, 125)
variation:setSpeed(95, 12, 15)
--                  Arc, Dir, Range, CycleTime, Dmg
variation:setBeam(2,10, 0, 3500.0, 30.0, 150)
-- PJ
variation:setCombatManeuver(400, 250)
variation:addRoomSystem(1, 0, 2, 1, "Maneuver");
variation:addRoomSystem(1, 1, 2, 1, "BeamWeapons");
variation:addRoom(2, 2, 2, 1);
variation:addRoomSystem(0, 3, 1, 2, "RearShield");
variation:addRoomSystem(1, 3, 2, 2, "Reactor");
variation:addRoomSystem(3, 3, 2, 2, "Warp");
variation:addRoomSystem(5, 3, 1, 2, "JumpDrive");
variation:addRoom(6, 3, 2, 1);
variation:addRoom(6, 4, 2, 1);
variation:addRoomSystem(8, 3, 1, 2, "FrontShield");
variation:addRoom(2, 5, 2, 1);
variation:addRoomSystem(1, 6, 2, 1, "MissileSystem");
variation:addRoomSystem(1, 7, 2, 1, "Impulse");
variation:addDoor(1, 1, true);
variation:addDoor(2, 2, true);
variation:addDoor(3, 3, true);
variation:addDoor(1, 3, false);
variation:addDoor(3, 4, false);
variation:addDoor(3, 5, true);
variation:addDoor(2, 6, true);
variation:addDoor(1, 7, true);
variation:addDoor(5, 3, false);
variation:addDoor(6, 3, false);
variation:addDoor(6, 4, false);
variation:addDoor(8, 3, false);
variation:addDoor(8, 4, false); 
--Airlock doors
--variation:addDoor(2, 2, false);
--variation:addDoor(2, 5, false);


--[[Transport Leger Civil]]
template = ShipTemplate():setName("Transport Leger Civil"):setClass("Fregate", "Transport Leger"):setModel("Civilian_Transport_Ship")
template:setRadarTrace("radar_tug.png")
template:setDescription([[Ce transport leger civil est fait de bric et de broc, souvent utilise par les marchands au portefeuille leger. Ne cherchez pas la fiabilite il vol c'est deja pas mal !]])
template:setHull(600)
template:setShields(100, 100)
template:setSpeed(95, 10, 12)
--                  Arc, Dir, Range, CycleTime, Dmg
template:setBeam(0, 100, 90, 1500.0, 1.5, 6)
template:setBeam(1, 100, -90, 1500.0, 1.5, 6)
--       Tubes
template:setTubes(2, 12.0)
template:setTubeDirection(0, 90)
template:setTubeDirection(1, -90)
--Macro-canon : (rafale, non homing) 
template:setCustomWeapon("HVLI", "MCAN", 3, 1000.0, 3, "Kinetic")
template:setCustomWeaponColor("MCAN", 255, 150, 103)
template:setCustomWeaponStorage("MCAN", 10)
--MGAA Skyreaper simple pour les chasseur/petit vaisseau ( homing trés rapide ) 
template:setCustomWeapon("Homing", "SKY1", 0.75, 1000.0, 1, "Kinetic")
template:setCustomWeaponColor("SKY1", 255, 204, 0)
template:setCustomWeaponStorage("SKY1", 12)
--DockClasses
template:setDockClasses("Drone")
