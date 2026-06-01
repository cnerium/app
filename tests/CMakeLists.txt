# ========================
# Cnerium tests
# ========================

find_package(GTest CONFIG QUIET)

if(NOT GTest_FOUND)
    find_package(GTest QUIET)
endif()

if(NOT GTest_FOUND)
    message(FATAL_ERROR
        "GTest was not found.\n"
        "Install GTest or disable CNERIUM_BUILD_TESTS."
    )
endif()

add_executable(cnerium_tests
    StoreTests.cpp
    RequestHashTests.cpp
    IdempotencyTests.cpp
    DurablePostTests.cpp
    RealtimeTests.cpp
)

target_link_libraries(cnerium_tests
    PRIVATE
        cnerium
        GTest::gtest
        GTest::gtest_main
)

target_compile_features(cnerium_tests
    PRIVATE
        cxx_std_20
)

if(MSVC)
    target_compile_options(cnerium_tests PRIVATE /W4)

    if(CNERIUM_WARNINGS_AS_ERRORS)
        target_compile_options(cnerium_tests PRIVATE /WX)
    endif()
else()
    target_compile_options(cnerium_tests PRIVATE -Wall -Wextra -Wpedantic)

    if(CNERIUM_WARNINGS_AS_ERRORS)
        target_compile_options(cnerium_tests PRIVATE -Werror)
    endif()
endif()

add_test(
    NAME cnerium_tests
    COMMAND cnerium_tests
)
