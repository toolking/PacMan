cmake_minimum_required( VERSION 3.25.0 )
project( PacMan VERSION 0.1.0 LANGUAGES C CXX )

include( FetchContent )
FetchContent_Declare(
  centurion
  GIT_REPOSITORY https://github.com/albin-johansson/centurion.git
  GIT_TAG adc604505c9b8b5b22360723adeb3b87e16ab91c # v7.3.0
)
FetchContent_MakeAvailable( centurion )

find_package( SDL2 REQUIRED )
find_package( fmt REQUIRED )

find_program( CLANG_TIDY_EXE NAMES "clang-tidy" )

# set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")

# the main program
add_executable( PacMan main.cpp
    src/Blinky.cpp
    src/Board.cpp
    src/Clyde.cpp
    src/Entity.cpp
    src/Fruit.cpp
    src/Game.cpp
    src/Ghost.cpp
    src/Inky.cpp
    src/Pac.cpp
    src/Pinky.cpp
    src/Texture.cpp
)

target_include_directories( PacMan PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include )
target_include_directories( PacMan SYSTEM PRIVATE ${SDL2_INCLUDE_DIRS} ${centurion_SOURCE_DIR}/src )
add_compile_definitions(CENTURION_MOCK_FRIENDLY_MODE)
target_link_libraries( PacMan PRIVATE fmt::fmt ${SDL2_LIBRARIES} SDL2_mixer SDL2_ttf SDL2_image )

target_compile_features( PacMan
  PUBLIC
    cxx_std_23
)

target_compile_options( PacMan
  PRIVATE
    -Wall
    -Wextra
    -Werror
    #-Wconversion
    #-Wsign-conversion
    -Wshadow
    -pedantic
    -pedantic-errors
)
