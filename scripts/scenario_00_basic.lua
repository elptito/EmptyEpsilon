-- Name: Basic
-- Description: Basic scenario. A few random stations, with random stuff around them, are under attack by enemies. Kill all enemies to win.
---
--- The scenario provides a single Atlantis (which is sufficient to win even on "Extreme").
--- Other player ships can be spawned, but the strength of the enemy is independent of their number and type.
-- Type: Basic
-- Variation[Empty]: Places no enemies. Recommended for GM-controlled scenarios and rookie crew orientation. The scenario continues until the GM declares victory or all Human Navy craft are destroyed.
-- Variation[Easy]: Places fewer enemies. Recommended for inexperienced crews.
-- Variation[Hard]: Places more enemies. Recommended if you have multiple player-controlled ships.
-- Variation[Extreme]: Places many enemies. You're pretty surely overwhelmed.

--- Scenario
-- @script scenario_00_basic

require("utils.lua")
-- For this scenario, utils.lua provides:
--   vectorFromAngle(angle, length)
--      Returns a relative vector (x, y coordinates)
--   setCirclePos(obj, x, y, angle, distance)
--      Returns the object with its position set to the resulting coordinates.

require("ee.lua") --tsht for systems
require("luax.lua") --tsht for table functions

-- Global variables for this scenario
local enemyList
local friendlyList
local stationList

--- Add an enemy wave.
--
-- That is, create it and add it to `list`.
--
-- @tparam table list A table containing enemy ship objects.
-- @tparam integer kind A number; at each integer, determines a different wave of ships to add
--  to the list. Any number is valid, but only 0.99-9.0 are meaningful.
-- @tparam number a The spawned wave's heading relative to the players' spawn point.
-- @tparam number d The spawned wave's distance from the players' spawn point.
function addWave(list, kind, a, d)
    if kind < 1.0 then
        table.insert(list, setCirclePos(CpuShip():setTemplate("Stalker Q7"):setRotation(a + 180):orderRoaming(), 0, 0, a, d))
    elseif kind < 2.0 then
        local leader = setCirclePos(CpuShip():setTemplate("Phobos T3"):setRotation(a + 180):orderRoaming(), 0, 0, a + random(-1, 1), d + random(-100, 100))
        table.insert(list, leader)
        table.insert(list, setCirclePos(CpuShip():setTemplate("MT52 Hornet"):setRotation(a + 180):orderFlyFormation(leader, -400, 0), 0, 0, a + random(-1, 1), d + random(-100, 100)))
        table.insert(list, setCirclePos(CpuShip():setTemplate("MT52 Hornet"):setRotation(a + 180):orderFlyFormation(leader, 400, 0), 0, 0, a + random(-1, 1), d + random(-100, 100)))
        table.insert(list, setCirclePos(CpuShip():setTemplate("MT52 Hornet"):setRotation(a + 180):orderFlyFormation(leader, -400, 400), 0, 0, a + random(-1, 1), d + random(-100, 100)))
        table.insert(list, setCirclePos(CpuShip():setTemplate("MT52 Hornet"):setRotation(a + 180):orderFlyFormation(leader, 400, 400), 0, 0, a + random(-1, 1), d + random(-100, 100)))
    elseif kind < 3.0 then
        table.insert(list, setCirclePos(CpuShip():setTemplate("Adder MK5"):setRotation(a + 180):orderRoaming(), 0, 0, a + random(-5, 5), d + random(-100, 100)))
        table.insert(list, setCirclePos(CpuShip():setTemplate("Adder MK5"):setRotation(a + 180):orderRoaming(), 0, 0, a + random(-5, 5), d + random(-100, 100)))
    elseif kind < 4.0 then
        table.insert(list, setCirclePos(CpuShip():setTemplate("Phobos T3"):setRotation(a + 180):orderRoaming(), 0, 0, a + random(-5, 5), d + random(-100, 100)))
        table.insert(list, setCirclePos(CpuShip():setTemplate("Phobos T3"):setRotation(a + 180):orderRoaming(), 0, 0, a + random(-5, 5), d + random(-100, 100)))
        table.insert(list, setCirclePos(CpuShip():setTemplate("Phobos T3"):setRotation(a + 180):orderRoaming(), 0, 0, a + random(-5, 5), d + random(-100, 100)))
    elseif kind < 5.0 then
        table.insert(list, setCirclePos(CpuShip():setTemplate("Atlantis X23"):setRotation(a + 180):orderRoaming(), 0, 0, a + random(-5, 5), d + random(-100, 100)))
    elseif kind < 6.0 then
        local leader = setCirclePos(CpuShip():setTemplate("Piranha F12"):setRotation(a + 180):orderRoaming(), 0, 0, a + random(-5, 5), d + random(-100, 100))
        table.insert(list, leader)
        table.insert(list, setCirclePos(CpuShip():setTemplate("MT52 Hornet"):setRotation(a + 180):orderFlyFormation(leader, -1500, 400), 0, 0, a + random(-1, 1), d + random(-100, 100)))
        table.insert(list, setCirclePos(CpuShip():setTemplate("MT52 Hornet"):setRotation(a + 180):orderFlyFormation(leader, 1500, 400), 0, 0, a + random(-1, 1), d + random(-100, 100)))
    elseif kind < 7.0 then
        table.insert(list, setCirclePos(CpuShip():setTemplate("Phobos T3"):setRotation(a + 180):orderRoaming(), 0, 0, a + random(-5, 5), d + random(-100, 100)))
        table.insert(list, setCirclePos(CpuShip():setTemplate("Phobos T3"):setRotation(a + 180):orderRoaming(), 0, 0, a + random(-5, 5), d + random(-100, 100)))
    elseif kind < 8.0 then
        table.insert(list, setCirclePos(CpuShip():setTemplate("Nirvana R5"):setRotation(a + 180):orderRoaming(), 0, 0, a + random(-5, 5), d + random(-100, 100)))
    elseif kind < 9.0 then
        table.insert(list, setCirclePos(CpuShip():setTemplate("MU52 Hornet"):setRotation(a + 180):orderRoaming(), 0, 0, a + random(-5, 5), d + random(-100, 100)))
    else
        table.insert(list, setCirclePos(CpuShip():setTemplate("Stalker R7"):setRotation(a + 180):orderRoaming(), 0, 0, a + random(-5, 5), d + random(-100, 100)))
        table.insert(list, setCirclePos(CpuShip():setTemplate("Stalker R7"):setRotation(a + 180):orderRoaming(), 0, 0, a + random(-5, 5), d + random(-100, 100)))
    end
