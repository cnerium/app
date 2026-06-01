# ========================
# Vix dependency
# ========================

find_package(vix CONFIG QUIET)

if(NOT vix_FOUND)
    if(CNERIUM_ENABLE_LOCAL_DEPS AND EXISTS "${CNERIUM_VIX_SOURCE_DIR}/CMakeLists.txt")
        message(STATUS "[cnerium] using local Vix source: ${CNERIUM_VIX_SOURCE_DIR}")

        set(VIX_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
        set(VIX_BUILD_TESTS OFF CACHE BOOL "" FORCE)
        set(VIX_BUILD_APPS OFF CACHE BOOL "" FORCE)
        set(VIX_BUILD_CLI OFF CACHE BOOL "" FORCE)
        set(VIX_INSTALL OFF CACHE BOOL "" FORCE)

        add_subdirectory(
            "${CNERIUM_VIX_SOURCE_DIR}"
            "${CMAKE_CURRENT_BINARY_DIR}/_deps/vix"
        )
    else()
        message(STATUS "[cnerium] installed Vix package not found, trying module packages")

        find_package(vix_core CONFIG QUIET)
        find_package(vix_json CONFIG QUIET)
        find_package(vix_websocket CONFIG QUIET)
    endif()
else()
    message(STATUS "[cnerium] using installed Vix package")
endif()

# ========================
# Softadastra SDK dependency
# ========================

find_package(sdk-cpp CONFIG QUIET)

if(NOT sdk-cpp_FOUND)
    if(CNERIUM_ENABLE_LOCAL_DEPS AND EXISTS "${CNERIUM_SDK_SOURCE_DIR}/CMakeLists.txt")
        message(STATUS "[cnerium] using local Softadastra SDK source: ${CNERIUM_SDK_SOURCE_DIR}")

        set(SDK_CPP_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
        set(SDK_CPP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
        set(SDK_CPP_INSTALL OFF CACHE BOOL "" FORCE)

        add_subdirectory(
            "${CNERIUM_SDK_SOURCE_DIR}"
            "${CMAKE_CURRENT_BINARY_DIR}/_deps/sdk-cpp"
        )
    else()
        message(FATAL_ERROR
            "Softadastra SDK was not found.\n"
            "Install sdk-cpp first or set CNERIUM_SDK_SOURCE_DIR."
        )
    endif()
else()
    message(STATUS "[cnerium] using installed Softadastra SDK package")
endif()

# ========================
# Helper: resolve targets
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

cnerium_find_target(CNERIUM_VIX_CORE_TARGET
    vix::core
    vix_core
)

cnerium_find_target(CNERIUM_VIX_JSON_TARGET
    vix::json
    vix_json
)

cnerium_find_target(CNERIUM_VIX_WEBSOCKET_TARGET
    vix::websocket
    vix::ws
    vix_websocket
)

cnerium_find_target(CNERIUM_VIX_EXECUTOR_TARGET
    vix::executor
    vix_executor
)

cnerium_find_target(CNERIUM_SOFTADASTRA_SDK_TARGET
    softadastra::sdk
    sdk-cpp::sdk-cpp
    sdk-cpp
)

# ========================
# Required targets
# ========================

set(CNERIUM_REQUIRED_TARGETS
    CNERIUM_VIX_CORE_TARGET
    CNERIUM_VIX_JSON_TARGET
    CNERIUM_VIX_WEBSOCKET_TARGET
    CNERIUM_SOFTADASTRA_SDK_TARGET
)

foreach(required_target_var IN LISTS CNERIUM_REQUIRED_TARGETS)
    if("${${required_target_var}}" STREQUAL "")
        message(FATAL_ERROR
            "Required Cnerium dependency target not found: ${required_target_var}"
        )
    endif()
endforeach()

# ========================
# Public dependencies
# ========================

set(CNERIUM_PUBLIC_DEPENDENCIES
    ${CNERIUM_VIX_CORE_TARGET}
    ${CNERIUM_VIX_JSON_TARGET}
    ${CNERIUM_VIX_WEBSOCKET_TARGET}
    ${CNERIUM_SOFTADASTRA_SDK_TARGET}
)

if(NOT "${CNERIUM_VIX_EXECUTOR_TARGET}" STREQUAL "")
    list(APPEND CNERIUM_PUBLIC_DEPENDENCIES ${CNERIUM_VIX_EXECUTOR_TARGET})
endif()
