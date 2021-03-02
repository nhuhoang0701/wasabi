#!/usr/bin/env bash

$C_COMPILER \
-g \
--target=wasm32 \
--no-standard-libraries \
-Xlinker --import-memory \
-Xlinker --no-entry \
-Xlinker --export=tolower \
-fvisibility=hidden \
-Xlinker --allow-undefined-file=./js_fct.syms \
-o main.wasm main.cc
