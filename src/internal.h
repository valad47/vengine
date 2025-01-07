#pragma once

#include "lua.h"
#include "luacode.h"
#include "lualib.h"
#include "raylib.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct vengine_State {
    lua_State *L;
    lua_CFunction lua_loop;
    bool show_fps;
    Color bg_color;
} vengine_State;

typedef struct vengine_Instance {
    Vector2 pos;
    Color color;
} vengine_Instance;

// Macro that checks if lua state exist, without which, functions other than init cannot work
#define VENGINE_FUNC assert(L->L != NULL)

void vel_inslib(lua_State *L);
void vel_openlib(lua_State *L);
