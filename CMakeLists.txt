cmake_minimum_required(VERSION 3.30)
project(GameProject)

set(CMAKE_CXX_STANDARD 26)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

set(SDL2TTF_VENDORED ON CACHE BOOL "" FORCE)
set(SDL2MIXER_VENDORED ON CACHE BOOL "" FORCE)
set(SDL2MIXER_OPUS ON CACHE BOOL "" FORCE)

add_subdirectory(external/SDL EXCLUDE_FROM_ALL)
add_subdirectory(external/SDL_image EXCLUDE_FROM_ALL)
add_subdirectory(external/SDL_ttf EXCLUDE_FROM_ALL)
add_subdirectory(external/SDL_mixer EXCLUDE_FROM_ALL)

add_executable(${PROJECT_NAME} src/main.cpp
        include/Game.hpp
        src/Game.cpp
        include/Player.hpp
        src/Player.cpp
        include/Platform.hpp
        src/Platform.cpp
        include/TextureManager.hpp
        src/TextureManager.cpp
        include/GameState.hpp
        include/Button.hpp
        include/Background.hpp
        src/Background.cpp
        include/MenuState.hpp
        include/PlayState.hpp
        include/PauseState.hpp
        src/MenuState.cpp
        src/PlayState.cpp
        src/PauseState.cpp
        src/Button.cpp
        include/InstructionState.hpp
        src/InstructionState.cpp
        include/GameOverState.hpp
        src/GameOverState.cpp
        include/LevelCompleteState.hpp
        src/LevelCompleteState.cpp
        include/ExitDoor.hpp
        src/ExitDoor.cpp
        include/AudioManager.hpp
        src/AudioManger.cpp
        include/Slider.hpp
        src/Slider.cpp
        include/SettingsState.hpp
        src/SettingsState.cpp
)

if (CMAKE_BUILD_TYPE MATCHES Release)
    if (MSVC)
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
    else()
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mwindows")
    endif()
endif ()

target_include_directories(${PROJECT_NAME} PRIVATE include SDL2_image::SDL2_image SDL2::SDL2main SDL2::SDL2 SDL2_ttf::SDL2_ttf SDL2_mixer::SDL2_mixer)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL2_image::SDL2_image SDL2::SDL2main SDL2::SDL2 SDL2_ttf::SDL2_ttf SDL2_mixer::SDL2_mixer)



add_custom_target(copy_assets
        COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=$<CONFIGURATION> -P ${CMAKE_CURRENT_LIST_DIR}/copy-assets.cmake
)
add_dependencies(${PROJECT_NAME} copy_assets)