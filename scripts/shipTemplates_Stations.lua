--[[                  Stations
Rappel des catégories : 
Vaisseau léger : 1 à 3 places ; aucun moteur Warp ou Jump possible ; shipTemplates_VaisseauxLegers.lua ;
Frégate : 4 à 9 places ; shipTemplates_Fregates.lua ;
Destroyer : 10 à 50 places ; shipTemplates_Escorteurs.lua ;
Croiseur Léger : 50 à 100 places ; shipTemplates_CroiseursLegers.lua ;
Croiseur : 100 à 250 places ; shipTemplates_Croiseurs.lua ;
Cuirassé : 250 à 500 places ;
Stations;

Les Stations apporte un soutien aux vaisseaux de combat et sert d'atelier de réparations.
----------------------------------------------------------]]


template = ShipTemplate():setName("Small Station"):setModel("space_station_4"):setType("station")
template:setDescription([[ Petite station spatiale, elle sert pour le ravitaillement en espace profond]])
template:setHull(150)
template:setShields(300)
template:setRadarTrace("radartrace_smallstation.png")

template = ShipTemplate():setName("Medium Station"):setModel("space_station_3"):setType("station")
template:setDescription([[Large enough to accommodate small crews for extended periods of times, stations of this size are often trading posts, refuelling bases, mining operations, and forward military bases. While their shields are strong, concerted attacks by many ships can bring them down quickly.]])
template:setHull(400)
template:setShields(800)
template:setRadarTrace("radartrace_mediumstation.png")

template = ShipTemplate():setName("Large Station"):setModel("space_station_2"):setType("station")
template:setDescription([[De la taille d'une ville tentaculaire, les stations de cette échelle représentent le centre de la puissance spatiale de cette région. Elles remplissent plusieurs fonctions à la fois et représentent un investissement considérable en temps, en argent et en main d'oeuvre. Les boucliers et l'épaisse coque d'une énorme station peuvent la maintenir intacte assez longtemps pour que des renforts arrivent, même en cas de siège ou d'assaut massif parfaitement coordonné.]])
template:setHull(500)
template:setShields(1000, 1000, 1000)
template:setRadarTrace("radartrace_largestation.png")

template = ShipTemplate():setName("Huge Station"):setModel("space_station_1"):setType("station")
template:setDescription([[De la taille d'une ville tentaculaire, les stations de cette échelle représentent le centre de la puissance spatiale de cette région. Elles remplissent plusieurs fonctions à la fois et représentent un investissement considérable en temps, en argent et en main d'oeuvre. Les boucliers et l'épaisse coque d'une énorme station peuvent la maintenir intacte assez longtemps pour que des renforts arrivent, même en cas de siège ou d'assaut massif parfaitement coordonné.]])
template:setHull(800)
template:setShields(1200, 1200, 1200, 1200)
template:setRadarTrace("radartrace_hugestation.png")


--[[Grande station Imperial]]
template = ShipTemplate():setName("Grande station Imperial"):setModel("Navy_Station"):setType("station")
template:setDescription([[De la taille d'une ville tentaculaire, les stations de cette échelle représentent le centre de la puissance spatiale de cette région. Elles remplissent plusieurs fonctions à la fois et représentent un investissement considérable en temps, en argent et en main d'oeuvre. Les boucliers et l'épaisse coque d'une énorme station peuvent la maintenir intacte assez longtemps pour que des renforts arrivent, même en cas de siège ou d'assaut massif parfaitement coordonné.]])
template:setHull(3000)
template:setShields(1000, 1000, 1000, 1000)
template:setRadarTrace("radartrace_hugestation.png")
--                  Arc, Dir, Range, CycleTime, Dmg
template:setBeam(0, 100, 1, 100.0, 30.0, 350)
template:setBeam(1, 100, 180, 100.0, 30.0, 350)
template:setBeam(2, 100, 90, 100.0, 30.0, 350)
template:setBeam(3, 100, -90, 100.0, 30.0, 350)
template:setBeam(4, 100, 45, 2000.0, 7.0, 50)
template:setBeam(5, 100, -45, 2000.0, 7.0, 50)
template:setBeam(6, 100, 135, 2000.0, 7.0, 50)
template:setBeam(7, 100, -135, 2000.0, 7.0, 50)
template:setBeam(8, 100, 42, 1500.0, 1.5, 12)
template:setBeam(9, 100, -42, 1500.0, 1.5, 12)
template:setBeam(10, 100, 48, 1500.0, 1.5, 12)
template:setBeam(11, 100,-48, 1500.0, 1.5, 12)
template:setBeam(12, 100, 132, 1500.0, 1.5, 12)
template:setBeam(13, 100,-132, 1500.0, 1.5, 12)
template:setBeam(14, 100, 138, 1500.0, 1.5, 12)
template:setBeam(15, 100,-138, 1500.0, 1.5, 12)
--                              Arc, Dir, Rotate speed

--       Tubes
template:setTubes(10, 12.0)
template:setTubeDirection(0, 1)
template:setTubeDirection(1, -1)
template:setTubeDirection(2, 90)
template:setTubeDirection(3, -90)
template:setTubeDirection(4, 90)
template:setTubeDirection(5, -90)
template:setTubeDirection(6, 90)
template:setTubeDirection(7, -90)
template:setTubeDirection(8, 90)
template:setTubeDirection(9, -90)
--TG Seeker (homing) 
template:setCustomWeapon("Homing", "SEEK", 1.5, 800.0, 1, "Kinetic")
template:setCustomWeaponColor("SEEK", 255, 204, 0)
template:setCustomWeaponStorage("SEEK", 60)
--MGAA Skyreaper (homing trés rapide) fregrate et plus
template:setCustomWeapon("Homing", "SKYR", 0.75, 1000.0, 3, "Kinetic")
template:setCustomWeaponColor("SKYR", 255, 204, 0)
template:setCustomWeaponStorage("SKYR", 60)
--Macro-canon : (rafale, non homing) 
template:setCustomWeapon("HVLI", "MCAN", 3, 1000.0, 3, "Kinetic")
template:setCustomWeaponColor("MCAN", 255, 150, 103)
template:setCustomWeaponStorage("MCAN", 60)
--MGAA Flakburst (nuke faible dommage) 
template:setCustomWeapon("Nuke", "FLAK", 0.5, 500.0, 1, "Kinetic")
template:setCustomWeaponColor("FLAK", 220, 87, 20)
template:setCustomWeaponStorage("FLAK", 6)