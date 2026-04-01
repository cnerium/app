cmake_minimum_required(VERSION 3.20)

project(cnerium_app
    VERSION 0.2.0
    DESCRIPTION "Cnerium App - High-level application layer for the Cnerium web framework"
    LANGUAGES CXX
)

include(GNUInstallDirs)

# ========================
# Options
# ========================
option(CNERIUM_APP_BUILD_TESTS "Build app tests" OFF)
option(CNERIUM_APP_BUILD_EXAMPLES "Build app examples" OFF)

# ========================
# Global settings
# ========================
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# ========================
# Dependencies
# ========================
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/.vix/vix_deps.cmake")
    include("${CMAKE_CURRENT_SOURCE_DIR}/.vix/vix_deps.cmake")
endif()

if(NOT TARGET cnerium::server
   AND NOT TARGET cnerium_server
   AND NOT TARGET server::server
   AND NOT TARGET server)
    find_package(cnerium_server CONFIG QUIET)
    find_package(server CONFIG QUIET)
endif()

if(NOT TARGET cnerium::runtime
   AND NOT TARGET cnerium_runtime
   AND NOT TARGET runtime::runtime
   AND NOT TARGET runtime)
    find_package(cnerium_runtime CONFIG QUIET)
    find_package(runtime CONFIG QUIET)
endif()

if(NOT TARGET cnerium::http
   AND NOT TARGET cnerium_http
   AND NOT TARGET http::http
   AND NOT TARGET http)
    find_package(cnerium_http CONFIG QUIET)
    find_package(http CONFIG QUIET)
endif()

if(NOT TARGET cnerium::middleware
   AND NOT TARGET cnerium_middleware
   AND NOT TARGET middleware::middleware
   AND NOT TARGET middleware)
    find_package(cnerium_middleware CONFIG QUIET)
    find_package(middleware CONFIG QUIET)
endif()

if(NOT TARGET cnerium::router
   AND NOT TARGET cnerium_router
   AND NOT TARGET router::router
   AND NOT TARGET router)
    find_package(cnerium_router CONFIG QUIET)
    find_package(router CONFIG QUIET)
endif()

if(NOT TARGET cnerium::json
   AND NOT TARGET cnerium_json
   AND NOT TARGET json::json
   AND NOT TARGET json)
    find_package(cnerium_json CONFIG QUIET)
    find_package(json CONFIG QUIET)
endif()

find_package(Threads REQUIRED)

# ========================
# Resolve targets
# ========================
set(CNERIUM_APP_SERVER_TARGET "")
set(CNERIUM_APP_RUNTIME_TARGET "")
set(CNERIUM_APP_HTTP_TARGET "")
set(CNERIUM_APP_MIDDLEWARE_TARGET "")
set(CNERIUM_APP_ROUTER_TARGET "")
set(CNERIUM_APP_JSON_TARGET "")

if(TARGET cnerium::server)
    set(CNERIUM_APP_SERVER_TARGET cnerium::server)
elseif(TARGET cnerium_server)
    set(CNERIUM_APP_SERVER_TARGET cnerium_server)
elseif(TARGET server::server)
    set(CNERIUM_APP_SERVER_TARGET server::server)
elseif(TARGET server)
    set(CNERIUM_APP_SERVER_TARGET server)
endif()

if(TARGET cnerium::runtime)
    set(CNERIUM_APP_RUNTIME_TARGET cnerium::runtime)
elseif(TARGET cnerium_runtime)
    set(CNERIUM_APP_RUNTIME_TARGET cnerium_runtime)
elseif(TARGET runtime::runtime)
    set(CNERIUM_APP_RUNTIME_TARGET runtime::runtime)
elseif(TARGET runtime)
    set(CNERIUM_APP_RUNTIME_TARGET runtime)
endif()

if(TARGET cnerium::http)
    set(CNERIUM_APP_HTTP_TARGET cnerium::http)
elseif(TARGET cnerium_http)
    set(CNERIUM_APP_HTTP_TARGET cnerium_http)
elseif(TARGET http::http)
    set(CNERIUM_APP_HTTP_TARGET http::http)
elseif(TARGET http)
    set(CNERIUM_APP_HTTP_TARGET http)
endif()

if(TARGET cnerium::middleware)
    set(CNERIUM_APP_MIDDLEWARE_TARGET cnerium::middleware)
elseif(TARGET cnerium_middleware)
    set(CNERIUM_APP_MIDDLEWARE_TARGET cnerium_middleware)
elseif(TARGET middleware::middleware)
    set(CNERIUM_APP_MIDDLEWARE_TARGET middleware::middleware)
elseif(TARGET middleware)
    set(CNERIUM_APP_MIDDLEWARE_TARGET middleware)
endif()

if(TARGET cnerium::router)
    set(CNERIUM_APP_ROUTER_TARGET cnerium::router)
elseif(TARGET cnerium_router)
    set(CNERIUM_APP_ROUTER_TARGET cnerium_router)
elseif(TARGET router::router)
    set(CNERIUM_APP_ROUTER_TARGET router::router)
elseif(TARGET router)
    set(CNERIUM_APP_ROUTER_TARGET router)
endif()

if(TARGET cnerium::json)
    set(CNERIUM_APP_JSON_TARGET cnerium::json)
elseif(TARGET cnerium_json)
    set(CNERIUM_APP_JSON_TARGET cnerium_json)
elseif(TARGET json::json)
    set(CNERIUM_APP_JSON_TARGET json::json)
