# ========================
# Cnerium dependencies
# ========================

function(cnerium_find_target output_var)
    foreach(candidate IN LISTS ARGN)
        if(TARGET "${candidate}")
            set(${output_var} "${candidate}" PARENT_SCOPE)
            return()
        endif()
    endforeach()

    set(${output_var} "" PARENT_SCOPE)
endfunction()

# ========================
# Default dependency prefixes
# ========================
#
# A user must be able to run:
#
#   cmake -S . -B build
#   vix build --build-target all -v
#
# without knowing CMAKE_PREFIX_PATH.
#

set(CNERIUM_DEFAULT_PREFIXES)

if(DEFINED ENV{HOME})
    list(APPEND CNERIUM_DEFAULT_PREFIXES
        "$ENV{HOME}/.local"
        "$ENV{HOME}/.softadastra/sdk"
        "$ENV{HOME}/.softadastra"
    )
endif()

if(DEFINED ENV{LOCALAPPDATA})
    list(APPEND CNERIUM_DEFAULT_PREFIXES
        "$ENV{LOCALAPPDATA}/Vix"
        "$ENV{LOCALAPPDATA}/Softadastra/sdk"
        "$ENV{LOCALAPPDATA}/Softadastra"
    )
endif()

foreach(prefix IN LISTS CNERIUM_DEFAULT_PREFIXES)
    if(EXISTS "${prefix}")
        list(PREPEND CMAKE_PREFIX_PATH "${prefix}")
    endif()
endforeach()

list(REMOVE_DUPLICATES CMAKE_PREFIX_PATH)

# ========================
# Optional local source trees
# ========================
#
# These paths are only used when explicitly provided.
# Installed packages remain the default behavior.
#

if(CNERIUM_ENABLE_LOCAL_DEPS)
    if(NOT "${CNERIUM_VIX_SOURCE_DIR}" STREQUAL "")
        if(EXISTS "${CNERIUM_VIX_SOURCE_DIR}/CMakeLists.txt")
            add_subdirectory(
                "${CNERIUM_VIX_SOURCE_DIR}"
                "${CMAKE_BINARY_DIR}/_deps/vix"
            )
        else()
            message(FATAL_ERROR
                "CNERIUM_VIX_SOURCE_DIR is set, but no CMakeLists.txt was found.\n"
                "Path: ${CNERIUM_VIX_SOURCE_DIR}"
            )
        endif()
    endif()

    if(NOT "${CNERIUM_SDK_SOURCE_DIR}" STREQUAL "")
        if(EXISTS "${CNERIUM_SDK_SOURCE_DIR}/CMakeLists.txt")
            add_subdirectory(
                "${CNERIUM_SDK_SOURCE_DIR}"
                "${CMAKE_BINARY_DIR}/_deps/softadastra-sdk"
            )
        else()
            message(FATAL_ERROR
                "CNERIUM_SDK_SOURCE_DIR is set, but no CMakeLists.txt was found.\n"
                "Path: ${CNERIUM_SDK_SOURCE_DIR}"
            )
        endif()
    endif()
endif()

# ========================
# Installed packages
# ========================

find_package(Vix CONFIG QUIET)
find_package(sdk-cpp CONFIG QUIET)

# ========================
# Main dependency targets
# ========================

cnerium_find_target(CNERIUM_VIX_TARGET
    vix::vix
    Vix::Vix
    vix
)

cnerium_find_target(CNERIUM_SOFTADASTRA_SDK_TARGET
    softadastra::sdk
    sdk-cpp::sdk-cpp
    sdk-cpp
)

# ========================
# Validation
# ========================

if("${CNERIUM_VIX_TARGET}" STREQUAL "")
    message(FATAL_ERROR
        "Vix development package was not found.\n"
        "Install Vix SDK first.\n"
        "Expected CMake package: VixConfig.cmake\n"
        "Expected target: vix::vix.\n"
        "Default search paths:\n"
        "  ~/.local\n"
        "  %LOCALAPPDATA%/Vix"
    )
endif()

if("${CNERIUM_SOFTADASTRA_SDK_TARGET}" STREQUAL "")
    message(FATAL_ERROR
        "Softadastra SDK development package was not found.\n"
        "Install Softadastra SDK first.\n"
        "Expected CMake package: sdk-cppConfig.cmake\n"
        "Expected target: softadastra::sdk, sdk-cpp::sdk-cpp or sdk-cpp.\n"
        "Default search paths:\n"
        "  ~/.softadastra/sdk\n"
        "  ~/.softadastra\n"
        "  %LOCALAPPDATA%/Softadastra/sdk\n"
        "  %LOCALAPPDATA%/Softadastra"
    )
endif()

# ========================
# Full Vix dependency set
# ========================
#
# Cnerium is built on top of the Vix + Softadastra ecosystem.
# It must link the complete available Vix SDK, because Cnerium
# adapters may directly use HTTP, WebSocket, async, runtime, JSON,
# process, crypto, cache, and other Vix modules.
#

set(CNERIUM_VIX_DEPENDENCIES
    ${CNERIUM_VIX_TARGET}
)

foreach(candidate
    vix::error
    vix::path
    vix::fs
    vix::io
    vix::env
    vix::os
    vix::utils
    vix::log
    vix::core
    vix::json
    vix::tests
    vix::time
    vix::conversion
    vix::validation
    vix::crypto
    vix::net
    vix::cache
    vix::sync
    vix::async
    vix::process
    vix::threadpool
    vix::game
    vix::kv
    vix::ai_agent
    vix::p2p
    vix::p2p_http
    vix::db
    vix::webrpc
    vix::websocket
    vix::middleware
    vix::template

    vix_error
    vix_path
    vix_fs
    vix_io
    vix_env
    vix_os
    vix_utils
    vix_log
    vix_core
    vix_json
    vix_tests
    vix_time
    vix_conversion
    vix_validation
    vix_crypto
    vix_net
    vix_cache
    vix_sync
    vix_async
    vix_process
    vix_threadpool
    vix_game
    vix_kv
    vix_ai_agent
    vix_p2p
    vix_p2p_http
    vix_db
    vix_webrpc
    vix_websocket
    vix_middleware
    template
)
    if(TARGET "${candidate}")
        list(APPEND CNERIUM_VIX_DEPENDENCIES "${candidate}")
    endif()
endforeach()

list(REMOVE_DUPLICATES CNERIUM_VIX_DEPENDENCIES)

# ========================
# Public dependency list
# ========================

set(CNERIUM_PUBLIC_DEPENDENCIES
    ${CNERIUM_VIX_DEPENDENCIES}
    ${CNERIUM_SOFTADASTRA_SDK_TARGET}
)

# ========================
# Summary
# ========================

message(STATUS "[cnerium] Vix target:      ${CNERIUM_VIX_TARGET}")
message(STATUS "[cnerium] Vix deps:        ${CNERIUM_VIX_DEPENDENCIES}")
message(STATUS "[cnerium] Softadastra SDK: ${CNERIUM_SOFTADASTRA_SDK_TARGET}")
