#define VENGINE_IMPL

#include "internal.h"
#include "vengine.h"

static int vel_inew(lua_State* L) {
    lua_settop(L, 0);
    lua_newuserdata(L, sizeof(vengine_Instance));

    lua_newtable(L);
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");

//    lua_pushvalue(L, -1);
//    lua_setfield(L, -2, "__newindex");

    lua_pushstring(L, "Instance");
    lua_setfield(L, -2, "__type");

    lua_pushstring(L, "It's new Instance!");
    lua_setfield(L, -2, "Name");

    lua_setmetatable(L, -2);

    lua_getfield(L, LUA_REGISTRYINDEX, "_instances");
    int n = lua_objlen(L, -1);
    lua_pushvalue(L, -2);
    lua_rawseti(L, -2, n+1);

    lua_pop(L, 1);
    return 1;
}

static int vel_getins(lua_State* L) {
    lua_getfield(L, LUA_REGISTRYINDEX, "_instances");
    return 1;
}

void vel_inslib(lua_State* L) {
    luaL_Reg reg[] = {
        {"new", vel_inew},
        {"GetAllInstances", vel_getins},
        
        {NULL, NULL}
    };
    lua_pushvalue(L, LUA_GLOBALSINDEX);
    luaL_register(L, "Instance", reg);

    lua_pop(L, 1);

    lua_newtable(L);
    lua_setfield(L, LUA_REGISTRYINDEX, "_instances");
}
