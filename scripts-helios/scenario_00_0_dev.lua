-- Name: sandbox
-- Description: a place to throw simulations for development tests
-- Type: Basic

function init()
        setFactionVsFactionState("Free Miners", "FSN", "Friendly")
        setFactionVsFactionState("FSN", "Free Miners", "Friendly")
        setFactionVsFactionState("FSN", "MCF", "Enemy")
        setFactionVsFactionState("FSN", "MCF Extreme", "Enemy")
        setFactionVsFactionState("FSN", "Pirates", "Enemy")
        setFactionVsFactionState("FSN", "Corp1", "Friendly")
        setFactionVsFactionState("FSN", "Corp2", "Friendly")
        setFactionVsFactionState("MCF Extreme", "FSN", "Enemy")
        setFactionVsFactionState("Raiders", "FSN", "Enemy")
        setFactionVsFactionState("Corp1", "FSN", "Friendly")
        PlayerSpaceship():setTemplate("Gravitas"):setFaction("FSN"):setPosition(0, 0):setWarpFrequency(5)
        -- enemyShips()
        -- tactical()
        targets()
end

function update(delta)
	--No victory condition
end

function targets()
        SpaceStation():setTemplate("Small Station"):setFaction("Free Miners"):setCallSign("DS665"):setPosition(-16949, 18366)
        SpaceStation():setTemplate("Small Station"):setFaction("Free Miners"):setCallSign("DS666"):setPosition(-17659, 17780)
        SpaceStation():setTemplate("Small Station"):setFaction("Free Miners"):setCallSign("DS667"):setPosition(-12743, 18683)
        SpaceStation():setTemplate("Medium Station"):setFaction("MCF Extreme"):setCallSign("target_UN_HIDDEN"):setPosition(-14967, 17975)
        SpaceStation():setTemplate("Medium Station"):setFaction("MCF Extreme"):setCallSign("target_HIDDEN_1"):setPosition(12410, 18687)
        SpaceStation():setTemplate("Medium Station"):setFaction("MCF Extreme"):setCallSign("target_3"):setPosition(0, 15000)
        SpaceStation():setTemplate("Medium Station"):setFaction("MCF Extreme"):setCallSign("target_2"):setPosition(0, 7500)
        SpaceStation():setTemplate("Medium Station"):setFaction("MCF Extreme"):setCallSign("Target_1"):setPosition(481, 3000)
        SpaceStation():setTemplate("Medium Station"):setFaction("MCF Extreme"):setCallSign("target_HIDDEN_2"):setPosition(-13261, 8980)
        Nebula():setPosition(-12869, 11876)
end

function enemyShips()
        CpuShip():setFaction("MCF"):setTemplate("Phobos T3"):setCallSign("UTI10"):setPosition(9747, 708):orderRoaming():setWeaponStorage("Homing", 2):setWarpFrequency(5)
        CpuShip():setFaction("MCF"):setTemplate("Phobos T3"):setCallSign("SS11"):setPosition(-1042, -915):orderRoaming():setWeaponStorage("Homing", 2):setWarpFrequency(5)
        CpuShip():setFaction("MCF"):setTemplate("Phobos T3"):setCallSign("VK12"):setPosition(-1190, 813):orderRoaming():setWeaponStorage("Homing", 2):setWarpFrequency(2)
        CpuShip():setFaction("MCF"):setTemplate("Adder MK5"):setCallSign("UTI13"):setPosition(1400, 35668):orderRoaming():setWeaponStorage("HVLI", 2):setWarpFrequency(2)
        CpuShip():setFaction("MCF"):setTemplate("Adder MK5"):setCallSign("CSS14"):setPosition(144, 35603):orderRoaming():setWeaponStorage("HVLI", 2):setWarpFrequency(5)
        CpuShip():setFaction("MCF"):setTemplate("Phobos T3"):setCallSign("CSS15"):setPosition(455, -8130):orderRoaming():setWeaponStorage("Homing", 2):setWarpFrequency(2)
        CpuShip():setFaction("MCF"):setTemplate("MT52 Hornet"):setCallSign("S16"):setPosition(372, -7962):setWarpFrequency(4)
        CpuShip():setFaction("MCF"):setTemplate("MT52 Hornet"):setCallSign("S17"):setPosition(387, -8146):setWarpFrequency(5)
        CpuShip():setFaction("MCF"):setTemplate("MT52 Hornet"):setCallSign("VS18"):setPosition(264, -7988):setWarpFrequency(2)
        CpuShip():setFaction("MCF"):setTemplate("MT52 Hornet"):setCallSign("CV19"):setPosition(627, -8285):setWarpFrequency(4)
        CpuShip():setFaction("MCF"):setTemplate("Phobos T3"):setCallSign("CCN20"):setPosition(-3272, -47106):orderRoaming():setWeaponStorage("Homing", 2):setWarpFrequency(4)
        CpuShip():setFaction("MCF"):setTemplate("Phobos T3"):setCallSign("BR21"):setPosition(-2968, -47220):orderRoaming():setWeaponStorage("Homing", 2):setWarpFrequency(5)
        CpuShip():setFaction("MCF"):setTemplate("Phobos T3"):setCallSign("UTI22"):setPosition(-3362, -47063):orderRoaming():setWeaponStorage("Homing", 2):setWarpFrequency(4)
        CpuShip():setFaction("MCF"):setTemplate("Atlantis X23"):setCallSign("CCN23"):setPosition(34368, 41116):orderRoaming():setWeaponStorage("Homing", 0):setWeaponStorage("HVLI", 16):setWarpFrequency(3)
end

