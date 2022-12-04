
cmake_minimum_required (VERSION 3.8)

SET(PROJ_MAIN_NAME "hss-tpv0.10")
PROJECT(${PROJ_MAIN_NAME})


# Set hss threadpool version.
# ------------------------------------------------------------------------------------------------------
set(HSS_TP_MAJOR_VERSION 0)
set(HSS_TP_MINOR_VERSION 1)
set(HSS_TP_PATCH_VERSION 0)

#set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -lpthread)
find_package(Threads REQUIRED)

# We need extra libraries on Windows
# if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
#   set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=gnu++11)
#   set(CMAKE_CXX_EXTENSIONS ON)
#   # Below line solves fileno issue at google benchmark compilation.
#   # https://github.com/google/benchmark/issues/294
#   add_definitions(-D_POSIX_SOURCE) # Solves fileno issue in google benchmark.
#   add_definitions(-D_GNU_SOURCE) #Solves strdup issue on google test.
# elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
#   set(CMAKE_C_COMPILER /bin/x86_64-linux-gnu-gcc-9)
#   set(CMAKE_CXX_COMPILER /bin/x86_64-linux-gnu-g++-9)
# endif()

# set(CMAKE_C_COMPILER /bin/x86_64-linux-gnu-gcc-9)
#   set(CMAKE_CXX_COMPILER /bin/x86_64-linux-gnu-g++-9)

# Configuration types
# ------------------------------------------------------------------------------------------------------
SET(CMAKE_CONFIGURATION_TYPES "Debug;Release;RelWithDebInfo" CACHE STRING "Configs" FORCE)
IF(DEFINED CMAKE_BUILD_TYPE AND CMAKE_VERSION VERSION_GREATER "2.8")
  SET_PROPERTY(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS ${CMAKE_CONFIGURATION_TYPES})
ENDIF()

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

SET(BUILD_SHARED_LIBS "ON" FORCE)

enable_testing()


# Configure variables
# ------------------------------------------------------------------------------------------------------
IF(DEFINED GBENCHMARK_PATH)
ELSE()
  SET(GBENCHMARK_PATH ${CMAKE_SOURCE_DIR}/thirdparties/benchmark)
ENDIF()
message(STATUS "Google benchmark path: " ${GBENCHMARK_PATH})

IF(DEFINED GOOGLETEST_PATH)
ELSE()
  SET(GOOGLETEST_PATH ${CMAKE_SOURCE_DIR}/thirdparties/googletest) #google test path var needed for benchmark.
ENDIF()
message(STATUS "Google test path: " ${GOOGLETEST_PATH})

IF(DEFINED HSSTSQUEUE_PATH)
ELSE()
  SET(HSSTSQUEUE_PATH ${CMAKE_SOURCE_DIR}/thirdparties/hss-tsqv0.10) #hss-tsqv0.10 needed for enqueue tasks.
ENDIF()
message(STATUS "hss-tsqv0.10 path: " ${HSSTSQUEUE_PATH})

IF(DEFINED HSSBMRKUTILS_PATH)
ELSE()
  SET(HSSBMRKUTILS_PATH ${CMAKE_SOURCE_DIR}/thirdparties/hss-bmrk-utils)
ENDIF()
message(STATUS "hss-bmrk-utils path: " ${HSSBMRKUTILS_PATH})


# Ouput paths.
# ------------------------------------------------------------------------------------------------------
SET(LIBRARY_OUTPUT_PATH          ${PROJECT_BINARY_DIR}/bin         CACHE PATH "Library output" FORCE)
SET(EXECUTABLE_OUTPUT_PATH       ${PROJECT_BINARY_DIR}/bin         CACHE PATH "Executable output" FORCE)

#add_compile_definitions(BENCHMARK_NO_EXPORT)

# Add Subfolders.
# ------------------------------------------------------------------------------------------------------
ADD_SUBDIRECTORY(hss-threadpool)
ADD_SUBDIRECTORY(${GOOGLETEST_PATH})
ADD_SUBDIRECTORY(${GBENCHMARK_PATH})
ADD_SUBDIRECTORY(${HSSTSQUEUE_PATH})
ADD_SUBDIRECTORY(${HSSBMRKUTILS_PATH})
