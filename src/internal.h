#pragma once

#include "lua.h"
#include "luacode.h"
#include "lualib.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct vengine_State {
    lua_State *L;
    lua_CFunction lua_loop;
} vengine_State;

// Macro that checks if lua state exist, without which, functions other than init cannot work
#define VENGINE_FUNC assert(L->L != NULL)

void vel_openlib(lua_State *L);
