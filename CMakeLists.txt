cmake_minimum_required(VERSION 3.16)

project(FabricsMapping)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_RUNTIME_OUTPUT ${CMAKE_BINARY_DIR})
set(CONAN_DISABLE_CHECK_COMPILER TRUE)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
	set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

include(${CMAKE_BINARY_DIR}/conan_paths.cmake)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_modules ${CMAKE_MODULE_PATH})

include_directories(include)

set(SRCS src/main.cpp
	src/window.cpp
	src/node.cpp
	src/grid.cpp
	src/edge.cpp
	src/radiobuttonmotif.cpp
	src/pixmap.cpp
	src/mappingdialog.cpp
	src/surfacebezier.cpp
	src/savetemplatedialog.cpp
	src/mappingmath.cpp)

set(HEADERS include/window.hpp
	include/node.hpp
	include/grid.hpp
	include/edge.hpp
	include/radiobuttonmotif.hpp
	include/pixmap.hpp
	include/mappingdialog.hpp
	include/surfacebezier.hpp
	include/savetemplatedialog.hpp
	include/mappingmath.hpp)

add_executable(${PROJECT_NAME} ${SRCS} ${HEADERS})

find_package(Qt5 COMPONENTS Widgets REQUIRED)
target_link_libraries(${PROJECT_NAME} Qt5::Widgets Qt5::Core)
