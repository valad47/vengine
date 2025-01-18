#define VENGINE_IMPL

#include "internal.h"
#include "vengine.h"
#include "raylib.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static int vel_setwindowsize(lua_State *L) {
    expect(L, 1, LUA_TNUMBER);
    expect(L, 2, LUA_TNUMBER);
    lua_Number x = lua_tonumber(L, 1), y = lua_tonumber(L, 2);
    SetWindowSize(x, y);
    return 0;
}

static int vel_setfps(lua_State *L) {
    expect(L, 1, LUA_TNUMBER);
    SetTargetFPS(lua_tointeger(L, 1));
    return 0;
}

static int vel_showfps(lua_State *L) {
    expect(L, 1, LUA_TBOOLEAN);

    lua_getglobal(L, "__vengine");
    vengine_State *VL = lua_tolightuserdata(L, 2);
    VL->show_fps = lua_toboolean(L, 1);

    return 0;
}

static int vel_setbg(lua_State *L) {
    expect(L, 1, LUA_TNUMBER);
    expect(L, 2, LUA_TNUMBER);
    expect(L, 3, LUA_TNUMBER);

    lua_getglobal(L, "__vengine");
    vengine_State *VL = lua_tolightuserdata(L, -1);
    VL->bg_color = (Color){
        lua_tointeger(L, 1),
        lua_tointeger(L, 2),
        lua_tointeger(L, 3),
        255
    };

    return 0;
}

static int vel_gettime(lua_State *L) {
    lua_pushnumber(L, GetTime());
    return 1;
}

const char *lua_globals =
"";

void load_libtask(lua_State *L) {
    char buffer[] = {
        #embed "task.lua"
        ,'\0'
    };
    size_t bytecode_size = 0;
    char *bytecode = luau_compile(buffer, strlen(buffer), NULL, &bytecode_size);
    if(luau_load(L, "task.lua", bytecode, bytecode_size, 0) != LUA_OK) {
        printf("Failed to load %s:\n\t%s\n", "task.lua", luaL_checkstring(L, -1));
        exit(1);
    }
    free(bytecode);
    if(lua_pcall(L, 0, 1, 0) != LUA_OK) {
        printf("Providen file has execution error:\n%s", luaL_checkstring(L, -1));
        exit(1);
    };
}

static int vel_setlocal(lua_State *L) {
    expect(L, 1, LUA_TSTRING);
    expect(L, 2, LUA_TFUNCTION);
    lua_settop(L, 2);
    lua_settable(L, LUA_REGISTRYINDEX);
    return 0;
}

void vel_openlib(lua_State *L) {
    static const luaL_Reg funcs[] = {
        {"SetWindowSize", vel_setwindowsize},
        {"SetFPS", vel_setfps},
        {"ShowFPS", vel_showfps},
        {"SetBackgroundColor", vel_setbg},
        {"GetTime", vel_gettime},

        {NULL, NULL}
    };

    lua_pushvalue(L, LUA_GLOBALSINDEX);
    luaL_register(L, NULL, funcs);
    lua_pop(L, 1);

    static const luaL_Reg locals[] = {
        {"setlocal", vel_setlocal},

        {NULL, NULL}
    };

    lua_pushvalue(L, LUA_ENVIRONINDEX);
    luaL_register(L, NULL, locals);
    lua_pop(L, 1);

    size_t bytecode_size = 0;
    char *bytecode = luau_compile(lua_globals, strlen(lua_globals), NULL, &bytecode_size);
    if(luau_load(L, "set_global", bytecode, bytecode_size, 0) != LUA_OK) {
        printf("\nError occured while setting globals: %s\n", lua_tostring(L, -1));
        exit(1);
    }
    free(bytecode);

    if(lua_pcall(L, 0, 0, 1) != LUA_OK) {
        printf("%s\n", lua_tostring(L, -1));
        exit(1);
    };
    load_libtask(L);
    vel_inslib(L);
}
