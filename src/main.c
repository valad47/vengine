#define VENGINE_IMPL

#include "internal.h"
#include "vengine.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

vengine_State* vengine_init() {
    vengine_State *L = malloc(sizeof(vengine_State));
    memset(L, 0, sizeof(vengine_State));
    L->L = luaL_newstate();
    if(L->L == NULL) {
        printf("Failed to create new luau state\n");
        free(L);

        return NULL;
    }
    luaL_openlibs(L->L);
    vel_openlib(L->L);

    lua_pushlightuserdata(L->L, L);
    lua_setglobal(L->L, "__vengine");

    printf("Succesfully started engine\n");
    return L;
}

int vengine_close(vengine_State* L) {
    VENGINE_FUNC;
    lua_close(L->L);
    free(L);

    return 0;
}

int vengine_loadstring(vengine_State* L, const char *str) {
    VENGINE_FUNC;

    size_t bytecode_size;
    char *bytecode = luau_compile(str, strlen(str), NULL, &bytecode_size);

    lua_getglobal(L->L, "task");
    lua_getfield(L->L, -1, "spawn");

    lua_State* newthread = lua_newthread(L->L);
    luau_load(newthread, "src", bytecode, bytecode_size, 0);
    luaL_sandboxthread(newthread);

    lua_call(L->L, 1, 0);
    free(bytecode);

    return 0;
}
