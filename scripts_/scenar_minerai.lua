
function init()

	-- Peut être prévoir de scanner tous les joueurs pour leur attribuer cela
	player = getPlayerShip(-1)
	
	-- Tableau des minerais
	
	-- par ligne : id, nom, proba de trouver, facilité de scanner, concentration max
	table_minerai = {} -- création d'une table
	-- table_minerai[1] = {"Fer",0.3,"facile",20}
	-- table_minerai[2] = {"Nickel",0.2,"moyen",20}
	-- table_minerai[3] = {"Silice",0.1,"moyen",15}
	-- table_minerai[4] = {"Vide",0.4,"facile",0}

	table_minerai[1] = {"Fer",0.20,"facile",20}
	table_minerai[2] = {"Nickel",0.2,"moyen",20}
	table_minerai[3] = {"Silice",0.15,"moyen",15}
	table_minerai[4] = {"Scandium",0.1,"facile",5}
	table_minerai[5] = {"Yttrium",0.05,"moyen",5}
	table_minerai[6] = {"Cerium",0.05,"difficile",5}
	table_minerai[7] = {"Praseodyme",0.1,"difficile",5}
	table_minerai[8] = {"Vide",0.15,"facile",0}
	
	table_minerai_nebula = {}
	-- table_minerai_nebula[1] = {"Fer",0.1,"facile",20}
	-- table_minerai_nebula[2] = {"Nickel",0.1,"moyen",20}
	-- table_minerai_nebula[3] = {"Silice",0.05,"moyen",15}
	-- table_minerai_nebula[4] = {"Scandium",0.15,"facile",5}
	-- table_minerai_nebula[5] = {"Yttrium",0.2,"moyen",5}
	-- table_minerai_nebula[6] = {"Cerium",0.1,"difficile",5}
	-- table_minerai_nebula[7] = {"COIN",0.1,"difficile",5}
	-- table_minerai_nebula[8] = {"Vide",0.2,"facile",0}
	
	table_minerai_nebula[1] = {"Fer",0.05,"facile",20}
	table_minerai_nebula[2] = {"Nickel",0.05,"moyen",20}
	table_minerai_nebula[3] = {"Silice",0.05,"moyen",15}
	table_minerai_nebula[4] = {"Scandium",0.2,"facile",5}
	table_minerai_nebula[5] = {"Yttrium",0.2,"moyen",5}
	table_minerai_nebula[6] = {"Cerium",0.2,"difficile",5}
	table_minerai_nebula[7] = {"Praseodyme",0.25,"difficile",5}
	table_minerai_nebula[8] = {"Vide",0,"facile",0}
	
	liste_minerais = {"Fer","Nickel","Silice","Scandium","Yttrium","Cerium","Praseodyme"}
	player.minerai = {["Fer"]=0,["Nickel"]=0,["Silice"]=0,["Scandium"]=0,["Yttrium"]=0,["Cerium"]=0,["Praseodyme"]=0}
		
	player.max_stock = 50
	
	table_scan = {}
	table_scan[1] = {"facile","Tres faible probabilite de minerai precieux",0,0,1,1,"Faible probabilite de minerai precieux",0,1,1,2}
	table_scan[2] = {"moyen","Faible probabilite de minerai precieux",1,3,1,2,"Moyenne probabilite de minerai precieux",1,3,2,3}
	table_scan[3] = {"difficile","Moyenne probabilite de minerai precieux",1,3,2,3,"Forte probabilite de minerai precieux",2,4,1,3}
	
	-- Les astéroids avec du minerais
	
	for _, obj in ipairs(getAllObjects()) do
		if obj.typeName == "Asteroid" then
			alea_creation_minerai(obj)
		end
	end
	
	player:addCustomButton("weapons", "ASTEROID_DESTROY", "Detruire asteroides", function()
		x0,y0 = player:getPosition()
		if player:getWeaponStorage("HVLI") >= 1 then
			player:setWeaponStorage("HVLI",player:getWeaponStorage("HVLI")-1)
			for _, obj in ipairs(getObjectsInRadius(x0,y0,2500)) do
				if obj.typeName == "Asteroid" then
					CpuShip():setHull(1):setShields(0,0,0,0):setPosition(obj:getPosition()):orderIdle():setScanned(true):setRadarTrace("RadarBlip.png")
				end
			end
		else
			player:addToShipLog("HVLI necessaire pour detruire les asteroides","red","intern")
		end
	end)
	
	-- Fonctions MJ
	addGMFunction("Ajouter minerai", function()
        for _, obj in ipairs(getGMSelection()) do
            if obj.typeName == "Asteroid" then alea_creation_minerai(obj) end
        end
    end)
	
	addGMFunction("Ajouter minerai sp", function()
		-- removeCustom("Ajouter minerai sp")
		for _, ligne_minerai_temp in ipairs(table_minerai_nebula) do
			if ligne_minerai_temp[1] ~= "Vide" then
				addGMFunction(ligne_minerai_temp[1], function()
					for _, obj in ipairs(getGMSelection()) do
						if obj.typeName == "Asteroid" then alea_creation_minerai(obj,ligne_minerai_temp[1]) end
					end
				end)
				addGMFunction(ligne_minerai_temp[1].."+1", function()
					player.minerai[ligne_minerai_temp[1]] = player.minerai[ligne_minerai_temp[1]] + 1
					player.update_stock = 1
				end)
			end
		end
    end)
	
	addGMFunction("Vider minerai", function()
        for _, obj in ipairs(getGMSelection()) do
            if obj.typeName == "Asteroid" then vider_minerai(obj) end
        end
    end)
		
