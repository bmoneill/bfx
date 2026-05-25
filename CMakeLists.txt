cmake_minimum_required(VERSION 3.31.6)
project("libbfx" C)
include(CTest)

option(TEST "Build tests" OFF)
option(TOOLS "Build interpreter" ON)

execute_process(
    COMMAND git rev-parse --short HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_COMMIT_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Version
add_compile_definitions("LIBBFX_VERSION=\"${GIT_COMMIT_HASH}\"")


function(Enable_Tests)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pg -fprofile-arcs -ftest-coverage -fsanitize=address -fno-omit-frame-pointer")
  set(TEST ON CACHE INTERNAL "")
  set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMakeModules)
  include(CodeCoverage)
endfunction()

function(Build_Binary)
  add_subdirectory(bfx)
  include_directories(bfx)
endfunction()

function(Build_Library)
  add_subdirectory(libbfx)
  include_directories(libbfx)
endfunction()

function(Build_Tests)
  add_subdirectory(external)
  add_subdirectory(test)
endfunction()

if(TEST)
  Enable_Tests()
endif()

Build_Library()

if(TEST)
  Build_Tests()
endif()

if(TOOLS)
  Build_Binary()
endif()
