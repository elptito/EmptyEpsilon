#ifndef _MISSILEWEAPONDATA_HPP_
#define _MISSILEWEAPONDATA_HPP_

/* Define script conversion function for the EMissileWeapons enum. */
template<> void convert<EMissileWeapons>::param(lua_State* L, int& idx, EMissileWeapons& es)
{
    string str = string(luaL_checkstring(L, idx++)).lower();
    if (str == "cruise")
        es = MW_Cruise;
    else if (str == "torpedo")
        es = MW_Torpedo;
    else if (str == "heavy")
        es = MW_Heavy;
    else if (str == "emp")
        es = MW_EMP;
    else if (str == "nuke")
        es = MW_Nuke;
    else if (str == "hvli")
        es = MW_HVLI;
    else if (str == "mine")
        es = MW_Mine;
    else
        es = MW_None;
}

template<> int convert<EMissileWeapons>::returnType(lua_State* L, EMissileWeapons es)
{
    switch(es)
    {
    case MW_Cruise:
        lua_pushstring(L, "cruise");
        return 1;
    case MW_Torpedo:
        lua_pushstring(L, "torpedo");
        return 1;
    case MW_Heavy:
        lua_pushstring(L, "heavy");
        return 1;
    case MW_EMP:
        lua_pushstring(L, "emp");
        return 1;
    case MW_Nuke:
        lua_pushstring(L, "nuke");
        return 1;
    case MW_HVLI:
        lua_pushstring(L, "hvli");
        return 1;
    case MW_Mine:
        lua_pushstring(L, "mine");
        return 1;
    default:
        return 0;
    }
}

#endif /* _MISSILEWEAPONDATA_HPP_ */