end


function update(delta)


	-- Fonctions supplémentaires pour les joueurs 
	-- si du minerai scanner autour
	if bouton_extraire == nil then bouton_extraire = 0 end
	if bouton_extraire == 0 then
		x0,y0 = player:getPosition()
		dummy_asteroid = 0		
		for _, obj in ipairs(getObjectsInRadius(x0,y0,1000)) do
			if obj.typeName == "Asteroid" and obj:isScannedBy(player) then
				dummy_asteroid = 1
			end
		end
		
		if dummy_asteroid == 1 then
			player:addCustomButton("engineering", "FARM_ASTEROID", "Extraire Minerai", function()
				x0,y0 = player:getPosition()
				for _, obj in ipairs(getObjectsInRadius(x0,y0,1000)) do
					if obj.typeName == "Asteroid" and obj:isScannedBy(player) and compte_stocks(player) < player.max_stock then
						player:commandImpulse(0)
						if obj.minerai == "Vide" then
							player:addToShipLog("Rien d'interessant à extraire dans un rayon de 1U","white","intern")
						else
							local diff_max = player.max_stock - compte_stocks(player)
							player.minerai[obj.minerai] = player.minerai[obj.minerai] + math.min(obj.concentration,diff_max)
							
							if diff_max < obj.concentration then player:addToShipLog("Stock maximum de minerais atteint","red","intern") end
							
							vider_minerai(obj)
							player.update_stock = 1
						end
					end
				end
			end)
			bouton_extraire = 1
		end
	end
	
	if bouton_decharger == nil then bouton_decharger = 0 end
	if bouton_decharger == 0 then
		if compte_stocks(player) > 0 then
			bouton_decharger = 1
			player:addCustomButton("engineering", "REMOVE_ASTEROID", "Decharger Minerai", function()
				if compte_stocks(player) > 0 then
					for _, res in ipairs(liste_minerais) do
						player.minerai[res] = 0
					end
					
					local x,y = player:getPosition()
					local dummy_station = 0
					for _, obj in ipairs(getObjectsInRadius(x,y,1500)) do
						if obj.typeName == "SpaceStation" then dummy_station = 1 end
					end
					if dummy_station == 1 then
						player:addToShipLog("Ressources minieres dechargees","white","intern")
					else
						player:addToShipLog("Ressources minieres evacuees dans l'espace","red","intern")
					end
					player.update_stock = 1
				end
			end)
		end
	end
	if bouton_decharger == 1 then
		if compte_stocks(player) == 0 then
			bouton_decharger = 0
			player:removeCustom("FARM_ASTEROID")
			player:removeCustom("REMOVE_ASTEROID")
		end
	end

	-- MAJ des stocks du joueur
	MAJ_minerai(delta)
	
	-- Calcul des probas pour le relais
	if sonde_minerai == nil then sonde_minerai = 0 end
	if sonde_asteroid == nil then sonde_asteroid = 0 end
	if pc_minerai == nil then pc_minerai = 0 end
	
	sonde_minerai_new = 0
	sonde_asteroid_new = 0
	pc_minerai_new = 0
	
	-- MAJ des descriptions des astéroids et des sondes du relais
	for _, obj in ipairs(getAllObjects()) do
		if obj.typeName == "Asteroid" then
			if obj:isScannedBy(player) then 
				if obj.description2 ~= nil then
					obj:setDescription(obj.description2)
				end
			end
			
			-- Calcul des probas pour le relay
			x,y = obj:getPosition()
			for _, obj2 in ipairs(getObjectsInRadius(x,y,5000)) do
				if obj2.typeName == "ScanProbe" or obj2 == player then
					sonde_asteroid_new = sonde_asteroid_new + 1
					if obj.bool_minerai == 1 then sonde_minerai_new = sonde_minerai_new + 1 end
				end
			end
		end
	end
	
	if sonde_asteroid_new > 0 then pc_minerai_new = sonde_minerai_new / sonde_asteroid_new end
		
	if pc_minerai_new ~= pc_minerai then
	
		player:removeCustom("INFO_minerai_relay")
		player:addCustomInfo("relay","INFO_TEMP_RELAY","MAJ des sondes...")
		
		if timer_probe == nil then timer_probe = 0 end
		timer_probe = timer_probe + delta
		
		if timer_probe > 3 then
			player:removeCustom("INFO_TEMP_RELAY")
				
			sonde_minerai = sonde_minerai_new
			sonde_asteroid = sonde_asteroid_new
			pc_minerai = pc_minerai_new
			player:addCustomInfo("relay","INFO_minerai_relay","% de minerai : ".. math.floor(pc_minerai*100) .. " %")
			timer_probe = nil
		end
	end
	
	-- Cr?ion des EMPP
	
	create_hull(delta)
	create_energy(delta)
	create_HVLI(delta)
	create_Homing(delta)
	create_Nuke(delta)
	create_EMP(delta)
	create_Mine(delta)
	create_empp(delta)


