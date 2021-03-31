cmake_minimum_required( VERSION 3.15.0 )

project( cern-grid CXX )

include(CheckCXXCompilerFlag)

OPTION (BUILD_SHARED_LIBS "Build shared libraries." ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CFLAGS " -pthread -g -ansi -pedantic -Wall -Wextra -Wno-sign-compare -Wsign-promo -Wabi -std=c++17 -Wno-pragma-once-outside-header")
set(CMAKE_CXX_FLAGS ${CFLAGS})

set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

find_package(ROOT REQUIRED)
include(${ROOT_USE_FILE})
include_directories(${ROOT_INCLUDE_DIRS})
link_directories(${ROOT_LIBRARIES})

# set( ALIROOT /home/bdrum/apps/alice/sw/ubuntu1804_x86-64/AliRoot/v5-09-53-1 )

find_program(ALIROOT_EXE NAMES aliroot PATHS $ENV{ALICE_ROOT}/bin NO_DEFAULT_PATH)
  
mark_as_advanced(ALIROOT_EXE)
  
include_directories(
    $ENV{ALICE_ROOT}/include
    $ENV{ALICE_ROOT}/include/pythia
)

link_directories($ENV{ALICE_ROOT}/lib)
    
include($ENV{ALICE_ROOT}/etc/AliRoot-config.cmake)
message(STATUS "Found AliRoot version : \"${AliRoot_VERSION}\", git hash : \"${AliRoot_REVISION}\"")


include_directories($ENV{ALICE_PHYSICS}/include)
link_directories($ENV{ALICE_PHYSICS}/lib)
    
message(STATUS "Found AliPhysics version : \"${ALIPHYSICS_VERSION}\", release : \"${ALIPHYSICS_RELEASE}\"")

# include_directories( /home/bdrum/apps/alice/sw/ubuntu1804_x86-64/ROOT/latest/include)
# link_directories(/home/bdrum/apps/alice/sw/ubuntu1804_x86-64/ROOT/latest/lib)

add_subdirectory(selection)
add_subdirectory(utils)


# add_custom_target(gfull COMMAND aliroot -l -q "runAnalysis.C(false, false, false, true)")
# add_custom_target(gtest COMMAND aliroot -l -q "runAnalysis.C(false, true, false, true)")
# add_custom_target(local COMMAND aliroot -l -q "runAnalysis.C(true, false, false, true)")

# add_custom_target(merge_grid  COMMAND aliroot -l -q "runAnalysis.C(false, false, true, true)")
# add_custom_target(merge_local COMMAND aliroot -l -q "runAnalysis.C(false, false, true, false)")

# add_custom_command(
#         TARGET local POST_BUILD
#         COMMAND ${CMAKE_COMMAND} -E copy
#                 ${CMAKE_SOURCE_DIR}/selection/RhoPrime/macro/runAnalysis.C
#                 ${CMAKE_CURRENT_BINARY_DIR}/runAnalysis.C)

# add_custom_command(
#         TARGET local POST_BUILD
#         COMMAND ${CMAKE_COMMAND} -E copy
#                 ${CMAKE_SOURCE_DIR}/selection/RhoPrime/macro/AddTaskUpc4Prongs.C
#                 ${CMAKE_CURRENT_BINARY_DIR}/AddTaskUpc4Prongs.C)