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


## [test] TODO : @cmea

# This part is so the Modern CMake book can verify this example builds. For your code,
# you'll probably want tests too
#enable_testing()
#add_test(NAME TestJson COMMAND TestJson)