end

--- Returns a semi-random heading.
--
-- @tparam number cnt A counter, generally between 1 and the number of enemy groups.
-- @tparam number enemy_group_count A number of enemy groups, generally set by the scenario variation.
-- @treturn number a random angle (between 0-60 and 360+60)
function randomWaveAngle(cnt, enemy_group_count)
    return cnt * 360 / enemy_group_count + random(-60, 60)
end

--- Returns a semi-random distance.
--
-- `enemy_group_count` is multiplied by 3 u and increases the distance.
--
-- @tparam number enemy_group_count A number of enemy groups, generally set by the scenario variation.
-- @treturn number a distance
function randomWaveDistance(enemy_group_count)
    return random(35000, 40000 + enemy_group_count * 3000)
end

-- Get number_of_systems random systems. Warning : systems may or may not be activated on the ship
function luaRandomSystems(number_of_systems)
    --local pship = getPlayerShip(-1)
    rand_table = {}
    for _, system in ipairs(SYSTEMS) do
        table.insert(rand_table, system)
    end
    table.shuffle(rand_table)
    return_table = {}
    for count = 1,number_of_systems do
        table.insert(return_table, rand_table[count])
    end
    return return_table
end

--You should only have to modify what is between "Begin balance modification" and "end balance modification"
onNewShip(
    function(ship)
        ship:onTakingDamage(
            function(self,instigator, typeOfDamage, freq, systemHit, shieldsDamage, hullDamage, hitShield)
                string.format("")	--serious proton needs a global context
                print(string.format("%s is hit", ship:getCallSign()))
                if instigator ~= nil then --mandatory to check there is an instigator, else it can be asteroid etc.
                    if typeOfDamage == "emp" and hullDamage > 0 then --means damage is an EMP missile, and some damage passed through shields
                        -- BEGIN BALANCE MODIFICATION HERE
                        -- BEGIN BALANCE MODIFICATION HERE
                        -- BEGIN BALANCE MODIFICATION HERE
                        local num_of_sys_hit = 3 --edit here to change number of affected systems
                        -- END BALANCE MODIFICATION
                        -- END BALANCE MODIFICATION
                        -- END BALANCE MODIFICATION

                        list_of_systems = luaRandomSystems(num_of_sys_hit) --means we get the random systems (which may NOT be activated on ship)
                        --print(string.format("Degats: type %s freq %i sys %s shi %f dam %f hit %i",typeOfDamage,freq,systemHit,shieldsDamage,hullDamage,hitShield))
                        for count = 1, num_of_sys_hit do
                            print(string.format("%s hit for %f damage ", list_of_systems[count], hullDamage))

                            -- BEGIN BALANCE MODIFICATION HERE
                            -- BEGIN BALANCE MODIFICATION HERE
                            -- BEGIN BALANCE MODIFICATION HERE
                            
                            --inflicts energy loss
                            local current_energy = ship:getEnergy()
                            ship:setEnergy((current_energy - hullDamage) *10 ) --set the multiplier as it's total energy - damage
                            
                            --inflicts heat (between 0 and 1)
                            local current_heat = ship:getSystemHeat(list_of_systems[count])
                            ship:setSystemHeat(list_of_systems[count], (current_heat + hullDamage)/100) --set this as for now it's 1 damage = 1%
                            
                            --inflicts hack (between 0 and 1)
                            local current_hack = ship:getSystemHackedLevel(list_of_systems[count])
                            ship:setSystemHackedLevel(list_of_systems[count], (current_hack + hullDamage)/100) --set this as for now it's 1 damage = 1%

                            --inflicts damage on system (overrides system harness set by ratio or minimal damage needed to hit) (between 0 and 1)
                            local current_health = ship:getSystemHealth(list_of_systems[count])
                            ship:setSystemHealth(list_of_systems[count], (current_health + hullDamage)/100) --set this as for now it's 1 damage = 1%

                            -- END BALANCE MODIFICATION
                            -- END BALANCE MODIFICATION
                            -- END BALANCE MODIFICATION

                        end --for
                    elseif typeOfDamage ~= "emp" then
                            print "Not emp damage"
                    elseif hullDamage <= 0 then
                            print "No damage went through shields"
                    end --if damage and type
                end --if instigator
            end
        )
    end
)

