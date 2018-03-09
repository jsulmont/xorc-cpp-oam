cmake_minimum_required(VERSION 3.2)

message("")
message("cpp-oam Testkit driver")

add_executable(oamTestkitDriver
    Driver.cpp
)

target_include_directories(oamTestkitDriver PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/msgpack-c/include
)

target_compile_options(oamTestkitDriver PRIVATE "-std=c++14")
target_link_libraries(oamTestkitDriver PRIVATE cppoam)

get_target_property(Prop oamTestkitDriver INCLUDE_DIRECTORIES)
if (NOT Prop STREQUAL "Prop-NOTFOUND")
    message(STATUS "Include directories: ${Prop}")
endif()

get_target_property(Prop oamTestkitDriver LINK_LIBRARIES)
if (NOT Prop STREQUAL "Prop-NOTFOUND")
    message(STATUS "Link libraries: ${Prop}")
endif()

get_target_property(Prop oamTestkitDriver COMPILE_OPTIONS)
if (NOT Prop STREQUAL "Prop-NOTFOUND")
    message(STATUS "Compile options: ${Prop}")
endif()

get_target_property(Prop oamTestkitDriver COMPILE_DEFINITIONS)
if (NOT Prop STREQUAL "Prop-NOTFOUND")
    message(STATUS "Compile definitions: ${Prop}")
endif()
