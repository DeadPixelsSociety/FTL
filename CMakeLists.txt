cmake_minimum_required(VERSION 3.0)

find_package(gf REQUIRED)
include(GNUInstallDirs)

set(FTL_DATA_DIR "${CMAKE_INSTALL_PREFIX}/share/games/ftl")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h @ONLY)

add_definitions(-Wall -Wextra -g -O2 -std=c++11)

add_executable(ftl
    src/ftl.cc
    src/local/Crew.cc
    src/local/Room.cc
    src/local/RoomManager.cc
    src/local/Ship.cc
    src/local/Singletons.cc
)

target_include_directories(ftl
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(ftl
    gf::gf0
)

install(
    TARGETS ftl
    RUNTIME DESTINATION games
)

install(
    DIRECTORY "${CMAKE_SOURCE_DIR}/data/ftl"
    DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/games"
)
