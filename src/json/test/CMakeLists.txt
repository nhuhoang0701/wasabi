# CMake json/test

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories($ENV{WASABI_ROOT_DIR}/src/test_tools/include)

link_directories($ENV{WASABI_EXTERNAL_DIR}/cJSON/build)
link_directories($ENV{WASABI_ROOT_DIR}/src/json/build)
link_directories($ENV{WASABI_ROOT_DIR}/src/build/lib)

## [main]

set(SRCS
  test_json.cpp)

set(HEADERS
  test_data.h)


add_executable(TestJson.wasm ${SRCS} ${HEADERS})

target_link_libraries(TestJson.wasm PRIVATE cjson jsonUtils.wasm exceptions.wasm)


## [test]
# run with ctest -V
add_test(NAME TestJson COMMAND $ENV{WASMTIME} TestJson.wasm)
