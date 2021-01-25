# CMake json/test

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${WASABI_ROOT}/src/test_tools/include)

link_directories(${WASABI_EXTERNAL}/cJSON/build)
link_directories(${WASABI_ROOT}/src/json/build)
link_directories(${WASABI_ROOT}/src/build/lib)

## [main]

set(SRCS
  test_json.cpp)

set(HEADERS
  test_data.h)


add_executable(TestJson.wasm ${SRCS} ${HEADERS})

target_link_libraries(TestJson.wasm PRIVATE cjson jsonUtils.wasm exceptions.wasm)


## [test]
# run with ctest -V
add_test(NAME TestJson COMMAND ${WASABI_EXTERNAL}/wasmtime-v0.22.0/wasmtime TestJson.wasm)