end

-- Gestion des minerais
function vider_minerai(obj)

	obj.minerai = "Vide"
	obj.bool_minerai = 0
	
	obj.description1 = nil
	obj.description2 = nil
	obj.concentration = nil
	
	obj:setDescription("Rien d'interessant")
	obj:setScanningParameters(0,0)
	
end

function creation_minerai(obj,minerai,description1,description2,para_scan1,para_scan2,concentration)

	obj.minerai = minerai
	if minerai == "Vide" then obj.bool_minerai = 0 
	else obj.bool_minerai = 1 end
	
	obj.description1 = description1
	obj.description2 = description2
	obj.concentration = concentration
	
	obj:setDescription(description1)
	obj:setScanningParameters(para_scan1,para_scan2)
	
end

function alea_creation_minerai(obj,choix_minerai)
	if choix_minerai == nil then choix_minerai = "" end
	if obj.typeName == "Asteroid" then
		
		-- Pour savoir si l'astéroid est dans une nébuleuse
		obj.bool_nebula = 0
		local x0,y0 = obj:getPosition()
		for _, obj_temp in ipairs(getObjectsInRadius(x0,y0,5000)) do
			if obj_temp.typeName == "Nebula" then obj.bool_nebula = 1 end
		end
		
		if choix_minerai == "" then
			-- Astéroid avec ou sans nébuleuse
			local table_minerai_temp
			if obj.bool_nebula == 0 then table_minerai_temp = table_minerai end
			if obj.bool_nebula == 1 then table_minerai_temp = table_minerai_nebula end
			
			local r = random(0,100)/100
			local r_temp = 0
			for _, ligne_minerai in ipairs(table_minerai_temp) do
				r_temp = r_temp + ligne_minerai[2]
				-- obj:setDescription("TEST BUG")
				if r <= r_temp then
					-- Pour être sûr de ne plus avoir de résultat valide
					r = 1.1
									
					-- la bonne description
					local descri2 = ""
					local random_concentration = 0
					if ligne_minerai[1] == "Vide" then 
						local random_concentration = 0
						descri2 = "Aucune Ressource presente" 
					else
						-- Pour déterminer la concentration
						random_concentration = math.floor(random(1,ligne_minerai[4]))
						descri2 =  "Minerai : " .. ligne_minerai[1] .. " ; concentration : ".. random_concentration
					end

					-- Pour trouver la difficulté de scan
					for _, ligne_scan in ipairs(table_scan) do
						if ligne_scan[1] == ligne_minerai[3] then
							local random_scan = random(0,100)/100
							if random_scan <= 0.5 then
								-- création du minerai
								creation_minerai(obj,ligne_minerai[1],ligne_scan[2],descri2,random(ligne_scan[3],ligne_scan[4]),random(ligne_scan[5],ligne_scan[6]),random_concentration)
							else
								-- création du minerai
								creation_minerai(obj,ligne_minerai[1],ligne_scan[7],descri2,random(ligne_scan[8],ligne_scan[9]),random(ligne_scan[10],ligne_scan[11]),random_concentration)
							end
						end
					end
				end
			end
		else -- Détermination manuelle du minerai
			for _, ligne_minerai in ipairs(table_minerai_nebula) do
				if ligne_minerai[1] == choix_minerai then
					-- Pour déterminer la concentration
					local random_concentration = math.floor(random(1,ligne_minerai[4]))
					local  descri2 =  "Minerai : " .. ligne_minerai[1] .. " ; concentration : ".. random_concentration

					-- Pour trouver la difficulté de scan
					for _, ligne_scan in ipairs(table_scan) do
						if ligne_scan[1] == ligne_minerai[3] then
							local random_scan = random(0,100)/100
							if random_scan <= 0.5 then
								-- création du minerai
								creation_minerai(obj,ligne_minerai[1],ligne_scan[2],descri2,random(ligne_scan[3],ligne_scan[4]),random(ligne_scan[5],ligne_scan[6]),random_concentration)
							else
								-- création du minerai
								creation_minerai(obj,ligne_minerai[1],ligne_scan[7],descri2,random(ligne_scan[8],ligne_scan[9]),random(ligne_scan[10],ligne_scan[11]),random_concentration)
							end
						end
					end
				end
			end
		end
	end