function tactical()
        SpaceStation():setTemplate("Small Station"):setFaction("FSN"):setCallSign("DS13"):setPosition(14110, 411)
        SpaceStation():setTemplate("Medium Station"):setFaction("FSN"):setCallSign("DS14"):setPosition(-4115, 949)
        SpaceStation():setTemplate("Large Station"):setFaction("FSN"):setCallSign("DS15"):setPosition(-4517, -3638)
        Nebula():setPosition(8215, -707)
        Nebula():setPosition(2098, 28901)
        Nebula():setPosition(-17949, 25065)
        Nebula():setPosition(3306, -4659)
        Nebula():setPosition(-2719, -7201)
        Nebula():setPosition(-37226, 6164)
        Asteroid():setPosition(-18095, -42796)
        Asteroid():setPosition(-1907, -19278)
        Asteroid():setPosition(-22208, -40087)
        Asteroid():setPosition(-8834, -14878)
        Asteroid():setPosition(-8395, -14614)
        Asteroid():setPosition(-8933, -14924)
        Asteroid():setPosition(-2661, -19477)
        Asteroid():setPosition(-7351, -16602)
        Asteroid():setPosition(-49209, -23251)
        Asteroid():setPosition(-13462, -10923)
        Asteroid():setPosition(-8610, -15486)
        Asteroid():setPosition(-17158, -44389)
        Asteroid():setPosition(-13381, -12533)
        Asteroid():setPosition(-12124, -12801)
        Asteroid():setPosition(-11737, -12022)
        Asteroid():setPosition(-20192, -41464)
        Asteroid():setPosition(-43063, -25701)
        VisualAsteroid():setPosition(-41026, -13031)
        VisualAsteroid():setPosition(-39657, -15362)
        VisualAsteroid():setPosition(-39751, -16964)
        VisualAsteroid():setPosition(-46097, -12584)
        VisualAsteroid():setPosition(-32967, -20560)
        VisualAsteroid():setPosition(-45953, -12785)
        VisualAsteroid():setPosition(-42322, -13295)
        VisualAsteroid():setPosition(-44481, -13759)
        VisualAsteroid():setPosition(-43029, -14881)
        Asteroid():setPosition(-29054, -18073)
        Asteroid():setPosition(-38662, -42789)
        Asteroid():setPosition(-6458, -15211)
        Asteroid():setPosition(-6683, -15537)
        Asteroid():setPosition(-4119, -5404)
        Asteroid():setPosition(-38192, -46789)
        Asteroid():setPosition(-29052, -17974)
        Asteroid():setPosition(-5135, -8395)
        Asteroid():setPosition(-39211, -45339)
        Asteroid():setPosition(-36303, -39087)
        Asteroid():setPosition(-39942, -52107)
        Asteroid():setPosition(-41827, -54025)
        Asteroid():setPosition(-37128, -41397)
        Asteroid():setPosition(-9760, -19773)
        Asteroid():setPosition(-6353, -10712)
        Asteroid():setPosition(-36914, -41593)
        Asteroid():setPosition(-29436, -18768)
        Asteroid():setPosition(-30462, -18713)
        Asteroid():setPosition(-5210, -10499)
        Asteroid():setPosition(-8489, -20723)
        Asteroid():setPosition(-38273, -43381)
        Asteroid():setPosition(-39120, -49014)
        Asteroid():setPosition(-9445, -20644)
        Asteroid():setPosition(-29596, -16398)
        Asteroid():setPosition(-30962, -20458)
        Asteroid():setPosition(-3325, -5321)
        Asteroid():setPosition(-29356, -18374)
        Asteroid():setPosition(-28465, -17168)
        Asteroid():setPosition(-7988, -16304)
        VisualAsteroid():setPosition(-40799, -20614)
        VisualAsteroid():setPosition(-31478, 8471)
        VisualAsteroid():setPosition(-39123, -11267)
        VisualAsteroid():setPosition(-32669, 1268)
        VisualAsteroid():setPosition(-31502, 12556)
        VisualAsteroid():setPosition(-34889, -1479)
        VisualAsteroid():setPosition(-36462, -3373)
        VisualAsteroid():setPosition(-37184, -7594)
        VisualAsteroid():setPosition(-34248, -2810)
        VisualAsteroid():setPosition(-37782, -10035)
        Asteroid():setPosition(6167, 6019)
        Asteroid():setPosition(-4208, -4611)
        Asteroid():setPosition(7705, 6922)
        Asteroid():setPosition(4980, -9994)
        Asteroid():setPosition(2255, 2197)
        Asteroid():setPosition(8532, -5729)
        Asteroid():setPosition(5168, 3829)
        Asteroid():setPosition(5604, -9962)
        Asteroid():setPosition(18813, 21548)
        Asteroid():setPosition(3448, 1910)
        Asteroid():setPosition(7540, -7296)
        Asteroid():setPosition(17896, 19520)
        Asteroid():setPosition(5932, 6302)
        Asteroid():setPosition(5512, -9870)
        Asteroid():setPosition(6000, 6273)
        Asteroid():setPosition(18989, 21309)
        Asteroid():setPosition(3842, -12563)
        Asteroid():setPosition(7504, 8264)
        Asteroid():setPosition(3759, 3840)
        Asteroid():setPosition(8862, 7594)
        Asteroid():setPosition(-4768, -7812)
        Mine():setPosition(-23116, 29143)
        Mine():setPosition(-22139, 28761)
        Mine():setPosition(-21340, 28358)
        Mine():setPosition(-20338, 27865)
        Mine():setPosition(-19592, 27449)
        Mine():setPosition(-18730, 26836)
        Mine():setPosition(-17848, 26432)
        Mine():setPosition(-16872, 26013)
end