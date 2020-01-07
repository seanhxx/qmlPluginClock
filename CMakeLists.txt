cmake_minimum_required(VERSION 3.10)

project(TimeExample LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

get_filename_component(QT5_INSTALL_PREFIX $ENV{QT_DIR} DIRECTORY)
set(CMAKE_PREFIX_PATH "${QT5_INSTALL_PREFIX}")

find_package(Qt5 COMPONENTS Quick Core Qml REQUIRED)
qt5_add_resources(RESOURCES src/plugin.qrc)

set(SOURCES
    src/plugin.cpp
)

add_library(
    ${CMAKE_PROJECT_NAME}
    SHARED

    ${SOURCES}
    ${RESOURCES}

)

target_include_directories(${CMAKE_PROJECT_NAME}
    PUBLIC
        $<INSTALL_INTERFACE:interface>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)


target_compile_features(${CMAKE_PROJECT_NAME} PRIVATE cxx_std_11)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin/Debug"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin/Release"
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_BINARY_DIR}/bin/RelWithDebInfo"
    RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${CMAKE_BINARY_DIR}/bin/MinSizeRel"
)

target_link_libraries(${CMAKE_PROJECT_NAME} 
    PRIVATE
        Qt5::Quick Qt5::Qml Qt5::Core
)