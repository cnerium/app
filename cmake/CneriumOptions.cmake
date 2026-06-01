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
    ""
    CACHE PATH "Optional path to a local Vix source tree"
)

set(CNERIUM_SDK_SOURCE_DIR
    ""
    CACHE PATH "Optional path to a local Softadastra SDK source tree"
)

# ========================
# Build policy
# ========================

option(CNERIUM_ENABLE_LOCAL_DEPS
    "Allow Cnerium to use explicitly provided local dependency source trees"
    ON
)

option(CNERIUM_ENABLE_PCH
    "Enable private precompiled headers for Cnerium"
    OFF
)
