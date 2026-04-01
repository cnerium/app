add_executable(cnerium_app_example_modular_users
    main.cpp
)

target_include_directories(cnerium_app_example_modular_users
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(cnerium_app_example_modular_users
    PRIVATE
        cnerium::app
)

target_compile_features(cnerium_app_example_modular_users PRIVATE cxx_std_20)

if(MSVC)
    target_compile_options(cnerium_app_example_modular_users PRIVATE /W4 /permissive-)
else()
    target_compile_options(cnerium_app_example_modular_users PRIVATE -Wall -Wextra -Wpedantic)
endif()
