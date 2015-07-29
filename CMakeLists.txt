cmake_minimum_required(VERSION 3.1)
project(OarphKit C CXX)

### Versioning ###############################################################

set(OK_MAJOR_VERSION 0)
set(OK_MINOR_VERSION 1)
set(OK_PATCH_VERSION 0)
set(OK_VERSION
  ${OK_MAJOR_VERSION}.${OK_MINOR_VERSION}.${OK_PATCH_VERSION})



### Default Install Paths ####################################################

set(EXEC_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}"
  CACHE PATH "Base installation path for executables.")
set(INSTALL_BIN_DIR "${EXEC_INSTALL_PREFIX}/bin"
  CACHE PATH "Installation directory for binaries (default: prefix/bin).")
set(LIB_INSTALL_DIR "${EXEC_INSTALL_PREFIX}/lib"
  CACHE PATH "Installation directory for libraries (default: prefix/lib).")
set(INSTALL_INCLUDE_DIR "${EXEC_INSTALL_PREFIX}/include"
  CACHE PATH
  "Installation directory for header files (default: prefix/include).")



### Options ##################################################################

option(OK_DISABLE_LOGERR "Disable logging to stderr" OFF)
option(OK_DISABLE_ASSERTIONS_AND_CHECKS "Elide out assertions and checks" OFF)
option(OK_DISABLE_PROTOBUF "Compile without protobuf-based serialization" OFF)
option(OK_DISABLE_CAPNP "Compile without capnp-based serialization" OFF)
option(OK_DISABLE_FLI_RT "Compile without FLi Run-time Type Checks" OFF)
option(OK_DISABLE_FLI_CT "Compile without FLi Compile-time Type Checks" OFF)
option(OK_DISABLE_EXCEPTIONS "Compile without exceptions" OFF)



# Magic (deleteme?)

if(WIN32 AND NOT CYGWIN)
  set(DEF_INSTALL_CMAKE_DIR CMake)
else()
  set(DEF_INSTALL_CMAKE_DIR lib/CMake/OarphKit)
endif()
set(INSTALL_CMAKE_DIR ${DEF_INSTALL_CMAKE_DIR} CACHE PATH
  "Installation directory for CMake files")

include_directories(
  "${PROJECT_SOURCE_DIR}/oarphkit"
  "${PROJECT_BINARY_DIR}")



### Compiler Flags ###########################################################

set(oarphkit_common_flags "-Wall -std=c++11 -stdlib=libc++ -pthread")

if(OK_DISABLE_LOGERR)
  set(oarphkit_common_flags "${oarphkit_common_flags} -DOK_LOGERR_ENABLED=0")
endif()

if(OK_DISABLE_ASSERTIONS_AND_CHECKS)
  set(oarphkit_common_flags "${oarphkit_common_flags} -DOK_ASSERTIONS_AND_CHECKS=0")
endif()

if(OK_DISABLE_PROTOBUF)
  set(oarphkit_common_flags "${oarphkit_common_flags} -DOK_ENABLE_PROTOBUF=0")
endif()

if(OK_DISABLE_CAPNP)
  set(oarphkit_common_flags "${oarphkit_common_flags} -DOK_ENABLE_CAPNP=0")
endif()

if(OK_DISABLE_FLI_RT)
  set(oarphkit_common_flags "${oarphkit_common_flags} -DOK_FLI_RUNTIME_TYPECHECK_ENABLED=0")
endif()

if(OK_DISABLE_FLI_CT)
  set(oarphkit_common_flags "${oarphkit_common_flags} -DOK_FLI_COMPILETIME_TYPECHECK_ENABLED=0")
endif()

if(OK_DISABLE_EXCEPTIONS)
  set(oarphkit_common_flags "${oarphkit_common_flags} -DOK_DISABLE_EXCEPTIONS=0")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${oarphkit_common_flags}")
if(UNIX OR APPLE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} ${oarphkit_common_flags} -g3 -pg")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} ${oarphkit_common_flags} -O3")



### Dependencies #############################################################

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/Modules)

