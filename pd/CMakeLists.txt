cmake_minimum_required(VERSION 2.8)

set(CMAKE_SUPPRESS_REGENERATION true)
set(CMAKE_MACOSX_RPATH Off)
set(CMAKE_OSX_DEPLOYMENT_TARGET 10.4)
set(CMAKE_OSX_ARCHITECTURES "i386;x86_64")

include(pd.build/pd.cmake)

project(pd.vbap)
set_pd_sources(${PROJECT_SOURCE_DIR}/pd.src/src/)
set_pd_external_path("${PROJECT_SOURCE_DIR}/ext/")

file(GLOB vbap_tilde_sources ${PROJECT_SOURCE_DIR}/src/vbap_tilde.c ${PROJECT_SOURCE_DIR}/../src/vbap.c ${PROJECT_SOURCE_DIR}/../src/vbap.h)
add_pd_external(vbap_tilde_project vbap~ "${vbap_tilde_sources}")

file(GLOB vbap_coeff_sources ${PROJECT_SOURCE_DIR}/src/vbap_coeff.c ${PROJECT_SOURCE_DIR}/../src/vbap.c ${PROJECT_SOURCE_DIR}/../src/vbap.h)
add_pd_external(vbap_coeff_project vbap.coeff "${vbap_coeff_sources}")
