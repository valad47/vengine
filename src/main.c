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

void vengine_loadstring(vengine_State* L, const char *str, const char *chunkname) {
    VENGINE_FUNC;

    if(chunkname == NULL) {
        chunkname = "loaded_string";
    }

    size_t bytecode_size;
    char *bytecode = luau_compile(str, strlen(str), NULL, &bytecode_size);

    lua_getglobal(L->L, "task");
    lua_getfield(L->L, -1, "spawn");

    lua_State* newthread = lua_newthread(L->L);
    luau_load(newthread, chunkname, bytecode, bytecode_size, 0);
    luaL_sandboxthread(newthread);

    lua_call(L->L, 1, 0);
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
    fread(buffer, 1, length, file);

    vengine_loadstring(L, buffer, filename);

    free(buffer);
    fclose(file);
}
