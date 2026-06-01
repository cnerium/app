# ========================
# Cnerium options
# ========================

option(CNERIUM_BUILD_EXAMPLES "Build Cnerium examples" ON)
option(CNERIUM_BUILD_TESTS "Build Cnerium tests" OFF)
option(CNERIUM_INSTALL "Install Cnerium package" ON)
option(CNERIUM_WARNINGS_AS_ERRORS "Treat warnings as errors" OFF)

# ========================
# Dependency source paths
# ========================

set(CNERIUM_VIX_SOURCE_DIR
    "${CMAKE_CURRENT_SOURCE_DIR}/../vix"
    CACHE PATH "Path to Vix source tree"
)

set(CNERIUM_SDK_SOURCE_DIR
    "${CMAKE_CURRENT_SOURCE_DIR}/../sdk"
    CACHE PATH "Path to Softadastra SDK source tree"
)

# ========================
# Build policy
# ========================

set(CNERIUM_ENABLE_LOCAL_DEPS ON CACHE BOOL
    "Allow Cnerium to use local Vix and Softadastra SDK source trees when packages are not installed"
)

set(CNERIUM_ENABLE_PCH OFF CACHE BOOL
    "Enable private precompiled headers for Cnerium"
)
