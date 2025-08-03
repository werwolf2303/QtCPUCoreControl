cmake_minimum_required(VERSION 3.30)
project(QtCPUCoreControl)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
qt_standard_project_setup()

set(CMAKE_AUTOMOC on)

add_executable(QtCPUCoreControl main.cpp core.cpp core.h)

target_link_libraries(QtCPUCoreControl PRIVATE Qt6::Core Qt6::Widgets)
