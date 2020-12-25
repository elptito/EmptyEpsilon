--- This is Wyld ! Creates a wormhole which destructs jump and warp system
--- and disappear after set timer

function init()
    timer = 0
    missile_wormhole = WormHole():setPosition(posx, posy):setTargetPosition(posx,posy)
    missile_wormhole:onTeleportation(
        function(self, object)
            print(object.typeName)
            if string.find(string.lower(object.typeName), "ship") then -- yes, ugly but don't know how to get every ship type
                if(object:hasJumpDrive()) then
                    object:setSystemHealth("jumpdrive", -0.5)
                end
                if(object:hasWarpDrive()) then
                    object:setSystemHealth("warp", -0.5)
                end
            end
        end
    )

    timer_max = tonumber(runtime)
end

function update(delta)
    timer = timer + delta
    if timer > timer_max then
        missile_wormhole:destroy()
        destroyScript()
    end
    return
end