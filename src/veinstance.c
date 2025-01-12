#define VENGINE_IMPL

#include "internal.h"
#include "vengine.h"

#include <stdio.h>

const char *ve_classnames[] = {
    "Rectangle",
    "Circle"
};

const char *ve_getclassname(VE_CLASS n) {
    return ve_classnames[n-1];
}

static int vel_newindex(lua_State* L) {
    printf("Stack size: %d\n", lua_gettop(L));
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

    printf("Stack size at end: %d\n", lua_gettop(L));
    return 0;
}

static int vel_inew(lua_State* L) {
    expect(L, 1, LUA_TNUMBER);
    lua_newuserdata(L, sizeof(vengine_Instance));

    lua_newtable(L);
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");

    lua_pushcfunction(L, vel_newindex, NULL);
    lua_setfield(L, -2, "__newindex");

    lua_pushstring(L, "Instance");
    lua_setfield(L, -2, "__type");

    VE_CLASS class = lua_tointeger(L, 1);
    lua_pushstring(L, ve_getclassname(class));
    lua_setfield(L, -2, "classname");

    lua_pushstring(L, "It's new Instance!");
    lua_setfield(L, -2, "Name");

    lua_pushboolean(L, true);
    lua_setfield(L, -2, "Visible");

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

void ve_reg(lua_State *L, const char* ename, ve_Enum_Reg* reg) {
    lua_getfield(L, LUA_GLOBALSINDEX, "Enum");
    if(!lua_istable(L, -1)) {
        lua_pop(L, 1);
        lua_newtable(L);
        lua_pushvalue(L, -1);
        lua_setfield(L, LUA_GLOBALSINDEX, "Enum");
    }

    lua_newtable(L);
    lua_pushvalue(L, -1);
    lua_setfield(L, -3, ename);

    for(; reg->name; reg++) {
        lua_pushnumber(L, reg->value);
        lua_setfield(L, -2, reg->name);
    }
}

static int vel_enums(lua_State* L) {
    ve_Enum_Reg enums[] = {
        {"Rectangle", VE_RECTANGLE},
        {"Circle", VE_CIRCLE},

        {NULL, -1}
    };

    ve_reg(L, "Classes", enums);
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

    vel_enums(L);
}
