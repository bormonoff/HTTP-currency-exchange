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

# Model lib -------------
add_library(model_lib STATIC 
	src/model/exchange.h
  src/model/exchange.cpp
  src/model/broker.h
  src/model/broker.cpp
  src/util/util.h
  src/util/util.cpp
)

target_include_directories(model_lib PUBLIC src)
# Server ----------------
add_executable(server
	  src/main.cpp
    src/core/server.h
    src/core/server.cpp
    src/handler/handler.h
)

target_link_libraries(server PRIVATE Threads::Threads CONAN_PKG::boost model_lib)
target_include_directories(server PRIVATE CONAN_PKG::boost src)
# -----------------------

# Server tests ----------
add_executable(model_test
    tests/model_tests.cpp
)

target_link_libraries(model_test PRIVATE CONAN_PKG::catch2 model_lib)
target_include_directories(model_test PRIVATE src)
# -----------------------