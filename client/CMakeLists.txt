cmake_minimum_required(VERSION 3.11)

project(game_server CXX)
set(CMAKE_CXX_STANDARD 20)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

find_package(Boost 1.78.0 REQUIRED)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
endif()

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

set(CMAKE_BUILD_TYPE Debug)

add_compile_definitions(BOOST_BEAST_USE_STD_STRING_VIEW)

# Client ----------------
add_executable(client
    src/main.cpp
    src/menu/menu.h
    src/menu/menu.cpp
    src/handler/handler.h
    src/handler/handler.cpp
    src/scenarios/scenario.h
    src/scenarios/scenario.cpp
    src/scenarios/request_former.h
    src/scenarios/request_former.cpp
    src/model/broker.h
    src/util/util.h
    src/util/util.cpp
    src/util/literal_storage.h
)

target_link_libraries(client PRIVATE Threads::Threads CONAN_PKG::boost)
target_include_directories(client PRIVATE CONAN_PKG::boost src)
# -----------------------