# To use local install, set GTEST_ROOT=deps/gtest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

if(NOT OK_DISABLE_PROTOBUF)
  # To use local install, set
  #  PROTOBUF_LIBRARY=deps/protobuf/lib/libprotobuf.dylib (use file path!)
  #  PROTOBUF_INCLUDE_DIR=deps/protobuf/include
  find_package(Protobuf)
  include_directories(${PROTOBUF_INCLUDE_DIRS})
endif()

if(NOT OK_DISABLE_CAPNP)
  # To use local install, set
  #  CAPNP_INCLUDE_DIRS=deps/capnproto/include
  #  CAPNP_LIB_KJ=deps/capnproto/lib/libkj.a
  #  CAPNP_LIB_CAPNP=deps/capnproto/lib/libcapnp.a
  find_package(CapnProto)
  include_directories(${CAPNP_INCLUDE_DIRS})
  add_definitions(${CAPNP_DEFINITIONS})
endif()



### Targets ##################################################################

###
### Library liboarphkit
###

# TODO: reconsider using config.h
# FMI:
#  http://www.cmake.org/Wiki/CMake/Tutorials/How_to_create_a_ProjectConfig.cmake_file
#configure_file(
#  oarphkit/ok/config.h.in
#  "${CMAKE_CURRENT_BINARY_DIR}/ok/config.h" @ONLY)

#install(
#  FILES
#  "${CMAKE_CURRENT_BINARY_DIR}/ok/config.h"
#  DESTINATION "${LIB_INSTALL_DIR}/pkgconfig")

