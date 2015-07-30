cmake_minimum_required (VERSION 2.6)
project (Worlds CXX)

set (BIN_DIR "${CMAKE_SOURCE_DIR}/bin")
set (SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})

add_executable(worlds ${SOURCE_DIR}/main.cpp)

# Libraries management
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

find_package (Irrlicht)
if (IRRLICHT_FOUND)
    include_directories(${IRRLICHT_INCLUDE_DIRS})
    target_link_libraries (worlds ${IRRLICHT_LIBRARIES})
else()
    message(FATAL_ERROR "Package Irrlicht not found!")
endif (IRRLICHT_FOUND)

find_package (Bullet)
if (BULLET_FOUND)
    include_directories(${BULLET_INCLUDE_DIRS})
    target_link_libraries (worlds ${BULLET_LIBRARIES})
else()
    message(FATAL_ERROR "Package Bullet not found!")
endif (BULLET_FOUND)


# TODO function (import_lib lib_name)

# endfunction(import_lib)

