#include "vengine.h"
#include "internal.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define VENGINE_FUNC assert(global_state != NULL)
#define L global_state

int vengine_init() {
    L = luaL_newstate();
    if(L == NULL) {
        printf("Failed to create new luau state");
        return 1;
    }
    luaL_openlibs(L);
    printf("Succesfully started engine");
    return 0;
}

int vengine_close() {
    VENGINE_FUNC;
    lua_close(L);
    L = NULL;

    return 0;
}

int vengine_loadstring(const char *str) {
    VENGINE_FUNC;

    size_t bytecode_size;
    char *bytecode = luau_compile(str, strlen(str), NULL, &bytecode_size);
    if(luau_load(L, "vengine", bytecode, bytecode_size, 0) != LUA_OK) {
        printf("Failed to load lua code: %s", luaL_checkstring(L, -1));
        free(bytecode);
        return 1;
    }
    free(bytecode);
    return 0;
}
