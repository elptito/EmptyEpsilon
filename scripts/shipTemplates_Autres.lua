--[[               Drones et Créatures
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

bada bip, bada bop, whaddaya know?
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

--[[Drone Mechanicum]]
template = ShipTemplate():setName("Drone Mechanicum"):setType("Drone"):setModel("sci_fi_alien_ship_1")
template:setRadarTrace("radar_ktlitan_fighter.png")
template:setHull(250)
template:setSpeed(125,32,25)
template:setBeam(0, 150, 0, 1500.0, 1.5, 12)
template:setDefaultAI('fighter')


--[[Drone Inconnu]]
template = ShipTemplate():setName("Drone Inconnu"):setType("Drone"):setModel("sci_fi_alien_ship_4")
template:setRadarTrace("radar_ktlitan_drone.png")
template:setHull(250)
template:setSpeed(125,32,25)
template:setBeam(0, 150, 0, 1500.0, 1.5, 12)


--[[Signature Inconnue]]
template = ShipTemplate():setName("Signature Inconnue"):setType("Alien"):setModel("sci_fi_alien_ship_6")
template:setRadarTrace("radar_ktlitan_scout.png")
template:setHull(300)
template:setSpeed(195,32,25)
template:setBeam(0, 150, 0, 1500.0, 1.5, 12)