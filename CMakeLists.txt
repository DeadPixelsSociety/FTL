cmake_minimum_required(VERSION 3.0)

#set(GF_2048_DATA_DIR "${CMAKE_INSTALL_PREFIX}/share/games/gf_2048")
#configure_file(${CMAKE_CURRENT_SOURCE_DIR}/code/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h @ONLY)

find_package(gf REQUIRED)

add_definitions(-Wall -Wextra -g -O2 -std=c++11)

add_executable(ftl
  src/ftl.cc
  src/local/Ship.cc
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
