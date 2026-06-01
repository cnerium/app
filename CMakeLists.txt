cmake_minimum_required(VERSION 3.20)

project(cnerium
    VERSION 0.1.0
    DESCRIPTION "Reliability-first application framework for Vix and Softadastra"
    LANGUAGES CXX
)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
include(CTest)

# ========================
# Options
# ========================
include(cmake/CneriumOptions.cmake)

# ========================
# Global settings
# ========================
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# ========================
# Dependencies
# ========================
include(cmake/CneriumDependencies.cmake)

# ========================
# Cnerium library
# ========================
add_library(cnerium
    src/support/Error.cpp
    src/support/Json.cpp
    src/support/String.cpp

    src/store/StoreKey.cpp
    src/store/StoredResponse.cpp
    src/store/Store.cpp

    src/reliability/IdempotencyKey.cpp
    src/reliability/RequestHash.cpp
    src/reliability/ReplayProtection.cpp
    src/reliability/Idempotency.cpp

    src/http/DurableRequest.cpp
    src/http/DurableResponse.cpp
    src/http/DurableRoute.cpp

    src/realtime/Event.cpp
    src/realtime/RealtimeConfig.cpp
    src/realtime/Realtime.cpp

    src/adapters/SoftadastraStore.cpp
    src/adapters/VixHttp.cpp
    src/adapters/VixWebSocket.cpp

    src/app/AppConfig.cpp
    src/app/AppRuntime.cpp
    src/app/App.cpp
)

add_library(cnerium::cnerium ALIAS cnerium)
add_library(softadastra::cnerium ALIAS cnerium)

target_include_directories(cnerium
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(cnerium
    PUBLIC
        ${CNERIUM_PUBLIC_DEPENDENCIES}
)

target_compile_features(cnerium PUBLIC cxx_std_20)

set_target_properties(cnerium PROPERTIES
    OUTPUT_NAME cnerium
    VERSION ${PROJECT_VERSION}
    SOVERSION 0
    EXPORT_NAME cnerium
)

if(CNERIUM_ENABLE_PCH)
    target_precompile_headers(cnerium PRIVATE
        <cstddef>
        <cstdint>
        <memory>
        <optional>
        <string>
        <string_view>
        <utility>
        <vector>
    )
endif()

if(MSVC)
    target_compile_options(cnerium PRIVATE /W4)

    if(CNERIUM_WARNINGS_AS_ERRORS)
        target_compile_options(cnerium PRIVATE /WX)
    endif()
else()
    target_compile_options(cnerium PRIVATE -Wall -Wextra -Wpedantic)

    if(CNERIUM_WARNINGS_AS_ERRORS)
        target_compile_options(cnerium PRIVATE -Werror)
    endif()
endif()

# ========================
# Examples
# ========================
if(CNERIUM_BUILD_EXAMPLES)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/examples/durable-orders-realtime/src/main.cpp")
        add_executable(cnerium_durable_orders_realtime
            examples/durable-orders-realtime/src/main.cpp
        )

        target_link_libraries(cnerium_durable_orders_realtime
            PRIVATE
                cnerium
        )

        target_compile_features(cnerium_durable_orders_realtime
            PRIVATE
                cxx_std_20
        )
    endif()
endif()

# ========================
# Tests
# ========================
if(CNERIUM_BUILD_TESTS)
    enable_testing()

    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/CMakeLists.txt")
        add_subdirectory(tests)
    endif()
endif()

# ========================
# Install
# ========================
include(cmake/CneriumInstall.cmake)

# ========================
# Summary
# ========================

if(NOT CMAKE_BUILD_TYPE)
    set(CNERIUM_BUILD_TYPE "Default")
else()
    set(CNERIUM_BUILD_TYPE "${CMAKE_BUILD_TYPE}")
endif()

message(STATUS "------------------------------------------------------")
message(STATUS "cnerium configured (${PROJECT_VERSION})")
message(STATUS "Build examples:       ${CNERIUM_BUILD_EXAMPLES}")
message(STATUS "Build tests:          ${CNERIUM_BUILD_TESTS}")
message(STATUS "Install:              ${CNERIUM_INSTALL}")
message(STATUS "Warnings as errors:   ${CNERIUM_WARNINGS_AS_ERRORS}")
message(STATUS "Local deps:           ${CNERIUM_ENABLE_LOCAL_DEPS}")
message(STATUS "PCH:                  ${CNERIUM_ENABLE_PCH}")
message(STATUS "Vix:                  ${CNERIUM_VIX_TARGET}")
message(STATUS "Softadastra SDK:      ${CNERIUM_SOFTADASTRA_SDK_TARGET}")
message(STATUS "Build type:           ${CNERIUM_BUILD_TYPE}")
message(STATUS "Install prefix:       ${CMAKE_INSTALL_PREFIX}")
message(STATUS "------------------------------------------------------")
