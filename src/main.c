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

void vengine_loadstring(vengine_State* L, const char *str, int length, const char *chunkname) {
    VENGINE_FUNC;

    if(chunkname == NULL) {
        chunkname = "loaded_string";
    }

    size_t bytecode_size;
    char *bytecode = luau_compile(str, length, NULL, &bytecode_size);

    lua_getglobal(L->L, "task");
    lua_getfield(L->L, -1, "spawn");
    lua_remove(L->L, -2);

    lua_State* newthread = lua_newthread(L->L);
    int result = luau_load(newthread, chunkname, bytecode, bytecode_size, 0);
    luaL_sandboxthread(newthread);

    if(result != LUA_OK) {
        printf("Failed to load bytecode:\n%s\n", lua_tostring(newthread, -1));
        exit(1);
    }

    if(lua_pcall(L->L, 1, 0, 0) != LUA_OK) {
        printf("Failed to create thread:\n%s", lua_tostring(L->L, -1));
        exit(1);
    }

    free(bytecode);

    return;
}

void vengine_loadfile(vengine_State* L, const char *filename) {
    VENGINE_FUNC;

    FILE *file = fopen(filename, "rb");
    if(file == NULL) {
        perror("Failed to load file");
        exit(1);
    }
    fseek(file, 0, SEEK_END);
    int length = ftell(file);
    if(length < 0) {
        fclose(file);
        return;
    }
    fseek(file, 0, SEEK_SET);

    char *buffer = malloc(length);
    memset(buffer, 0, length);
    int readsize = fread(buffer, 1, length, file);

    vengine_loadstring(L, buffer, readsize, filename);

    free(buffer);
    fclose(file);
}
