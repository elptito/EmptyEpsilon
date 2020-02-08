#ifndef EDAMAGETYPE_H
#define EDAMAGETYPE_H

#include "engine.h"

enum EDamageType
{
    DT_None,
    DT_Energy,
    DT_Kinetic,
    DT_EMP
};

/* Define script conversion function for the EDamageType enum. */
template<> void convert<EDamageType>::param(lua_State* L, int& idx, EDamageType& es);
template<> int convert<EDamageType>::returnType(lua_State* L, EDamageType es);


#ifdef _MSC_VER
// MFC: GCC does proper external template instantiation, VC++ doesn't.
#include "EDamageType.hpp"
#endif

#endif
