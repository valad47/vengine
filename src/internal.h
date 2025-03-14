#pragma once

#include "lua.h"
#include "luacode.h"
#include "lualib.h"
#include "raylib.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define expect(L, id, type)\
if(lua_type(L, id) != type)\
luaL_typeerror(L, id, lua_typename(L, type))

typedef struct vengine_State {
    lua_State *L;
    bool show_fps;
    Color bg_color;
} vengine_State;

typedef struct vengine_Instance {
    Vector2 pos;
    Color color;
} vengine_Instance;



typedef struct ve_Enum_Reg {
    const char *name;
    int value;
} ve_Enum_Reg;

typedef enum {
    VE_RECTANGLE = 1,
    VE_CIRCLE,
} VE_CLASS;

// Macro that checks if lua state exist, without which, functions other than init cannot work
#define VENGINE_FUNC assert(L->L != NULL)

void vel_inslib(lua_State *L);
void vel_openlib(lua_State *L);

void ve_reg(lua_State *L, const char* ename, ve_Enum_Reg* reg);
