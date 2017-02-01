cmake_minimum_required(VERSION 2.8.4)
project(ishi_animations)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/modules)

find_package(BISON)
find_package(FLEX)

BISON_TARGET(MyParser src/bvh.y ${CMAKE_CURRENT_BINARY_DIR}/y.tab.cpp)
FLEX_TARGET(MyScanner src/bvh.l ${CMAKE_CURRENT_BINARY_DIR}/lex.yy.cpp)
ADD_FLEX_BISON_DEPENDENCY(MyScanner MyParser)

include_directories(src)

add_definitions(-DGLEW_STATIC)
include_directories(lib/glew/include)

set(SOURCE_FILES
    lib/glew/src/glew.c
    lib/glew/include/GL/glew.h
    lib/glew/include/GL/glxew.h
    lib/glew/include/GL/eglew.h
    lib/glew/include/GL/wglew.h

    src/core/bbox.cpp
    src/core/bbox.h
    src/core/color.cpp
    src/core/color.h
    src/core/common.h
    src/core/point.cpp
    src/core/point.h
    src/core/matrix.cpp
    src/core/matrix.h
    src/core/vector.cpp
    src/core/vector.h
    src/core/transform.cpp
    src/core/transform.h

    src/bvh_cb_info.cpp
    src/bvh_cb_info.h
    src/bvh_defs.h
    src/common.h
    src/geom.h
    src/joint.cpp
    src/joint.h
    src/joint_info.h
    src/loader.h
    src/mat.h
    src/types.h
    src/vec.h)

add_executable(ishi_animations
    src/main.cpp
    ${SOURCE_FILES}
    ${BISON_MyParser_OUTPUTS}
    ${FLEX_MyScanner_OUTPUTS})

# Flex/Bison
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# required libraries
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIR})

set(MAIN_LIBRARIES
    ${OPENGL_LIBRARIES}
    ${GLUT_LIBRARIES})

target_link_libraries(ishi_animations ${MAIN_LIBRARIES})

# Build test
include_directories(lib)
set(TEST_FILES
    test/matrix_test.cpp
    test/point_test.cpp
    test/point_vector_test.cpp
    test/transform_point_test.cpp
    test/transform_test.cpp
    test/transform_vector_test.cpp
    test/vector_test.cpp)

add_executable(ishi_animations_test
    test/main.cpp
    ${SOURCE_FILES}
    ${TEST_FILES})

target_link_libraries(ishi_animations_test ${MAIN_LIBRARIES})

enable_testing()
add_test(MyTest ishi_animations_test)