file(
  GLOB_RECURSE oarphkit_headers
  oarphkit/ok/*.hpp
  oarphkit/ok/*.h)

file(
  GLOB_RECURSE oarphkit_srcs
  oarphkit/ok/*.hpp
  oarphkit/ok/*.cpp
  oarphkit/ok/*.h)

set(
  oarphkit_dep_libs
  pthread)

if(NOT OK_DISABLE_PROTOBUF)
  file(GLOB_RECURSE pb_headers oarphkit/ok_msg/*.pb.h)
  set(oarphkit_headers ${oarphkit_headers} ${pb_headers})
  
  file(GLOB_RECURSE pb_srcs oarphkit/ok_msg/*.pb.cc)
  set(oarphkit_srcs ${oarphkit_srcs} ${pb_srcs})
  
  set(oarphkit_dep_libs ${oarphkit_dep_libs} ${PROTOBUF_LIBRARIES}) 
endif()

if(NOT OK_DISABLE_CAPNP)
  file(GLOB_RECURSE capnp_headers oarphkit/ok_msg/*.capnp.h)
  set(oarphkit_headers ${oarphkit_headers} ${capnp_headers})
  
  file(GLOB_RECURSE capnp_srcs oarphkit/ok_msg/*.capnp.c++)
  set(oarphkit_srcs ${oarphkit_srcs} ${capnp_srcs})
  
  set(oarphkit_dep_libs ${oarphkit_dep_libs} ${CAPNP_LIBRARIES} ${CAPNP_LIB_CAPNPC})
endif()

add_library(oarphkit SHARED ${oarphkit_srcs})
add_library(oarphkitStatic STATIC ${oarphkit_srcs})
set_target_properties(oarphkitStatic PROPERTIES OUTPUT_NAME oarphkit)

target_link_libraries(
  oarphkit
  ${oarphkit_dep_libs})

install(
  TARGETS oarphkit oarphkitStatic
  ARCHIVE DESTINATION "${LIB_INSTALL_DIR}"
  LIBRARY DESTINATION "${LIB_INSTALL_DIR}")
if(NOT OK_DISABLE_PROTOBUF)
  install(FILES ${pb_headers} DESTINATION "${INSTALL_INCLUDE_DIR}/ok_msg")
  file(GLOB_RECURSE pb_protos oarphkit/ok_msg/*.proto)
  install(FILES ${pb_protos} DESTINATION "${INSTALL_INCLUDE_DIR}/ok_msg")
endif()
if(NOT OK_DISABLE_CAPNP)
  install(FILES ${capnp_headers} DESTINATION "${INSTALL_INCLUDE_DIR}/ok_msg")
  file(GLOB_RECURSE capnp_protos oarphkit/ok_msg/*.capnp)
  install(FILES ${capnp_protos} DESTINATION "${INSTALL_INCLUDE_DIR}/ok_msg")
endif()

# Install all main oarphkit headers, maintaining source tree format
install(
  DIRECTORY oarphkit/
  DESTINATION "${INSTALL_INCLUDE_DIR}"
  FILES_MATCHING PATTERN "*.hpp")

###
### Executable FLiX
###

set(flix_dep_libs ${oarphkit_dep_libs})

file(GLOB_RECURSE flix_srcs flix/*.cpp)

add_executable(flix ${flix_srcs})

target_link_libraries(
  flix
  oarphkit
  ${flix_dep_libs})

install(
  TARGETS flix
  # IMPORTANT: Add the flix executable to the "export-set"
  EXPORT OarphKitTargets
  RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin)


###
### Executable oarphkit_test
###

set(
  oarphkit_test_dep_libs
  ${oarphkit_dep_libs}
  ${GTEST_BOTH_LIBRARIES})

file(
  GLOB_RECURSE oarphkit_test_srcs
  oarphkit_test/ok/*.hpp
  oarphkit_test/ok/*.cpp
  oarphkit_test/ok_test/*.hpp
  oarphkit_test/ok_test/*.cpp)

if(NOT OK_DISABLE_PROTOBUF)
  file(GLOB_RECURSE pb_headers oarphkit_test/ok_test_msg/*.pb.h)
  set(oarphkit_test_srcs ${oarphkit_test_srcs} ${pb_headers})

  file(GLOB_RECURSE pb_srcs oarphkit_test/ok_test_msg/*.pb.cc)
  set(oarphkit_test_srcs ${oarphkit_test_srcs} ${pb_srcs})

  set(oarphkit_test_dep_libs ${oarphkit_test_dep_libs} ${PROTOBUF_LIBRARIES})
endif()

if(NOT OK_DISABLE_CAPNP)
  file(GLOB_RECURSE capnp_headers oarphkit_test/ok_test_msg/*.capnp.h)
  set(oarphkit_test_srcs ${oarphkit_test_srcs} ${capnp_headers})

  file(GLOB_RECURSE capnp_srcs oarphkit_test/ok_test_msg/*.capnp.c++)                              
  set(oarphkit_test_srcs ${oarphkit_test_srcs} ${capnp_srcs})

  set(oarphkit_test_dep_libs ${oarphkit_test_dep_libs} ${CAPNP_LIBRARIES_LITE})
endif()

add_executable(
  oarphkit_test
  ${oarphkit_test_srcs})

set_property(
  TARGET
  oarphkit_test
  APPEND PROPERTY INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/oarphkit_test")
set_property(
  TARGET
  oarphkit_test
  APPEND PROPERTY COMPILE_DEFINITIONS "GTEST_USE_OWN_TR1_TUPLE")

# TODO: consider using for coverage:
# cd cmake/Modules && \
# wget https://raw.githubusercontent.com/bilke/cmake-modules/master/CodeCoverage.cmake
#   Note the above script needs to be patched for clang (Compiler ID is not "Clang"
#   but "Clang .. version ... etc").  Trouble getting linking to work in OSX; lcov
#   reports "Out of memory!" in lcov 1.11 / ubuntu trusty.
#INCLUDE(CodeCoverage)
#set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} --coverage")
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} --coverage")
#SETUP_TARGET_FOR_COVERAGE(oarphkit_test_cov oarphkit_test coverage)

target_link_libraries(
  oarphkit_test
  oarphkitStatic
  ${oarphkit_test_dep_libs})

install(
  TARGETS oarphkit_test
  # IMPORTANT: Add the oarphkit_test executable to the "export-set"
  EXPORT OarphKitTargets
  RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin)

