#pragma once

#ifndef VENGINE_IMPL
typedef struct vengine_State vengine_State;
#endif

vengine_State* vengine_init();
int vengine_close(vengine_State* L);
int vengine_loadstring(vengine_State *L, const char *str);
void vengine_runtime_start(vengine_State *L);