elseif(TARGET json)
    set(CNERIUM_APP_JSON_TARGET json)
endif()

# ========================
# Hard validation
# ========================
if(CNERIUM_APP_SERVER_TARGET STREQUAL "")
    message(FATAL_ERROR
        "cnerium server target not found.\n"
        "Install dependencies with Vix:\n"
        "  vix add @cnerium/server\n"
        "  vix install"
    )
endif()

if(CNERIUM_APP_RUNTIME_TARGET STREQUAL "")
    message(FATAL_ERROR
        "cnerium runtime target not found.\n"
        "Install dependencies with Vix:\n"
        "  vix add @cnerium/runtime\n"
        "  vix install"
    )
endif()

if(CNERIUM_APP_HTTP_TARGET STREQUAL "")
    message(FATAL_ERROR
        "cnerium http target not found.\n"
        "Install dependencies with Vix:\n"
        "  vix add @cnerium/http\n"
        "  vix install"
    )
endif()

if(CNERIUM_APP_MIDDLEWARE_TARGET STREQUAL "")
    message(FATAL_ERROR
        "cnerium middleware target not found.\n"
        "Install dependencies with Vix:\n"
        "  vix add @cnerium/middleware\n"
        "  vix install"
    )
endif()

if(CNERIUM_APP_ROUTER_TARGET STREQUAL "")
    message(FATAL_ERROR
        "cnerium router target not found.\n"
        "Install dependencies with Vix:\n"
        "  vix add @cnerium/router\n"
        "  vix install"
    )
endif()

if(CNERIUM_APP_JSON_TARGET STREQUAL "")
    message(FATAL_ERROR
        "cnerium json target not found.\n"
        "Install dependencies with Vix:\n"
        "  vix add @cnerium/json\n"
        "  vix install"
    )
endif()

# ========================
# Library (header-only)
# ========================
add_library(cnerium_app INTERFACE)
add_library(cnerium::app ALIAS cnerium_app)

target_compile_features(cnerium_app INTERFACE cxx_std_20)

target_include_directories(cnerium_app
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(cnerium_app
    INTERFACE
        ${CNERIUM_APP_SERVER_TARGET}
        ${CNERIUM_APP_RUNTIME_TARGET}
        ${CNERIUM_APP_HTTP_TARGET}
        ${CNERIUM_APP_MIDDLEWARE_TARGET}
        ${CNERIUM_APP_ROUTER_TARGET}
        ${CNERIUM_APP_JSON_TARGET}
        Threads::Threads
)

# ========================
# Compiler options
# ========================
if(MSVC)
    target_compile_options(cnerium_app INTERFACE /W4 /permissive-)
else()
    target_compile_options(cnerium_app INTERFACE -Wall -Wextra -Wpedantic)
endif()

# ========================
# Target properties
# ========================
set_target_properties(cnerium_app PROPERTIES
    EXPORT_NAME app
)

# ========================
# Examples
# ========================
if(CNERIUM_APP_BUILD_EXAMPLES AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/examples/CMakeLists.txt")
    add_subdirectory(examples)
endif()

# ========================
# Tests
# ========================
if(CNERIUM_APP_BUILD_TESTS)
    include(CTest)
    enable_testing()

    function(cnerium_add_app_test target source_file test_name)
        if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/${source_file}")
            add_executable(${target} "tests/${source_file}")
            target_link_libraries(${target} PRIVATE cnerium::app)
            target_compile_features(${target} PRIVATE cxx_std_20)

            if(MSVC)
                target_compile_options(${target} PRIVATE /W4 /permissive-)
            else()
                target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
            endif()

            add_test(NAME ${test_name} COMMAND ${target})
        endif()
    endfunction()

    cnerium_add_app_test(cnerium_app_test_basic                  test_basic.cpp                  cnerium_app.basic)
    cnerium_add_app_test(cnerium_app_test_app_config             test_app_config.cpp             cnerium_app.config)
    cnerium_add_app_test(cnerium_app_test_app_context            test_app_context.cpp            cnerium_app.context)
    cnerium_add_app_test(cnerium_app_test_app_routes             test_app_routes.cpp             cnerium_app.routes)
    cnerium_add_app_test(cnerium_app_test_app_runtime            test_app_runtime.cpp            cnerium_app.runtime)
    cnerium_add_app_test(cnerium_app_test_app_server_integration test_app_server_integration.cpp cnerium_app.server_integration)
endif()

# ========================
# Summary
# ========================
message(STATUS "------------------------------------------------------")
message(STATUS "cnerium::app configured (${PROJECT_VERSION})")
message(STATUS "Server target:      ${CNERIUM_APP_SERVER_TARGET}")
message(STATUS "Runtime target:     ${CNERIUM_APP_RUNTIME_TARGET}")
message(STATUS "HTTP target:        ${CNERIUM_APP_HTTP_TARGET}")
message(STATUS "Middleware target:  ${CNERIUM_APP_MIDDLEWARE_TARGET}")
message(STATUS "Router target:      ${CNERIUM_APP_ROUTER_TARGET}")
message(STATUS "JSON target:        ${CNERIUM_APP_JSON_TARGET}")
message(STATUS "Include dir:        ${CMAKE_CURRENT_SOURCE_DIR}/include")
message(STATUS "Build tests:        ${CNERIUM_APP_BUILD_TESTS}")
message(STATUS "Build examples:     ${CNERIUM_APP_BUILD_EXAMPLES}")
message(STATUS "Build type:         ${CMAKE_BUILD_TYPE}")
message(STATUS "------------------------------------------------------")
