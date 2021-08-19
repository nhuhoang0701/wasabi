cmake_minimum_required(VERSION 3.16)

project(ninja-set-vars NONE)

set(WASABI_NINJA_DIR ${WASABI_EXTERNAL_DIR}/ninja CACHE PATH "Path of local ninja directory" FORCE)
message(DEBUG "WASABI_NINJA_DIR=" ${WASABI_NINJA_DIR})
set(NINJA ${WASABI_NINJA_DIR}/ninja CACHE PATH "Path of local binary ninja file" FORCE)
message(DEBUG "NINJA=" ${NINJA})
