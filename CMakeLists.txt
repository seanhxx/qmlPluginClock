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

set(target "${CMAKE_PROJECT_NAME}Plugin")

add_library(
    ${target}
    SHARED

    ${SOURCES}
    ${RESOURCES}

)

target_include_directories(${target}
    PUBLIC
        $<INSTALL_INTERFACE:interface>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)


target_compile_features(${target} PRIVATE cxx_std_11)
target_compile_definitions(${target} PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

set_target_properties(${target} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin/Debug"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin/Release"
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_BINARY_DIR}/bin/RelWithDebInfo"
    RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${CMAKE_BINARY_DIR}/bin/MinSizeRel"
)

target_link_libraries(${target} 
    PRIVATE
        Qt5::Quick Qt5::Qml Qt5::Core
)

find_program(QmlPluginDump_EXECUTABLE NAMES qmlplugindump HINTS "${QT5_INSTALL_PREFIX}/bin")
add_custom_command(TARGET ${target} POST_BUILD
    COMMAND ${QmlPluginDump_EXECUTABLE} -noinstantiate -nonrelocatable TimeExample 1.0 "../$<TARGET_FILE_DIR:${target}>" > "$<TARGET_FILE_DIR:${target}>/plugins.qmltypes"
)

#add_custom_target(qmltypes)
#set(qmltypes_target "${target}-qmltypes")
#set(plugins_qmltypes "${CMAKE_BINARY_DIR}/bin/Debug/plugins.qmltypes")
#add_custom_target("${qmltypes_target}"
#    BYPRODUCTS "${plugins_qmltypes}"
#    COMMAND ${QmlPluginDump_EXECUTABLE} -noinstantiate -nonrelocatable ${target} 1.0 "${CMAKE_BINARY_DIR}/bin/Debug" > "${plugins_qmltypes}"
#)
#
#add_dependencies(qmltypes ${qmltypes_target})
