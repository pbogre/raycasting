cmake_minimum_required(VERSION 3.21)
project(raycasting LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

add_executable(raycasting src/main.cpp)
target_link_libraries(raycasting PRIVATE sfml-graphics)
target_compile_features(raycasting PRIVATE cxx_std_17)
if (WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET raycasting POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:raycasting> $<TARGET_FILE_DIR:raycasting> COMMAND_EXPAND_LISTS)
endif()

install(TARGETS raycasting)
