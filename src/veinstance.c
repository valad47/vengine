#define VENGINE_IMPL

#include "internal.h"
#include "vengine.h"

#include <stdio.h>

static int vel_newindex(lua_State* L) {
    lua_getmetatable(L, 1);
    lua_pushvalue(L, 2);
    lua_gettable(L, 4);
    if(lua_isnil(L, -1)) {
        lua_getfield(L, 4, "Name");
        lua_getfield(L, 4, "__type");
        luaL_error(L, "%s is not a valid member of %s \"%s\"", lua_tostring(L, 2), lua_tostring(L, -1), lua_tostring(L, -2));
    }

    switch(lua_type(L, -1)) {
    case LUA_TSTRING:
        if(lua_type(L, 3) != LUA_TSTRING && lua_type(L, 3) != LUA_TNUMBER) {
            luaL_error(L, "Unable to assing property %s. Expected %s, got %s", lua_tostring(L, 2), lua_typename(L, LUA_TSTRING), luaL_typename(L, 3));
        }
        const char *str = lua_tostring(L, 3);
        lua_pushvalue(L, 2);
        lua_pushstring(L, str);
        lua_settable(L, 4);
        break;
    default:
        if(lua_type(L, 3) != lua_type(L, -1)) {
            luaL_error(L, "Unable to assing property %s. Expected %s, got %s", lua_tostring(L, 2), luaL_typename(L, -1), luaL_typename(L, 3));
        }
        lua_pushvalue(L, 2);
        lua_pushvalue(L, 3);
        lua_settable(L, 4);
    };
    return 0;
}

static int vel_inew(lua_State* L) {
    lua_settop(L, 0);
    lua_newuserdata(L, sizeof(vengine_Instance));

    lua_newtable(L);
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");

    lua_pushcfunction(L, vel_newindex, NULL);
    lua_setfield(L, -2, "__newindex");

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
