--[[                  Escorteurs alias Destroyers
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

L’escorteur (destroyer) est un puissant vaisseau de défense contre les vaisseaux moyens et lourds.
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


--[[Destroyer Imperial Cobra]]
template = ShipTemplate():setName("Destroyer Imperial Cobra"):setClass("Destroyer", "Combat"):setModel("Navy_Destroyer")
template:setDescription([[Les destroyers de classe Cobra composent la majeure partie de la flotte d'escorteurs de la Flotte de Guerre Obscuras notamment en raison de sa grande vitesse lui permettant de rivaliser avec la plupart des autres vaisseaux, y compris Eldar.]])
template:setHull(700)
template:setShields(200, 200)
template:setSpeed(105, 20, 25)
--                  Arc, Dir, Range, CycleTime, Dmg
template:setBeam(0, 100, 90, 1500.0, 1.5, 12)
template:setBeam(1, 100,-90, 1500.0, 1.5, 12)
--                                Arc, Dir, Rotate speed

--       Tubes
template:setTubes(4, 12.0)
template:setTubeDirection(0, 1)
template:setTubeDirection(1, -1)
template:setTubeDirection(2, 90)
template:setTubeDirection(3, -90)
--TG Seeker ( homing)
template:setCustomWeapon("Homing", "SEEK", 2.5, 600.0, 1, "Kinetic")
template:setCustomWeaponColor("SEEK", 255, 204, 0)
template:setCustomWeaponStorage("SEEK", 18)
--MGAA Skyreaper ( homing trés rapide ) fregrate et plus
template:setCustomWeapon("Homing", "SKYR", 0.75, 1000.0, 3, "Kinetic")
template:setCustomWeaponColor("SKYR", 255, 204, 0)
template:setCustomWeaponStorage("SKYR", 12)
--MGAA Flakburst (nuke faible dommage)
template:setCustomWeapon("Nuke", "FLAK", 0.5, 500.0, 1, "Kinetic")
template:setCustomWeaponColor("FLAK", 220, 87, 20)
template:setCustomWeaponStorage("FLAK", 2)
--DockClasses
template:setDockClasses("Drone","Vaisseau leger")


--[[Destroyer du chaos Iconoclast]]
variation = template:copy("Destroyer du chaos Iconoclast")
variation:setLocaleName("Destroyer du chaos Iconoclast")
variation:setModel("Chaos_Destroyer")
variation:setRadarTrace("radar_adv_striker.png")
variation:setDescription([[Le Destroyer Iconoclast est un vaisseau de conception simple reprenant les principales caracteristiques des differents escorteurs utilises par les Libre-Marchands ou la Flotte Marchande, ce qui fait qu'il peut etre produit dans pratiquement n'importe quel Dock Orbital de la Galaxie.
Le destroyer Iconoclast est redoute pour sa puissance de feu, important pour un vaisseau de cette taille, et constitue une menace permanente pour tous les vaisseaux navigants dans les immensite de l'espace, y compris pour les vaisseaux de ligne pouvant etre mis a mal par un escadron d'Iconoclast.]])