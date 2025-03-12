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
    expect(L, 1, LUA_TNUMBER);

    lua_State* ML = lua_mainthread(L);
    lua_newuserdata(ML, sizeof(vengine_Instance));

    lua_newtable(ML);
    lua_pushvalue(ML, -1);
    lua_setfield(ML, -2, "__index");

    lua_pushcfunction(ML, vel_newindex, NULL);
    lua_setfield(ML, -2, "__newindex");

    lua_pushstring(ML, "Instance");
    lua_setfield(ML, -2, "__type");

    VE_CLASS class = lua_tointeger(L, 1);
    lua_pushstring(ML, ve_getclassname(class));
    lua_setfield(ML, -2, "classname");

    lua_pushstring(ML, "It's new Instance!");
    lua_setfield(ML, -2, "Name");

    lua_pushboolean(ML, true);
    lua_setfield(ML, -2, "Visible");

    lua_newtable(ML);
    lua_setfield(ML, -2, "Position");

    lua_newtable(ML);
    lua_setfield(ML, -2, "Size");

    lua_newtable(ML);
    lua_setfield(ML, -2, "Color");

    lua_getfield(ML, LUA_REGISTRYINDEX, "Signal");
    lua_getfield(ML, -1, "new");
    lua_pcall(ML, 0, 1, 0);
    lua_setfield(ML, -3, "OnFrame");
    lua_pop(ML, 1);

    lua_setmetatable(ML, -2);

    lua_getfield(ML, LUA_REGISTRYINDEX, "_instances");
    int n = lua_objlen(ML, -1);
    lua_pushvalue(ML, -2);
    lua_rawseti(ML, -2, n+1);

    lua_pop(ML, 1);
    lua_xpush(ML, L, -1);
    return 1;
}

static int vel_getins(lua_State* L) {
    lua_State* ML = lua_mainthread(L);
    lua_getfield(ML, LUA_REGISTRYINDEX, "_instances");
    lua_xpush(ML, L, -1);
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

void vel_enums(lua_State* L) {
    ve_Enum_Reg enums[] = {
        {"Rectangle", VE_RECTANGLE},
        {"Circle", VE_CIRCLE},

        {NULL, -1}
    };

    ve_reg(L, "Classes", enums);
}

void ve_drawscript(lua_State *L) {
    char buffer[] = {
        #embed "draw.lua"
        ,'\0'
    };
    size_t bytecode_size = 0;
    char *bytecode = luau_compile(buffer, strlen(buffer), NULL, &bytecode_size);
    if(luau_load(L, "draw.lua", bytecode, bytecode_size, 0) != LUA_OK) {
        printf("Failed to load %s:\n\t%s\n", "draw.lua", luaL_checkstring(L, -1));
        exit(1);
    }
    free(bytecode);
    if(lua_pcall(L, 0, 1, 0) != LUA_OK) {
        printf("Providen file has execution error:\n%s", luaL_checkstring(L, -1));
        exit(1);
    };

}
void ve_signalscript(lua_State *L) {
    char buffer[] = {
        #embed "signal.lua"
        ,'\0'
    };
    size_t bytecode_size = 0;
    char *bytecode = luau_compile(buffer, strlen(buffer), NULL, &bytecode_size);
    if(luau_load(L, "signal.lua", bytecode, bytecode_size, 0) != LUA_OK) {
        printf("Failed to load %s:\n\t%s\n", "signal.lua", luaL_checkstring(L, -1));
        exit(1);
    }
    free(bytecode);
    if(lua_pcall(L, 0, 1, 0) != LUA_OK) {
        printf("Providen file has execution error:\n%s", luaL_checkstring(L, -1));
        exit(1);
    };

}

static int vel_drawrec(lua_State* L) {
    DrawRectangle(
        lua_tonumber(L, 1),
        lua_tonumber(L, 2),
        lua_tonumber(L, 3),
        lua_tonumber(L, 4),
        (Color) {
            lua_tonumber(L, 5),
            lua_tonumber(L, 6),
            lua_tonumber(L, 7),
            255
        }
        );

    return 0;
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

    luaL_Reg reg2[] = {
        {"DrawRectangle", vel_drawrec},

        {NULL, NULL}
    };
    lua_pushvalue(L, LUA_REGISTRYINDEX);
    luaL_register(L, "__draw", reg2);

    lua_pop(L, 1);

    lua_newtable(L);
    lua_setfield(L, LUA_REGISTRYINDEX, "_instances");

    vel_enums(L);

    ve_drawscript(L);
    ve_signalscript(L);
}
