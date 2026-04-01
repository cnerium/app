add_executable(app_example_modular_users
    main.cpp
)

target_include_directories(app_example_modular_users
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(app_example_modular_users
    PRIVATE
        app::app
)

target_compile_features(app_example_modular_users PRIVATE cxx_std_20)

if(MSVC)
    target_compile_options(app_example_modular_users PRIVATE /W4 /permissive-)
else()
    target_compile_options(app_example_modular_users PRIVATE -Wall -Wextra -Wpedantic)
endif()
