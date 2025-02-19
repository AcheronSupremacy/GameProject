cmake_minimum_required(VERSION 3.30)
project(GameProject)

set(CMAKE_CXX_STANDARD 26)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

add_subdirectory(external/SDL EXCLUDE_FROM_ALL)
add_subdirectory(external/SDL_image EXCLUDE_FROM_ALL)

add_executable(${PROJECT_NAME} src/main.cpp)

if (CMAKE_BUILD_TYPE MATCHES Release)
    if (MSVC)
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
    else()
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mwindows")
    endif()
endif ()

target_include_directories(${PROJECT_NAME} PRIVATE SDL2_image::SDL2_image SDL2::SDL2main SDL2::SDL2)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL2_image::SDL2_image SDL2::SDL2main SDL2::SDL2)