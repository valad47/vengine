#!/bin/bash

cmake -B build -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build build -j8

rm build/libvengine_bundle.a 
ar cqT build/libvengine_bundle.a \
   build/libvengine.a \
   build/luau/libLuau.VM.a \
   build/luau/libLuau.Compiler.a \
   build/luau/libLuau.Ast.a \
   build/_deps/raylib-build/raylib/libraylib.a

echo -e 'create build/libvengine_bundle.a\naddlib build/libvengine_bundle.a\nsave\nend' | ar -M
