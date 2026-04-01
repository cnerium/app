cmake_minimum_required(VERSION 3.20)

# Examples for cnerium/app

add_executable(app_example_01 01_hello_app.cpp)
target_link_libraries(app_example_01 PRIVATE app::app)

add_executable(app_example_02 02_routes_basic.cpp)
target_link_libraries(app_example_02 PRIVATE app::app)

add_executable(app_example_03 03_json_response.cpp)
target_link_libraries(app_example_03 PRIVATE app::app)

add_executable(app_example_04 04_middleware_basic.cpp)
target_link_libraries(app_example_04 PRIVATE app::app)

add_executable(app_example_05 05_runtime_app.cpp)
target_link_libraries(app_example_05 PRIVATE app::app)

add_executable(app_example_06 06_rest_api_complete.cpp)
target_link_libraries(app_example_06 PRIVATE app::app)

add_subdirectory(modular_users_api)
