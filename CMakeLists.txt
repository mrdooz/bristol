cmake_minimum_required (VERSION 2.6)

project (bristol)

set(LIBRARY_NAME_MAIN bristol_main)
set(LIBRARY_NAME_DX bristol_dx)
set(LIBRARY_NAME_SFML bristol_sfml)

include(FindProtobuf)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
if (MSVC)
  set(SFML_STATIC_LIBRARIES TRUE)
endif()
find_package(SFML 2 REQUIRED system window graphics)
find_package(Boost REQUIRED)
find_package(Protobuf REQUIRED)

include_directories(${Boost_INCLUDE_DIR})
include_directories(${SFML_INCLUDE_DIR})
include_directories(${PROTOBUF_INCLUDE_DIR})

set(SRC_MAIN
  bristol/error.cpp bristol/file_utils.cpp bristol/string_utils.cpp bristol/utils.cpp bristol/file_watcher.cpp
  bristol/bristol.hpp bristol/error.hpp bristol/file_utils.hpp bristol/flags.hpp bristol/string_utils.hpp bristol/utils.hpp bristol/file_watcher.hpp)

set(SRC_DX
  bristol/dx/graphics_utils.cpp
  bristol/dx/graphics_utils.hpp bristol/dx/vertex_types.hpp bristol/dx/DirectXTK/SimpleMath.h bristol/dx/DirectXTK/SimpleMath.inl)
  
set(SRC_SFML
  bristol/sfml/sfml_helpers.cpp bristol/sfml/sfml_math_utils.cpp bristol/sfml/standard_cursor.cpp
  bristol/sfml/virtual_window.cpp bristol/sfml/virtual_window_manager.cpp bristol/sfml/window_event_manager.cpp
  bristol/sfml/sfml_helpers.hpp bristol/sfml/sfml_math_utils.hpp bristol/sfml/standard_cursor.hpp
  bristol/sfml/virtual_window.hpp bristol/sfml/virtual_window_manager.hpp bristol/sfml/window_event_manager.hpp
)

add_library(${LIBRARY_NAME_MAIN} STATIC ${SRC_MAIN})
add_library(${LIBRARY_NAME_DX} STATIC ${SRC_DX})
add_library(${LIBRARY_NAME_SFML} STATIC ${SRC_SFML})

if (APPLE)
  # change c++ standard library to libc++ (llvm)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -v -std=c++11 -stdlib=libc++")
  set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
elseif (MSVC)
  # Force static runtime libraries
  foreach(flag CMAKE_CXX_FLAGS_RELEASE CMAKE_CXX_FLAGS_RELWITHDEBINFO CMAKE_CXX_FLAGS_DEBUG)
    STRING(REPLACE "/MD"  "/MT" "${flag}" "${${flag}}")
    SET("${flag}" "${${flag}}")
  endforeach()
endif()