end

	
function MAJ_minerai(delta)
	if player.update_stock == nil then player.update_stock = 0 end
	if player.timer_ressource == nil then player.timer_ressource = 0 end
	
	if player.update_stock == 1 then
		player:addCustomInfo("engineering","INFO_TEMP","MAJ des minerais...")
		
		for _, res in ipairs(liste_minerais) do
			player:removeCustom("INFO_"..res)
		end

		player.timer_ressource = player.timer_ressource + delta
		if player.timer_ressource > 3 then
			
			for _, res in ipairs(liste_minerais) do
				if player.minerai[res] > 0 then player:addCustomInfo("engineering", "INFO_"..res, res.." : "..player.minerai[res]) end
			end
		
			player:removeCustom("INFO_TEMP")
			player.timer_ressource = 0
			player.update_stock = 0
		end
	end		
end

function compte_stocks(obj)
	local compte = 0
	for _, res in ipairs(liste_minerais) do
		compte = compte + obj.minerai[res]
	end
	return compte
end


function create_empp(delta)

	if player.minerai["Fer"] >= 5 and player.minerai["Cerium"] >= 0 and player.minerai["Praseodyme"] >= 0 and player.creation_boutton_empp == nil then
		player.creation_boutton_empp = 1
		player:addCustomButton("engineering", "CREATE_EMPP", "Creer EMPP", function()
			player.create_empp = 1
		end)
	end
	if (player.minerai["Fer"] < 5 or player.minerai["Cerium"] == 0 or player.minerai["Praseodyme"] == 0) and player.creation_boutton_empp == 1 then
		player.creation_boutton_empp = nil
		player:removeCustom("CREATE_EMPP")
	end
	
	if player.create_empp == nil then player.create_empp = 0 end
	
	if player.create_empp == 1 then 
		if (player.minerai["Fer"] < 5 or player.minerai["Cerium"] == 0 or player.minerai["Praseodyme"] == 0) then
			player:addToShipLog("Cerium, Fer ou Praseodyme en quantite insuffisante pour creer la bombe a freqence","red","intern")
			player.create_empp = 0
		end
		if player.minerai["Fer"] >= 5 and player.minerai["Cerium"] >= 0 and player.minerai["Praseodyme"] >= 0 then
			player.minerai["Fer"] = player.minerai["Fer"] - 5
			player.minerai["Cerium"] = player.minerai["Cerium"] - 1
			player.minerai["Praseodyme"] = player.minerai["Praseodyme"] - 1
			player.update_stock = 1
			player:addCustomInfo("engineering","INFO_EMPP","Creation de la bombe")
			player.timer_empp = 0
			player.create_empp = 2
		end
	end
	if player.create_empp == 2 then
		player.timer_empp = player.timer_empp + delta
		if player.timer_empp > 20 then
			player:removeCustom("INFO_EMPP")
			
			if player.empp == nil then player.empp = 0 end
			
			player.empp = player.empp + 1
			player:addCustomInfo("weapons","COUNT_EMPP","Nb de bombes a ions : " .. player.empp)
			
			player:addCustomButton("weapons", "LAUNCH_EMPP", "Activer Bombe ions", function()
				x0,y0 = player:getPosition()
				if player.empp >= 1 then 
					player.empp = player.empp - 1
					if player.empp == 0 then player:removeCustom("LAUNCH_EMPP") end
					player:addCustomInfo("weapons","COUNT_EMPP","Nb de bombes a ions : " .. player.empp)
					for _, obj in ipairs(getObjectsInRadius(x0,y0,4000)) do
						if obj.typeName == "CpuShip" then
							effet_empp = math.floor(random(2,obj:getWeaponTubeCount()))
							player:addToShipLog("Vaisseau " .. obj:getCallSign() .. " : " .. effet_empp .. "canon(s) desactives)","yellow","intern")
							obj:setWeaponTubeCount(obj:getWeaponTubeCount()-effet_empp)
							for n=0,15 do
								if random(0,100) < 50 then
									obj:setBeamWeapon(n, obj:getBeamWeaponArc(n), obj:getBeamWeaponDirection(n) , obj:getBeamWeaponRange(n)*0.4, obj:getBeamWeaponCycleTime(n), obj:getBeamWeaponDamage(n))
								end
							end
							
							nb_shields = math.floor(obj:getShieldCount())
							if nb_shields > 0 then 
								nvx_shield = obj:getShieldMax(1)*0.5
								player:addToShipLog("Vaisseau " .. obj:getCallSign() .. " : " .. nb_shields .. "bouclier(s) affectés)","yellow","intern")
							end
							if nb_shields == 1 then obj:setShieldsMax(obj:getShield(1)*0.5) end
							if nb_shields == 2 then 
								obj:setShieldsMax(nvx_shield,nvx_shield)
							end
							if nb_shields == 3 then 
								obj:setShieldsMax(nvx_shield,nvx_shield,nvx_shield)
							end
							if nb_shields == 4 then 
								obj:setShieldsMax(nvx_shield,nvx_shield,nvx_shield,nvx_shield)
							end
							if nb_shields == 5 then 
								obj:setShieldsMax(nvx_shield,nvx_shield,nvx_shield,nvx_shield,nvx_shield)
							end
							if nb_shields >= 6 then 
								obj:setShieldsMax(nvx_shield,nvx_shield,nvx_shield,nvx_shield,nvx_shield,nvx_shield)
							end
						end
					end
				end
			end)
			player.create_empp = 0
		end
	end
