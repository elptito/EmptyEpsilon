
local densityLayer = 0
local firstHighway = 5
-- initialize map layers
function initLayers()
    setMapLayer(densityLayer, "space dilation_00004_00000.png", 0, 0, 7000, "Density")
	-- setMapLayer(1, "space dilation_00004_00001.png", 0, 0, 7000, "clouds 2")
	-- setMapLayer(2, "space dilation_00004_00002.png", 0, 0, 7000, "clouds 3")
	-- setMapLayer(3, "space dilation_00004_00003.png", 0, 0, 7000, "clouds 4")
	-- setMapLayer(4, "space dilation_00004_00004.png", 0, 0, 7000, "clouds 5")
	setMapLayer(firstHighway, "space dilation_00004_00005.png", 0, 0, 7000, "Highways " .. getFrequencyDisplayName(0))
	setMapLayer(firstHighway + 1, "space dilation_00004_00006.png", 0, 0, 7000, "Highways " .. getFrequencyDisplayName(2))
	setMapLayer(firstHighway + 2, "space dilation_00004_00007.png", 0, 0, 7000, "Highways " .. getFrequencyDisplayName(2))
	setMapLayer(firstHighway + 3, "space dilation_00004_00008.png", 0, 0, 7000, "Highways " .. getFrequencyDisplayName(3))
	setMapLayer(firstHighway + 4, "space dilation_00004_00009.png", 0, 0, 7000, "Highways " .. getFrequencyDisplayName(4))
end


local warp_boost_terrain_min_factor = tonumber(getPreference("warp_boost_terrain_min_factor", "1.0"))
local warp_boost_terrain_range_factor = tonumber(getPreference("warp_boost_terrain_max_factor", "10.0")) - warp_boost_terrain_min_factor
local warp_terrain_cap = tonumber(getPreference("warp_terrain_cap", "2.0"))
local heat_warp_density = tonumber(getPreference("heat_warp_density", "0.02"))
local systems = {"warp", "rearshield", "docks", "impulse", "beamweapons", "drones", "frontshield", "maneuver", "reactor", "missilesystem"}

-- update state helios mechanics
function updateHelios(delta)
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