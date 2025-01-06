#!/bin/bash

rm build/libvengine_full.a 

ar cqT build/libvengine_full.a build/libvengine.a luau/build/*.a build/_deps/raylib-build/raylib/libraylib.a
echo -e 'create build/libvengine_full.a\naddlib build/libvengine_full.a\nsave\nend' | ar -M
