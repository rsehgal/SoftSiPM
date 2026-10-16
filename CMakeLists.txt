cmake_minimum_required(VERSION 3.12)
project(SoftSiPM_UT VERSION 1.0 LANGUAGES CXX)

# Set the C++ standard (change to 20 if your project requires C++20)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Handle the src/ directory flexibly
file(GLOB_RECURSE SRC_FILES src/*.cpp)

if(SRC_FILES)
    # If .cpp files exist in src/, compile them into a static library
    add_library(SoftSiPM_Core ${SRC_FILES})
    target_include_directories(SoftSiPM_Core PUBLIC include)
else()
    # If src/ is currently empty, use an INTERFACE library for headers only
    add_library(SoftSiPM_Core INTERFACE)
    target_include_directories(SoftSiPM_Core INTERFACE include)
endif()

option(ROOT "Enable ROOT" ON)

if(ROOT)
add_definitions(-DUSE_ROOT)
find_package(ROOT REQUIRED)
include_directories(AFTER SYSTEM ${ROOT_INCLUDE_DIRS})
set(EXTERNAL_LIBS ${EXTERNAL_LIBS}
${ROOT_LIBRARIES}
)
endif(ROOT)


# 2. Automatically find and build all programs in the example directory
file(GLOB EXAMPLE_SOURCES examples/*.cpp)

foreach(EXAMPLE_SRC ${EXAMPLE_SOURCES})
    # Extract the filename without extension to use as the executable name
    get_filename_component(EXE_NAME ${EXAMPLE_SRC} NAME_WE)
    
    # Create an executable for each file found in the example directory
    add_executable(${EXE_NAME} ${EXAMPLE_SRC})
    
    # Link the core library (provides include paths and compiled sources)
    target_link_libraries(${EXE_NAME} PRIVATE SoftSiPM_Core ${EXTERNAL_LIBS}) 
endforeach()
