cmake_minimum_required(VERSION 3.20)

project(cnerium_app
    VERSION 0.1.0
    DESCRIPTION "Cnerium App - High-level application layer for the Cnerium web framework"
    LANGUAGES CXX
)

include(GNUInstallDirs)

# ========================
# Options
# ========================
option(CNERIUM_APP_BUILD_TESTS "Build app tests" ON)
option(CNERIUM_APP_BUILD_EXAMPLES "Build app examples" ON)

# ========================
# Global settings
# ========================
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# ========================
# Vix deps
# ========================
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/.vix/vix_deps.cmake")
    include("${CMAKE_CURRENT_SOURCE_DIR}/.vix/vix_deps.cmake")
endif()

# ========================
# Dependencies
# ========================
find_package(cnerium_server CONFIG QUIET)
find_package(server CONFIG QUIET)

find_package(cnerium_runtime CONFIG QUIET)
find_package(runtime CONFIG QUIET)

find_package(cnerium_http CONFIG QUIET)
find_package(http CONFIG QUIET)

find_package(cnerium_middleware CONFIG QUIET)
find_package(middleware CONFIG QUIET)

find_package(cnerium_router CONFIG QUIET)
find_package(router CONFIG QUIET)

find_package(cnerium_json CONFIG QUIET)
find_package(json CONFIG QUIET)

find_package(Threads REQUIRED)

set(CNERIUM_APP_SERVER_TARGET "")
set(CNERIUM_APP_RUNTIME_TARGET "")
set(CNERIUM_APP_HTTP_TARGET "")
set(CNERIUM_APP_MIDDLEWARE_TARGET "")
set(CNERIUM_APP_ROUTER_TARGET "")
set(CNERIUM_APP_JSON_TARGET "")

if(TARGET server::server)
    set(CNERIUM_APP_SERVER_TARGET server::server)
elseif(TARGET server)
    set(CNERIUM_APP_SERVER_TARGET server)
elseif(TARGET cnerium::server)
    set(CNERIUM_APP_SERVER_TARGET cnerium::server)
elseif(TARGET cnerium_server)
    set(CNERIUM_APP_SERVER_TARGET cnerium_server)
endif()

if(TARGET runtime::runtime)
    set(CNERIUM_APP_RUNTIME_TARGET runtime::runtime)
elseif(TARGET runtime)
    set(CNERIUM_APP_RUNTIME_TARGET runtime)
elseif(TARGET cnerium::runtime)
    set(CNERIUM_APP_RUNTIME_TARGET cnerium::runtime)
elseif(TARGET cnerium_runtime)
    set(CNERIUM_APP_RUNTIME_TARGET cnerium_runtime)
endif()

if(TARGET http::http)
    set(CNERIUM_APP_HTTP_TARGET http::http)
elseif(TARGET http)
    set(CNERIUM_APP_HTTP_TARGET http)
elseif(TARGET cnerium::http)
    set(CNERIUM_APP_HTTP_TARGET cnerium::http)
elseif(TARGET cnerium_http)
    set(CNERIUM_APP_HTTP_TARGET cnerium_http)
endif()

if(TARGET middleware::middleware)
    set(CNERIUM_APP_MIDDLEWARE_TARGET middleware::middleware)
elseif(TARGET middleware)
    set(CNERIUM_APP_MIDDLEWARE_TARGET middleware)
elseif(TARGET cnerium::middleware)
    set(CNERIUM_APP_MIDDLEWARE_TARGET cnerium::middleware)
elseif(TARGET cnerium_middleware)
    set(CNERIUM_APP_MIDDLEWARE_TARGET cnerium_middleware)
endif()

if(TARGET router::router)
    set(CNERIUM_APP_ROUTER_TARGET router::router)
elseif(TARGET router)
    set(CNERIUM_APP_ROUTER_TARGET router)
elseif(TARGET cnerium::router)
    set(CNERIUM_APP_ROUTER_TARGET cnerium::router)
elseif(TARGET cnerium_router)
    set(CNERIUM_APP_ROUTER_TARGET cnerium_router)
endif()

