#ifndef _EDAMAGETYPE_HPP_
#define _EDAMAGETYPE_HPP_

#include "EDamageType.h"

/* Define script conversion function for the EDamageType enum. */
template<> void convert<EDamageType>::param(lua_State* L, int& idx, EDamageType& es)
{
    string str = string(luaL_checkstring(L, idx++)).lower();
    if (str == "kinetic")
        es = DT_Kinetic;
    else if (str == "emp")
        es = DT_EMP;
    else if (str == "energy")
        es = DT_Energy;
    else
        es = DT_None;
}

template<> int convert<EDamageType>::returnType(lua_State* L, EDamageType es)
{
    switch(es)
    {
    case DT_Kinetic:
        lua_pushstring(L, "kinetic");
        return 1;
    case DT_EMP:
        lua_pushstring(L, "emp");
        return 1;
    case DT_Energy:
        lua_pushstring(L, "energy");
        return 1;
    default:
        return 0;
    }
}

#endif /* _MISSILEWEAPONDATA_HPP_ */
