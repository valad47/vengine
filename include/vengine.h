#pragma once

#ifndef VENGINE_IMPL
typedef struct vengine_State vengine_State;
#endif

vengine_State* vengine_init();
int vengine_close(vengine_State* L);
void vengine_loadstring(vengine_State *L, const char *str, const char *chunkname);
void vengine_loadfile(vengine_State *L, const char *filename);
void vengine_runtime_start(vengine_State *L);
