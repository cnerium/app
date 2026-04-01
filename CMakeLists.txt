cmake_minimum_required(VERSION 3.20)
project(app LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(app INTERFACE)
add_library(app::app ALIAS app)

target_include_directories(app INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

if (MSVC)
  target_compile_options(app INTERFACE /W4 /permissive-)
else()
  target_compile_options(app INTERFACE -Wall -Wextra -Wpedantic)
endif()

include(CTest)
enable_testing()

add_executable(app_basic_test tests/test_basic.cpp)
target_link_libraries(app_basic_test PRIVATE app::app)
add_test(NAME app.basic COMMAND app_basic_test)
