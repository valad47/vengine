#define VENGINE_IMPL

#include "internal.h"
#include "vengine.h"
#include "raylib.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static int vel_setwindowsize(lua_State *L) {
    lua_settop(L, 2);
    lua_Number x = luaL_checknumber(L, -2), y = luaL_checknumber(L, -1);
    SetWindowSize(x, y);
    return 0;
}

static int vel_setfps(lua_State *L) {
    lua_settop(L, 1);
    SetTargetFPS((int)luaL_checknumber(L, 1));
    return 0;
}

static int vel_showfps(lua_State *L) {
    if(!lua_isboolean(L, 1)) {
        luaL_error(L, "ShowFPS claims only boolean argument");
    }

    lua_getglobal(L, "__vengine");
    vengine_State *VL = lua_tolightuserdata(L, 2);
    VL->show_fps = lua_toboolean(L, 1);

    return 0;
}

static int vel_clearbg(lua_State *L) {
    int argc = lua_gettop(L);
    if(argc < 3) {
        luaL_error(L, "Function expects 3 arguments, provided %d", argc);
    }

    ClearBackground((Color){
        lua_tointeger(L, 1),
        lua_tointeger(L, 2),
        lua_tointeger(L, 3),
        255
    });
    return 0;
}

const char *lua_globals =
"";

void load_libtask(lua_State *L) {
    FILE *fd = fopen("/home/valad47/dev/vengine/src/task.lua", "rb");
    fseek(fd, 0, SEEK_END);
    int length = ftell(fd);

    fseek(fd, 0, SEEK_SET);
    char *buffer = malloc(sizeof(char) * length);
    fread(buffer, 1, length, fd);

    size_t bytecode_size = 0;
    char *bytecode = luau_compile(buffer, length, NULL, &bytecode_size);
    if(luau_load(L, "libtask", bytecode, bytecode_size, 0) != LUA_OK) {
        printf("What the fuck you gave me? This file don't fucking work.\n%s", luaL_checkstring(L, -1));
        exit(1);
    }

    free(bytecode);
    free(buffer);

    if(lua_pcall(L, 0, 1, 0) != LUA_OK) {
        printf("your code is bullshit, check it again. Error:\n%s\n", luaL_checkstring(L, -1));
        exit(1);
    };

}

void vel_openlib(lua_State *L) {
    static const luaL_Reg funcs[] = {
        {"SetWindowSize", vel_setwindowsize},
        {"SetFPS", vel_setfps},
        {"ShowFPS", vel_showfps},
        {"ClearBackground", vel_clearbg},

        {NULL, NULL}
    };

    lua_pushvalue(L, LUA_GLOBALSINDEX);
    luaL_register(L, NULL, funcs);
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
}
