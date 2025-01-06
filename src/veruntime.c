#define VENGINE_IMPL

#include "internal.h"
#include "vengine.h"
#include "raylib.h"

#include <stdio.h>

static void dumpstack (lua_State *L) {
  int top=lua_gettop(L);
  for (int i=1; i <= top; i++) {
    printf("%d\t%s\t", i, luaL_typename(L,i));
    switch (lua_type(L, i)) {
      case LUA_TNUMBER:
        printf("%g\n",lua_tonumber(L,i));
        break;
      case LUA_TSTRING:
        printf("%s\n",lua_tostring(L,i));
        break;
      case LUA_TBOOLEAN:
        printf("%s\n", (lua_toboolean(L, i) ? "true" : "false"));
        break;
      case LUA_TNIL:
        printf("%s\n", "nil");
        break;
      default:
        printf("%p\n",lua_topointer(L,i));
        break;
    }
  }
}


void vengine_runtime_start(vengine_State *L) {
    printf("Starting runtime\n");
    InitWindow(400, 800, "NOTHING");
    SetTargetFPS(60);
    while(!WindowShouldClose()) {
        BeginDrawing();

        lua_getglobal(L->L, "task");
        lua_getfield(L->L, -1, "step");
        if(lua_pcall(L->L, 0, 0, 0) != LUA_OK) {
            printf("Error during runtime:\n\t%s\n", lua_tostring(L->L, -1));
            exit(1);
        };
        //dumpstack(L->L);
        if (L->show_fps) {
            DrawFPS(5, 5);
        }

        EndDrawing();
    }
}