function popWarpJammer(toto)
    nb_warpjam = tonumber(toto:getInfosValue(1))
    print "test"
    if(nb_warpjam and nb_warpjam > 0) then
        local posx,posy = toto:getPosition()
        warpJammer = WarpJammer():setFaction(toto:getFaction()):setRange(10000):setPosition(posx-500, posy)
        toto:addInfos(1,"Nb Warpjam", nb_warpjam - 1)
        toto:removeCustom(popWarpJammerButton)
        toto:addCustomButton("Relay",popWarpJammerButton,string.format("Deployer antiwarp (%i)", tonumber(toto:getInfosValue(1))),toto.popWarpJammer)
    end
    
end

onNewPlayerShip(
	function(pc)
		pc.popWarpJammer = function()
			popWarpJammer(pc)
		end
		pc:addInfos(1,"Nb Warpjam", "4")
		popWarpJammerButton = "popWarpjammerButton"
		pc:addCustomButton("Relay",popWarpJammerButton,string.format("Deployer antiwarp (%i)", tonumber(pc:getInfosValue(1))),pc.popWarpJammer)
	end

)


--- Initialize scenario.
function init()
    -- Spawn a player Atlantis.
    player = PlayerSpaceship():setFaction("Human Navy"):setTemplate("Atlantis")

    enemyList = {}
    friendlyList = {}
    stationList = {}
	list_info_value = {}

    -- Randomly distribute 3 allied stations throughout the region.
    local n
    n = 0
    local station_1 = SpaceStation():setTemplate("Small Station"):setRotation(random(0, 360)):setFaction("Human Navy")
    setCirclePos(station_1, 0, 0, n * 360 / 3 + random(-30, 30), random(10000, 22000))
    table.insert(stationList, station_1)
    table.insert(friendlyList, station_1)
    n = 1
    local station_2 = SpaceStation():setTemplate("Medium Station"):setRotation(random(0, 360)):setFaction("Human Navy")
    setCirclePos(station_2, 0, 0, n * 360 / 3 + random(-30, 30), random(10000, 22000))
    table.insert(stationList, station_2)
    table.insert(friendlyList, station_2)
    n = 2
    local station_3 = SpaceStation():setTemplate("Large Station"):setRotation(random(0, 360)):setFaction("Human Navy")
    setCirclePos(station_3, 0, 0, n * 360 / 3 + random(-30, 30), random(10000, 22000))
    table.insert(stationList, station_3)
    table.insert(friendlyList, station_3)

    -- Start the players with 300 reputation.
    friendlyList[1]:addReputationPoints(300.0)

    -- Randomly scatter nebulae near the players' spawn point.
    local cx, cy = friendlyList[1]:getPosition()
    setCirclePos(Nebula(), cx, cy, random(0, 360), 6000)

    for _ = 1, 5 do
        setCirclePos(Nebula(), 0, 0, random(0, 360), random(20000, 45000))
    end

    -- GM functions to manually trigger enemy waves.
    local buttons = {
        -- button name, kind of wave
        {"Strikeship wave", 0},
        {"Fighter wave", 1},
        {"Gunship wave", 2},
        {"Dreadnought", 4},
        {"Missile cruiser wave", 5},
        {"Cruiser wave", 6},
        {"Adv. striker wave", 9}
    }
    for _, button in ipairs(buttons) do
        local name, kind = table.unpack(button)
        addGMFunction(
            name,
            function()
                addWave(enemyList, kind, randomWaveAngle(math.random(20), math.random(20)), randomWaveDistance(math.random(5)))
            end
        )
    end

    -- Let the GM spawn a random enemy wave.
    addGMFunction(
        "Random wave",
        function()
            local a = randomWaveAngle(math.random(20), math.random(20))
            local d = randomWaveDistance(math.random(20))
            local kind = random(0, 10)
            addWave(enemyList, kind, a, d)
        end
    )

    -- Let the GM spawn random reinforcements. Their distance from the
    -- players' spawn point is about half that of enemy waves.
    addGMFunction(
        "Random friendly",
        function()
            local a = randomWaveAngle(math.random(20), math.random(20))
            local d = random(15000, 20000 + math.random(20) * 1500)
            local friendlyShip = {"Phobos T3", "MU52 Hornet", "Piranha F12"}
            local friendlyShipIndex = math.random(#friendlyShip)
            table.insert(friendlyList, setCirclePos(CpuShip():setTemplate(friendlyShip[friendlyShipIndex]):setRotation(a):setFaction("Human Navy"):orderRoaming():setScanned(true), 0, 0, a + random(-5, 5), d + random(-100, 100)))
        end
    )

    -- Let the GM declare the Humans (players) victorious.
    addGMFunction(
        "Win",
        function()
            victory("Human Navy")
        end
    )

    -- Set the number of enemy waves based on the scenario variation.
    local counts = {
        ["Extreme"] = 20,
        ["Hard"] = 8,
        -- default:
        ["None"] = 5,
        ["Easy"] = 3,
        ["Empty"] = 0
    }
    local enemy_group_count = counts[getScenarioVariation()]
    assert(enemy_group_count, "unknown variation, could not set enemy_group_count")

    -- If not in the Empty variation, spawn the corresponding number of random
    -- enemy waves at distributed random headings and semi-random distances
    -- relative to the players' spawn point.
    for cnt = 1, enemy_group_count do
        local a = randomWaveAngle(cnt, enemy_group_count)
        local d = randomWaveDistance(enemy_group_count)
        local kind = random(0, 10)
        addWave(enemyList, kind, a, d)
    end

    -- Spawn 2-5 random asteroid belts.
    for _ = 1, irandom(2, 5) do
        local a = random(0, 360)
        local a2 = random(0, 360)
        local d = random(3000, 40000)
        local x, y = vectorFromAngle(a, d)

        for _ = 1, 50 do
            local dx1, dy1 = vectorFromAngle(a2, random(-1000, 1000))
            local dx2, dy2 = vectorFromAngle(a2 + 90, random(-20000, 20000))
            local posx = x + dx1 + dx2
            local posy = x + dy1 + dy2
            -- Avoid spawning asteroids within 1U of the player start position or
            -- 2U of any station.
            if math.abs(posx) > 1000 and math.abs(posy) > 1000 then
                for _, station in ipairs(stationList) do
                    if distance(station, posx, posy) > 2000 then
                        Asteroid():setPosition(posx, posy):setSize(random(100, 500))
                    end
                end
            end
        end

        for _ = 1, 100 do
            local dx1, dy1 = vectorFromAngle(a2, random(-1500, 1500))
            local dx2, dy2 = vectorFromAngle(a2 + 90, random(-20000, 20000))
            VisualAsteroid():setPosition(x + dx1 + dx2, y + dy1 + dy2)
        end
    end

    -- Spawn 0-3 random mine fields.
    for _ = 1, irandom(0, 3) do
        local a = random(0, 360)
        local a2 = random(0, 360)
        local d = random(20000, 40000)
        local x, y = vectorFromAngle(a, d)

        for nx = -1, 1 do
            for ny = -5, 5 do
                if random(0, 100) < 90 then
                    local dx1, dy1 = vectorFromAngle(a2, (nx * 1000) + random(-100, 100))
                    local dx2, dy2 = vectorFromAngle(a2 + 90, (ny * 1000) + random(-100, 100))
                    Mine():setPosition(x + dx1 + dx2, y + dy1 + dy2)
                end
            end
        end
    end

    -- Spawn a random black hole.
    local a = random(0, 360)
    local d = random(10000, 45000)
    local x, y = vectorFromAngle(a, d)
    -- Watching a station fall into a black hole to start the game never gets old,
    -- but players hate it. Avoid spawning black holes too close to stations.
    local spawn_hole = false
    while not spawn_hole do
        for _, station in ipairs(stationList) do
            if distance(station, x, y) > 3000 then
                spawn_hole = true
            else
                spawn_hole = false
            end
        end
    end
    BlackHole():setPosition(x, y)

    -- Spawn random neutral transports.
    Script():run("util_random_transports.lua")
end

--- Update.
--
-- @tparam number delta the time delta (in seconds)
function update(delta)
    -- Count all surviving enemies and allies.
    local enemy_count = 0
    for _, enemy in ipairs(enemyList) do
        if enemy:isValid() then
            enemy_count = enemy_count + 1
        end
    end
    local friendly_count = 0
    for _, friendly in ipairs(friendlyList) do
        if friendly:isValid() then
            friendly_count = friendly_count + 1
        end
    end

    -- If not playing the Empty variation, declare victory for the
    -- Humans (players) once all enemies are destroyed. Note that players can win
    -- even if they destroy the enemies by blowing themselves up.
    --
    -- In the Empty variation, the GM must use the Win button to declare
    -- a Human victory.
    if (enemy_count == 0 and getScenarioVariation() ~= "Empty") then
        victory("Human Navy")
    end

    -- If all allies are destroyed, the Humans (players) lose.
    if friendly_count == 0 then
        victory("Kraylor")
    else
        -- As the battle continues, award reputation based on
        -- the players' progress and number of surviving allies.
        for _, friendly in ipairs(friendlyList) do
            if friendly:isValid() then
                friendly:addReputationPoints(delta * friendly_count * 0.1)
            end
        end
    end
	
	for pidx=1,12 do
	p = getPlayerShip(pidx)
		if p ~= nil and p:isValid() then
			if p:getInfosValue(1) ~= list_info_value[pidx] then
				popWarpJammerButton = "popWarpjammerButton"
				p:addCustomButton("Relay",popWarpJammerButton,string.format("Deployer antiwarp (%i)", tonumber(p:getInfosValue(1))),p.popWarpJammer)
				list_info_value[pidx] = p:getInfosValue(1)
			end
		end
	end
end