end


function create_hull(delta)

	if player.minerai["Fer"] >= 5 and player.minerai["Nickel"] >= 5 and player.creation_boutton_hull == nil then
		player.creation_boutton_hull = 1
		player:addCustomButton("engineering", "ADD_HULL", "Ajout Hull", function()
			player.create_hull = 1
		end)
	end
	if (player.minerai["Fer"] < 5 or player.minerai["Nickel"] < 5) and player.creation_boutton_hull == 1 then
		player:removeCustom("ADD_HULL")
		player.creation_boutton_hull = nil
	end
	
	if player.create_hull == nil then player.create_hull = 0 end
	
	if player.create_hull == 1 then 
		if player.minerai["Fer"] < 5 or player.minerai["Nickel"] < 5 then
			player:addToShipLog("Fer ou Nickel en quantite insuffisante pour ameliorer la carlingue","red","intern")
			player.create_hull = 0
		end
		if player.minerai["Fer"] >= 5 and player.minerai["Nickel"] >=5 then
			player.minerai["Fer"] = player.minerai["Fer"] - 5
			player.minerai["Nickel"] = player.minerai["Nickel"] - 5
			player.update_stock = 1
			player:addCustomInfo("engineering","INFO_HULL","Amelioration de la carlingue")
			player.timer_hull = 0
			player.create_hull = 2
		end
	end
	if player.create_hull == 2 then
		player.timer_hull = player.timer_hull + delta
		if player.timer_hull > 20 then
			player:removeCustom("INFO_HULL")		
			player:setHull(math.min(player:getHull()+50,player:getHullMax()))
			player.create_hull = 0
		end
	end
