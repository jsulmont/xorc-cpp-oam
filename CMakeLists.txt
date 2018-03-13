cmake_minimum_required(VERSION 3.2)

project(cppoam LANGUAGES CXX)

message("")
message("global")
message(STATUS "System: ${CMAKE_SYSTEM_NAME}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "C++ flags: ${CMAKE_CXX_FLAGS}")
message(STATUS "C++ flags Debug: ${CMAKE_CXX_FLAGS_DEBUG}")
message(STATUS "C++ flags Release: ${CMAKE_CXX_FLAGS_RELEASE}")

message("")
message("cppoam")

add_library(cppoam
    include/cppoam/MsgPackParser.h
    include/cppoam/ProgramRef.h

    lib/MsgPackParser.cpp
    lib/OpCodes.h

    lib/BoxedValue.h
    lib/BoxedValue.cpp
    lib/Instruction.h
    lib/Instruction.cpp
    lib/Program.h
    lib/Program.cpp
    lib/ProgramRef.cpp
)

target_include_directories(cppoam PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/msgpack-c/include
)

target_compile_options(cppoam PUBLIC "-std=c++14")

get_target_property(Prop cppoam INCLUDE_DIRECTORIES)
if (NOT Prop STREQUAL "Prop-NOTFOUND")
    message(STATUS "Include directories: ${Prop}")
endif()

get_target_property(Prop cppoam LINK_LIBRARIES)
if (NOT Prop STREQUAL "Prop-NOTFOUND")
    message(STATUS "Link libraries: ${Prop}")
endif()

get_target_property(Prop cppoam COMPILE_OPTIONS)
if (NOT Prop STREQUAL "Prop-NOTFOUND")
    message(STATUS "Compile options: ${Prop}")
endif()

get_target_property(Prop cppoam COMPILE_DEFINITIONS)
if (NOT Prop STREQUAL "Prop-NOTFOUND")
    message(STATUS "Compile definitions: ${Prop}")
endif()

set(MSGPACK_CXX11 ON)
add_subdirectory(third_party/msgpack-c)

add_subdirectory(tests/oamtestkit-driver)
add_subdirectory(tests/gtest-driver)
