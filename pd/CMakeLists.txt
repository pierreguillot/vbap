cmake_minimum_required(VERSION 2.8)

set(CMAKE_SUPPRESS_REGENERATION true)
set(CMAKE_MACOSX_RPATH Off)
set(CMAKE_OSX_DEPLOYMENT_TARGET 10.4)
set(CMAKE_OSX_ARCHITECTURES "i386;x86_64")

include(pd.build/pd.cmake)

project(pd.vbap)
set_pd_sources(${PROJECT_SOURCE_DIR}/pd.src/src/)
set_pd_external_path("${PROJECT_SOURCE_DIR}/ext/")

if(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W4")
elseif(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pedantic")
elseif(CMAKE_C_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Weverything -Wno-padded -Wno-reserved-id-macro -Wno-disabled-macro-expansion")
endif()

include_directories(${PROJECT_SOURCE_DIR}/../src/)
file(GLOB pd_vbap_tilde_sources ${PROJECT_SOURCE_DIR}/src/vbap_tilde.c ${PROJECT_SOURCE_DIR}/../src/vbap.c ${PROJECT_SOURCE_DIR}/../src/vbap.h)
add_pd_external(pd_vbap_tilde_project vbap~ "${pd_vbap_tilde_sources}")

file(GLOB pd_vbap_coeff_sources ${PROJECT_SOURCE_DIR}/src/vbap_coeff.c ${PROJECT_SOURCE_DIR}/../src/vbap.c ${PROJECT_SOURCE_DIR}/../src/vbap.h)
add_pd_external(pd_vbap_coeff_project vbap.coeff "${pd_vbap_coeff_sources}")