end

function create_energy(delta)

	if player.minerai["Silice"] >= 5 and player.creation_boutton_energy == nil then
		player.creation_boutton_energy = 1
		player:addCustomButton("engineering", "ADD_ENERGY", "Ajout energie", function()
			player.create_energy = 1
		end)
	end
	if player.minerai["Silice"] < 5 and player.creation_boutton_energy == 1 then
		player:removeCustom("ADD_ENERGY")
		player.creation_boutton_energy = nil
	end
	
	if player.create_energy == nil then player.create_energy = 0 end
	
	if player.create_energy == 1 then 
		if player.minerai["Silice"] < 5 then
			player:addToShipLog("Silice en quantite insuffisante pour accroitre l'energie","red","intern")
			player.create_energy = 0
		end
		if player.minerai["Silice"] >= 5 then
			player.minerai["Silice"] = player.minerai["Silice"] - 5
			player.update_stock = 1
			player:addCustomInfo("engineering","INFO_ENERGY","Ajout d'energie")
			player.timer_energy = 0
			player.create_energy = 2
		end
	end
	if player.create_energy == 2 then
		player.timer_energy = player.timer_energy + delta
		if player.timer_energy > 20 then
			player:removeCustom("INFO_ENERGY")
			player:setEnergyLevel(player:getEnergyLevel()+200)
			if player:getEnergyLevel() > player:getEnergyLevelMax() then
				player:setSystemHealth("reactor",player:getSystemHeat("reactor")-0.5)
			end
			player.create_energy = 0
		end
	end
end

