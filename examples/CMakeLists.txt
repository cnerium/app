cmake_minimum_required(VERSION 3.20)

project(cnerium_app_examples LANGUAGES CXX)

# ========================
# Helpers
# ========================
function(cnerium_add_app_example target source_file)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${source_file}")
        add_executable(${target} "${source_file}")
        target_link_libraries(${target} PRIVATE cnerium::app)
        target_compile_features(${target} PRIVATE cxx_std_20)

        if(MSVC)
            target_compile_options(${target} PRIVATE /W4 /permissive-)
        else()
            target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
        endif()
    endif()
endfunction()

# ========================
# Examples
# ========================
cnerium_add_app_example(cnerium_app_example_01 01_hello_app.cpp)
cnerium_add_app_example(cnerium_app_example_02 02_routes_basic.cpp)
cnerium_add_app_example(cnerium_app_example_03 03_json_response.cpp)
cnerium_add_app_example(cnerium_app_example_04 04_middleware_basic.cpp)
cnerium_add_app_example(cnerium_app_example_05 05_runtime_app.cpp)
cnerium_add_app_example(cnerium_app_example_06 06_rest_api_complete.cpp)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/modular_users_api/CMakeLists.txt")
    add_subdirectory(modular_users_api)
endif()
