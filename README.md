vEngine
====

vEngine is small grafical engine, that uses [Luau](https://luau.org) as scripting language and [raylib](https://raylib.com) for rendering.

Currently it lacks of functionality and works only for Linux, but in future it will be a big, powerful library.

# Usage

All you need to use this library, is include files in directory `include` and compile executable with static library `libvengine_bundle.a`

Program code example:

```c
#include "vengine.h"
#include <string.h>

int main() {
    vengine_State *L = vengine_init(); //Initialising vEngine state

    vengine_loadfile(L, "main.luau"); //Loading file to a vengine state

    char *src = "print("Hello, vEngine!")"; //Simple Luau code string
    vengine_loadstring(L, src, strlen(src), "Chunkname"); //Loading string to a vengine state

    vengine_runtime_start(L); //Starting vengine runtime

    vengine_close(L); //Closing state after program exits
}
```

# Building

To build bundled library, use this command:

```sh
./build.sh
```

Or, if you want to link every library separately, use this:

```sh
cmake -B build
cmake --build build
```