function create_HVLI(delta)

	if player.minerai["Fer"] >= 5 and player.creation_boutton_HVLI == nil then
		player.creation_boutton_HVLI = 1
		player:addCustomButton("engineering", "ADD_HVLI", "Ajout HVLI", function()
			player.create_HVLI = 1
		end)
	end
	if player.minerai["Fer"] < 5 and player.creation_boutton_HVLI == 1 then
		player:removeCustom("ADD_HVLI")
		player.creation_boutton_HVLI = nil
	end
	
	if player.create_HVLI == nil then player.create_HVLI = 0 end
	
	if player.create_HVLI == 1 then 
		if player.minerai["Fer"] < 5 then
			player:addToShipLog("Fer en quantite insuffisante pour creer HVLI","red","intern")
			player.create_HVLI = 0
		end
		if player.minerai["Fer"] >= 5 then
			player.minerai["Fer"] = player.minerai["Fer"] - 5
			player.update_stock = 1
			player:addCustomInfo("engineering","INFO_HVLI","Ajout HVLI")
			player.timer_HVLI = 0
			player.create_HVLI = 2
		end
	end
	if player.create_HVLI == 2 then
		player.timer_HVLI = player.timer_HVLI + delta
		if player.timer_HVLI > 20 then
			player:removeCustom("INFO_HVLI")
			player:setWeaponStorage("HVLI", math.min(player:getWeaponStorage("HVLI")+3,player:getWeaponStorageMax("HVLI")))
			player.create_HVLI = 0
		end
	end
end

function create_Homing(delta)

	if player.minerai["Fer"] >= 5 and player.minerai["Scandium"] >= 1 and player.creation_boutton_Homing == nil then
		player.creation_boutton_Homing = 1
		player:addCustomButton("engineering", "ADD_Homing", "Ajout Homing", function()
			player.create_Homing = 1
		end)
	end
	if (player.minerai["Fer"] < 5 or player.minerai["Scandium"] < 1) and player.creation_boutton_Homing == 1 then
		player:removeCustom("ADD_Homing")
		player.creation_boutton_Homing = nil
	end
	
	if player.create_Homing == nil then player.create_Homing = 0 end
	
	if player.create_Homing == 1 then 
		if (player.minerai["Fer"] < 5 or player.minerai["Scandium"] < 1) then
			player:addToShipLog("Fer ou Scandium en quantite insuffisante pour creer Homing","red","intern")
			player.create_Homing = 0
		end
		if player.minerai["Fer"] >= 5 and player.minerai["Scandium"] >= 1 then
			player.minerai["Fer"] = player.minerai["Fer"] - 5
			player.minerai["Scandium"] = player.minerai["Scandium"] - 1
			player.update_stock = 1
			player:addCustomInfo("engineering","INFO_Homing","Ajout Homing")
			player.timer_Homing = 0
			player.create_Homing = 2
		end
	end
	if player.create_Homing == 2 then
		player.timer_Homing = player.timer_Homing + delta
		if player.timer_Homing > 20 then
			player:removeCustom("INFO_Homing")
			player:setWeaponStorage("Homing", math.min(player:getWeaponStorage("Homing")+3,player:getWeaponStorageMax("Homing")))
			player.create_Homing = 0
		end
	end
end

function create_Nuke(delta)

	if player.minerai["Fer"] >= 5 and player.minerai["Yttrium"] >= 1 and player.creation_boutton_Nuke == nil then
		player.creation_boutton_Nuke = 1
		player:addCustomButton("engineering", "ADD_Nuke", "Ajout Nuke", function()
			player.create_Nuke = 1
		end)
	end
	if (player.minerai["Fer"] < 5 or player.minerai["Yttrium"] < 1) and player.creation_boutton_Nuke == 1 then
		player:removeCustom("ADD_Nuke")
		player.creation_boutton_Nuke = nil
	end
	
	if player.create_Nuke == nil then player.create_Nuke = 0 end
	
	if player.create_Nuke == 1 then 
		if (player.minerai["Fer"] < 5 or player.minerai["Yttrium"] < 1) then
			player:addToShipLog("Fer ou Yttrium en quantite insuffisante pour creer Nuke","red","intern")
			player.create_Nuke = 0
		end
		if player.minerai["Fer"] >= 5 and player.minerai["Yttrium"] >= 1 then
			player.minerai["Fer"] = player.minerai["Fer"] - 5
			player.minerai["Yttrium"] = player.minerai["Yttrium"] - 1
			player.update_stock = 1
			player:addCustomInfo("engineering","INFO_Nuke","Ajout Nuke")
			player.timer_Nuke = 0
			player.create_Nuke = 2
		end
	end
	if player.create_Nuke == 2 then
		player.timer_Nuke = player.timer_Nuke + delta
		if player.timer_Nuke > 20 then
			player:removeCustom("INFO_Nuke")
			player:setWeaponStorage("Nuke", math.min(player:getWeaponStorage("Nuke")+3,player:getWeaponStorageMax("Nuke")))
			player.create_Nuke = 0
		end
	end