if(TARGET json::json)
    set(CNERIUM_APP_JSON_TARGET json::json)
elseif(TARGET json)
    set(CNERIUM_APP_JSON_TARGET json)
elseif(TARGET cnerium::json)
    set(CNERIUM_APP_JSON_TARGET cnerium::json)
elseif(TARGET cnerium_json)
    set(CNERIUM_APP_JSON_TARGET cnerium_json)
endif()

if(CNERIUM_APP_SERVER_TARGET STREQUAL "")
    message(FATAL_ERROR
        "cnerium server target not found.\n"
        "Tried: server::server, server, cnerium::server, cnerium_server\n"
        "Install dependencies with Vix:\n"
        "  vix add @cnerium/server\n"
        "  vix install"
    )
endif()

if(CNERIUM_APP_RUNTIME_TARGET STREQUAL "")
    message(FATAL_ERROR
        "cnerium runtime target not found.\n"
        "Tried: runtime::runtime, runtime, cnerium::runtime, cnerium_runtime\n"
        "Install dependencies with Vix:\n"
        "  vix add @cnerium/runtime\n"
        "  vix install"
    )
endif()

if(CNERIUM_APP_HTTP_TARGET STREQUAL "")
    message(FATAL_ERROR
        "cnerium http target not found.\n"
        "Tried: http::http, http, cnerium::http, cnerium_http\n"
        "Install dependencies with Vix:\n"
        "  vix add @cnerium/http\n"
        "  vix install"
    )
endif()

if(CNERIUM_APP_MIDDLEWARE_TARGET STREQUAL "")
    message(FATAL_ERROR
        "cnerium middleware target not found.\n"
        "Tried: middleware::middleware, middleware, cnerium::middleware, cnerium_middleware\n"
        "Install dependencies with Vix:\n"
        "  vix add @cnerium/middleware\n"
        "  vix install"
    )
endif()

if(CNERIUM_APP_ROUTER_TARGET STREQUAL "")
    message(FATAL_ERROR
        "cnerium router target not found.\n"
        "Tried: router::router, router, cnerium::router, cnerium_router\n"
        "Install dependencies with Vix:\n"
        "  vix add @cnerium/router\n"
        "  vix install"
    )
endif()

if(CNERIUM_APP_JSON_TARGET STREQUAL "")
    message(FATAL_ERROR
        "cnerium json target not found.\n"
        "Tried: json::json, json, cnerium::json, cnerium_json\n"
        "Install dependencies with Vix:\n"
        "  vix add @cnerium/json\n"
        "  vix install"
    )
endif()

# ========================
# Library (header-only)
# ========================
add_library(app INTERFACE)
add_library(app::app ALIAS app)

target_compile_features(app INTERFACE cxx_std_20)

target_include_directories(app INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(app
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
    target_compile_options(app INTERFACE /W4 /permissive-)
else()
    target_compile_options(app INTERFACE -Wall -Wextra -Wpedantic)
endif()

# ========================
# Target properties
# ========================
set_target_properties(app PROPERTIES
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
            target_link_libraries(${target} PRIVATE app::app)
            target_compile_features(${target} PRIVATE cxx_std_20)

            if(MSVC)
                target_compile_options(${target} PRIVATE /W4 /permissive-)
            else()
                target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
            endif()

            add_test(NAME ${test_name} COMMAND ${target})
        endif()
    endfunction()

    cnerium_add_app_test(app_test_basic                  test_basic.cpp                  app.basic)
    cnerium_add_app_test(app_test_app_config             test_app_config.cpp             app.config)
    cnerium_add_app_test(app_test_app_context            test_app_context.cpp            app.context)
    cnerium_add_app_test(app_test_app_routes             test_app_routes.cpp             app.routes)
    cnerium_add_app_test(app_test_app_runtime            test_app_runtime.cpp            app.runtime)
    cnerium_add_app_test(app_test_app_server_integration test_app_server_integration.cpp app.server_integration)
endif()

# ========================
# Summary
# ========================
message(STATUS "------------------------------------------------------")
message(STATUS "cnerium app configured (${PROJECT_VERSION})")
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
