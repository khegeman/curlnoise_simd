PROJECT(CurlNoise)

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)


find_package(GLUT)

find_package(OpenGL)

add_definitions(-DGLUT_NO_LIB_PRAGMA)
 
ADD_DEFINITIONS(
  -std=c++11
  -ffast-math
   -Wall
  -march=native
)

add_library(original original/example_2dwake.cpp original/noise.cpp)
add_library(modified example_2dwake.cpp noise.cpp)

if (GLUT_FOUND)
   include_directories(${GLUT_INCLUDE_DIR})
   add_executable(example_wake gluvi.cpp main2.cpp)


   target_link_libraries(example_wake ${GLUT_LIBRARY} ${OPENGL_LIBRARY} modified)
endif (GLUT_FOUND)

add_subdirectory(experiments)

enable_testing()

add_executable(wake_nogui main.cpp)
target_link_libraries(wake_nogui modified)

add_executable(original_wake_nogui original/main.cpp)
target_link_libraries(original_wake_nogui original)
set_target_properties(original_wake_nogui PROPERTIES COMPILE_DEFINITIONS RUN_ORIGINAL)

