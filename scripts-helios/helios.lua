
local densityLayer = 0
local firstHighway = densityLayer + 1
-- initialize map layers
function initLayers()
	setMapLayer(densityLayer, "space dilation_00004_00000.png", 0, 0, 7000, "Density")
	for i=firstHighway, (10 - firstHighway) do 
		setMapLayer(i, "space dilation_00004_000" .. string.format("%02d", i) .. ".png", 0, 0, 7000, "Highways " .. getFrequencyDisplayName(i))
	end
end

local warp_boost_terrain_min_factor = tonumber(getPreference("warp_boost_terrain_min_factor", "1.0"))
local warp_boost_terrain_range_factor = tonumber(getPreference("warp_boost_terrain_max_factor", "10.0")) - warp_boost_terrain_min_factor
local warp_terrain_cap = tonumber(getPreference("warp_terrain_cap", "2.0"))
local heat_warp_density = tonumber(getPreference("heat_warp_density", "0.02"))
local systems = {"warp", "rearshield", "docks", "impulse", "beamweapons", "drones", "frontshield", "maneuver", "reactor", "missilesystem"}

-- update state helios mechanics
function updateHelios(delta)
	local playerShip = getPlayerShip(-1)
	if (not playerShip._daedalus_hooks) then
		playerShip._daedalus_hooks = {}
	end
	for i, hook in ipairs(playerShip._daedalus_hooks) do
		hook(delta)
	end

	local ships = getAllShips()
	for _, ship in ipairs(ships) do
		local x, y = ship:getPosition()

		-- -- set max warp for ship
		-- local warpCapLayer = -1
		-- local maxWarpFactor = getTerrainValueAtPosition(warpCapLayer, x, y)
		-- local maxWarp = warp_terrain_cap + maxWarpFactor * (4 - warp_terrain_cap)
		-- ship:setMaxWarp(maxWarp)

        -- set warp highway boost for ship
		local highwayLayer = firstHighway + ship:getWarpFrequency()
		local warpBoostFactor = warp_boost_terrain_min_factor + warp_boost_terrain_range_factor * getTerrainValueAtPosition(highwayLayer, x, y)
		ship:setWarpBoostFactor(warpBoostFactor)

        -- set warp density effect on ship
		local currWarp = ship:getCurrentWarp()
		if (currWarp > 0) then
			local densityFactor = getTerrainValueAtPosition(densityLayer, x, y)
			if (densityFactor > 0) then
				-- local system = systems[irandom(1,#systems)]
				local systemIdx = 1 + math.floor(math.abs(math.fmod((x + y + irandom(1,30000)) / 50000, #systems)))
				-- print("systemIdx", systemIdx)
				local system = systems[systemIdx]
				ship:setSystemHeat(system, ship:getSystemHeat(system) + currWarp * densityFactor * delta * heat_warp_density)
			end
		end
	end
end