cmake_minimum_required(VERSION 3.16)

project(PixelPuzzle)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)

add_executable(${PROJECT_NAME}
        main.cpp
        mainwindow.cpp
        mainwindow.cpp mainwindow.h picture.cpp picture.h gallery.cpp gallery.h)


target_link_libraries(${PROJECT_NAME} PUBLIC
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        )

set_target_properties(${PROJECT_NAME} PROPERTIES AUTOMOC TRUE)
