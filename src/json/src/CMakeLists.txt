# CMake json/src

# [Build the library]

include_directories($ENV{WASABI_EXTERNAL_DIR})
include_directories(${PROJECT_SOURCE_DIR}/include)

set(SRCS
  jsonReader.cpp)

set(HEADERS
  ${PROJECT_SOURCE_DIR}/include/json/jsonReader.h)


add_library(jsonUtils.wasm ${SRCS} ${HEADERS})


## [installation]

install(TARGETS jsonUtils.wasm
  DESTINATION ${WASABI_ROOT}/src/build/lib)
