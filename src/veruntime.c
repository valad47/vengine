#define VENGINE_IMPL

#include "internal.h"
#include "vengine.h"
#include "raylib.h"

#include <stdio.h>

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
        if (L->show_fps) {
            DrawFPS(5, 5);
        }

        EndDrawing();
    }
}
