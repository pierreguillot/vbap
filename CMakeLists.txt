cmake_minimum_required(VERSION 2.8)

option(GCOV_SUPPORT "Build with coveralls")
set(CMAKE_SUPPRESS_REGENERATION true)
set(CMAKE_MACOSX_RPATH Off)
set(CMAKE_OSX_DEPLOYMENT_TARGET 10.4)
set(CMAKE_OSX_ARCHITECTURES "i386;x86_64")
ENABLE_TESTING()

project(vbap)

if(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W4")
elseif(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pedantic")
elseif(CMAKE_C_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Weverything")
endif()

if(${GCOV_SUPPORT} STREQUAL "On")
    message(STATUS "Build with coveralls")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage")
endif()

include_directories(${PROJECT_SOURCE_DIR}/src/)
file(GLOB vbap_sources ${PROJECT_SOURCE_DIR}/src/vbap.c ${PROJECT_SOURCE_DIR}/src/vbap.h)
source_group(src FILES ${vbap_sources})

add_library(vbap_dynamic SHARED ${vbap_sources})
add_library(vbap_static STATIC ${vbap_sources})
set_target_properties(vbap_dynamic PROPERTIES OUTPUT_NAME vbap)
set_target_properties(vbap_static PROPERTIES OUTPUT_NAME vbap)

find_library(MATH_LIB m)

add_executable(vbap_test_dynamic ${PROJECT_SOURCE_DIR}/tests/test_2d.c)
target_link_libraries(vbap_test_dynamic vbap_dynamic)
target_link_libraries(vbap_test_dynamic ${MATH_LIB})
add_test(vbap_test_d vbap_test_dynamic)

add_executable(vbap_test_static ${PROJECT_SOURCE_DIR}/tests/test_2d.c)
target_link_libraries(vbap_test_static vbap_static)
target_link_libraries(vbap_test_static ${MATH_LIB})
add_test(vbap_test_s vbap_test_static)

add_executable(vbap_test_compiled ${vbap_sources} ${PROJECT_SOURCE_DIR}/tests/test_2d.c)
target_link_libraries(vbap_test_compiled vbap_static)
target_link_libraries(vbap_test_compiled ${MATH_LIB})
add_test(vbap_test_c vbap_test_compiled)
