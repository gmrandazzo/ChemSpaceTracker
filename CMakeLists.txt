project(ChemSpaceTracker)
cmake_minimum_required(VERSION 3.5)
set(VERSION "1.9.0")
set(AUTHOR "Giuseppe Marco Randazzo")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC OFF)

set(_components Core Gui Widgets PrintSupport Concurrent Svg Sql WebEngine WebEngineWidgets)
foreach(_component ${_components})
  find_package(Qt6${_component})
  list(APPEND QT6_LIBRARIES ${Qt6${_component}_LIBRARIES})
endforeach()

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules/")
find_package(Libscientific)
find_package(RDKit)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build, options are: Debug Release" FORCE)
endif(NOT CMAKE_BUILD_TYPE)

string(TOLOWER "${CMAKE_BUILD_TYPE}" cmake_build_type_tolower)

# Disable some warnings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-c++11-long-long -Wno-extra-semi -Wno-c++11-extensions -Wno-extra-semi")

if(WIN32)
  message(STATUS "Build Release: WIN32")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall -DRELEASE -static -DWIN32 -Wno-long-long")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--no-undefined -static-libgcc -static-libstdc++ -Wl,-O1 -Wl,--as-needed -Wl,--sort-common -s -Wl,-O1")
  SET_PROPERTY(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS_RELEASE QT_NO_DEBUG)
  add_definitions(${QT_DEFINITIONS})
  set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
elseif(APPLE)
  if(cmake_build_type_tolower STREQUAL "debug")
    message(STATUS "Build Debug: APPLE")
    set(CMAKE_BUILD_TYPE "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -Wall -pedantic -g")
  else()
  message(STATUS "Build Release: APPLE")
    set(CMAKE_BUILD_TYPE "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DRELEASE -O2 -Wall -Wnewline-eof")
  endif()
else()
  message(STATUS "Build Release: Linux")
  if(cmake_build_type_tolower STREQUAL "debug")
    set(CMAKE_BUILD_TYPE "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -Wall -pedantic -g -Wno-long-long ")
  else()
    set(CMAKE_BUILD_TYPE "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DRELEASE -O2 -Wall -Wno-long-long ")
  endif()
endif(WIN32)

message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")

add_definitions(${QT5_DEFINITIONS})

include_directories(
  ${PROJECT_BINARY_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_BINARY_DIR} #this is where ui_SimpleViewUI.h is generated
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${LIBSCIENTIFIC_INCLUDE_DIRS}
  ${RDKIT_INCLUDE_DIRS}
)


add_subdirectory (src)
