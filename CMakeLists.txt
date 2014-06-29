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
  error.cpp file_utils.cpp string_utils.cpp utils.cpp
  bristol.hpp error.hpp file_utils.hpp flags.hpp string_utils.hpp utils.hpp)

set(SRC_DX
  graphics_utils.cpp
  graphics_utils.hpp vertex_types.hpp DirectXTK/SimpleMath.h DirectXTK/SimpleMath.inl)
  
set(SRC_SFML
  sfml/sfml_helpers.cpp sfml/sfml_math_utils.cpp sfml/standard_cursor.cpp
  sfml/virtual_window.cpp sfml/virtual_window_manager.cpp sfml/window_event_manager.cpp
  sfml/sfml_helpers.hpp sfml/sfml_math_utils.hpp sfml/standard_cursor.hpp
  sfml/virtual_window.hpp sfml/virtual_window_manager.hpp sfml/window_event_manager.hpp
)


add_library(${LIBRARY_NAME_MAIN} STATIC ${SRC_MAIN})
install(TARGETS ${LIBRARY_NAME_MAIN} LIBRARY DESTINATION lib COMPONENT main)

add_library(${LIBRARY_NAME_DX} STATIC ${SRC_DX})
install(TARGETS ${LIBRARY_NAME_DX} LIBRARY DESTINATION lib COMPONENT dx)

add_library(${LIBRARY_NAME_SFML} STATIC ${SRC_SFML})
install(TARGETS ${LIBRARY_NAME_SFML} LIBRARY DESTINATION lib COMPONENT sfml)

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
