cmake_minimum_required(VERSION 3.2)

set(BUILD_GTEST ON CACHE BOOL "Include googletest" FORCE)
set(BUILD_GMOCK OFF CACHE BOOL "Don't include googlemock" FORCE)

cmake_policy(PUSH)
add_subdirectory(third_party/googletest)
cmake_policy(POP)

enable_testing()

add_executable(gtestDriver
    Driver.cpp
)

target_include_directories(gtestDriver PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_options(gtestDriver PRIVATE "-std=c++14")
target_link_libraries(gtestDriver PRIVATE cppoam gtest)

message("")
message("cppoam gtest-driver")

get_target_property(Prop gtestDriver INCLUDE_DIRECTORIES)
if (NOT Prop STREQUAL "Prop-NOTFOUND")
    message(STATUS "Include directories: ${Prop}")
endif()

get_target_property(Prop gtestDriver LINK_LIBRARIES)
if (NOT Prop STREQUAL "Prop-NOTFOUND")
    message(STATUS "Link libraries: ${Prop}")
endif()

get_target_property(Prop gtestDriver COMPILE_OPTIONS)
if (NOT Prop STREQUAL "Prop-NOTFOUND")
    message(STATUS "Compile options: ${Prop}")
endif()

get_target_property(Prop gtestDriver COMPILE_DEFINITIONS)
if (NOT Prop STREQUAL "Prop-NOTFOUND")
    message(STATUS "Compile definitions: ${Prop}")
endif()
