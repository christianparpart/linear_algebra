cmake_minimum_required(VERSION 3.10)

project(linear_algebra VERSION "0.0.0" LANGUAGES CXX)

set(MASTER_PROJECT OFF)
if(${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
    set(MASTER_PROJECT ON)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ${MASTER_PROJECT})

option(LINEAR_ALGEBRA_TESTING "linear_algebra: enables building of unit tests [default: ${MASTER_PROJECT}" ${MASTER_PROJECT})
option(LINEAR_ALGEBRA_EMBEDDED_FMTLIB "linear_algebra: uses embedded fmtlib [default: ${MASTER_PROJECT}" ${MASTER_PROJECT})
option(LINEAR_ALGEBRA_EMBEDDED_CATCH2 "linear_algebra: uses embedded catch2 for testing [default: ${MASTER_PROJECT}]" ${MASTER_PROJECT})

# setting defaults
if (NOT("${CMAKE_CXX_STANDARD}"))
    set(CMAKE_CXX_STANDARD 17)
endif()
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU") OR ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang"))
    add_compile_options(-Wall)
    add_compile_options(-Wextra)
elseif(DEFINED MSVC)
    add_definitions(-DNOMINMAX)
endif()

option(LINEAR_ALGEBRA_ENABLE_TIDY "Enable clang-tidy [default: OFF]" OFF)
if(LINEAR_ALGEBRA_ENABLE_TIDY)
    find_program(CLANG_TIDY_EXE
        NAMES clang-tidy-10 clang-tidy-9 clang-tidy-8 clang-tidy-7 clang-tidy
        DOC "Path to clang-tidy executable")
    if(NOT CLANG_TIDY_EXE)
        message(STATUS "clang-tidy not found.")
    else()
        message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")
        set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
    endif()
endif()

# ----------------------------------------------------------------------------
# dependencies

if(LINEAR_ALGEBRA_EMBEDDED_FMTLIB)
    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/fmt" EXCLUDE_FROM_ALL)
    add_definitions(-DFMT_USE_WINDOWS_H=0)
else()
    # master project must provide its own fmtlib
endif()

if(LINEAR_ALGEBRA_TESTING AND LINEAR_ALGEBRA_EMBEDDED_CATCH2)
    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/catch2")
else()
    # master project must provide its own catch2
endif()

# ----------------------------------------------------------------------------
# LIBRARY

set(LINEAR_ALGEBRA_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/include/math/linear_algebra
	${CMAKE_CURRENT_SOURCE_DIR}/include/math/linear_algebra.h
)

add_library(linear_algebra INTERFACE)
target_sources(linear_algebra INTERFACE ${LINEAR_ALGEBRA_SRCS})

target_include_directories(linear_algebra INTERFACE
    $<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/include>)
target_compile_features(linear_algebra INTERFACE cxx_std_17)

# ----------------------------------------------------------------------------
# TEST

if(LINEAR_ALGEBRA_TESTING)
    enable_testing()
    add_executable(test_linear_algebra
        test/test_main.cpp
        test/linear_algebra_test.cpp
    )
    target_link_libraries(test_linear_algebra linear_algebra fmt::fmt-header-only Catch2::Catch2)
    add_test(test_linear_algebra ./test_linear_algebra)
endif()
