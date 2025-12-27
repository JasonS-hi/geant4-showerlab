cmake_minimum_required(VERSION 3.16)
project(geant4_showerlab)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Geant4 REQUIRED ui_all vis_all)
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cc )
add_executable(showerlab ${SOURCES} main.cc
)
target_link_libraries(showerlab ${Geant4_LIBRARIES})

# Copy macros into build dir for convenience
file(GLOB MACROS "${PROJECT_SOURCE_DIR}/macros/*.mac")
foreach(_macro ${MACROS})
    configure_file(${_macro} ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
endforeach()