end

function create_EMP(delta)

	if player.minerai["Fer"] >= 5 and player.minerai["Cerium"] >= 1 and player.creation_boutton_EMP == nil then
		player.creation_boutton_EMP = 1
		player:addCustomButton("engineering", "ADD_EMP", "Ajout EMP", function()
			player.create_EMP = 1
		end)
	end
	if (player.minerai["Fer"] < 5 or player.minerai["Cerium"] < 1) and player.creation_boutton_EMP == 1 then
		player:removeCustom("ADD_EMP")
		player.creation_boutton_EMP = nil
	end
	
	if player.create_EMP == nil then player.create_EMP = 0 end
	
	if player.create_EMP == 1 then 
		if (player.minerai["Fer"] < 5 or player.minerai["Cerium"] < 1) then
			player:addToShipLog("Fer ou Cerium en quantite insuffisante pour creer EMP","red","intern")
			player.create_EMP = 0
		end
		if player.minerai["Fer"] >= 5 and player.minerai["Cerium"] >= 1 then
			player.minerai["Fer"] = player.minerai["Fer"] - 5
			player.minerai["Cerium"] = player.minerai["Cerium"] - 1
			player.update_stock = 1
			player:addCustomInfo("engineering","INFO_EMP","Ajout EMP")
			player.timer_EMP = 0
			player.create_EMP = 2
		end
	end
	if player.create_EMP == 2 then
		player.timer_EMP = player.timer_EMP + delta
		if player.timer_EMP > 20 then
			player:removeCustom("INFO_EMP")
			player:setWeaponStorage("EMP", math.min(player:getWeaponStorage("EMP")+3,player:getWeaponStorageMax("EMP")))
			player.create_EMP = 0
		end
	end
end

function create_Mine(delta)

	if player.minerai["Fer"] >= 5 and player.creation_boutton_Mine == nil then
		player.creation_boutton_Mine = 1
		player:addCustomButton("engineering", "ADD_Mine", "Ajout Mine", function()
			player.create_Mine = 1
		end)
	end
	if player.minerai["Fer"] < 5 and player.creation_boutton_Mine == 1 then
		player:removeCustom("ADD_Mine")
		player.creation_boutton_Mine = nil
	end
	
	if player.create_Mine == nil then player.create_Mine = 0 end
	
	if player.create_Mine == 1 then 
		if player.minerai["Fer"] < 5 then
			player:addToShipLog("Fer en quantite insuffisante pour creer Mine","red","intern")
			player.create_Mine = 0
		end
		if player.minerai["Fer"] >= 5 then
			player.minerai["Fer"] = player.minerai["Fer"] - 5
			player.update_stock = 1
			player:addCustomInfo("engineering","INFO_Mine","Ajout Mine")
			player.timer_Mine = 0
			player.create_Mine = 2
		end
	end
	if player.create_Mine == 2 then
		player.timer_Mine = player.timer_Mine + delta
		if player.timer_Mine > 20 then
			player:removeCustom("INFO_Mine")
			player:setWeaponStorage("Mine", math.min(player:getWeaponStorage("Mine")+3,player:getWeaponStorageMax("Mine")))
			player.create_Mine = 0
		end
	end
end
	

