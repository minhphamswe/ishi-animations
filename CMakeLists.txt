cmake_minimum_required(VERSION 2.8.4)
project(ishi_animations)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/modules)

find_package(BISON)
find_package(FLEX)

BISON_TARGET(MyParser src/demo/yacc/bvh.y ${CMAKE_CURRENT_BINARY_DIR}/y.tab.cpp)
FLEX_TARGET(MyScanner src/demo/lex/bvh.l ${CMAKE_CURRENT_BINARY_DIR}/lex.yy.cpp)
ADD_FLEX_BISON_DEPENDENCY(MyScanner MyParser)

include_directories(src/main/cpp src/demo/cpp)

add_definitions(-DGLEW_STATIC)
include_directories(lib/glew/include)

set(SOURCE_FILES
    lib/glew/src/glew.c
    lib/glew/include/GL/glew.h
    lib/glew/include/GL/glxew.h
    lib/glew/include/GL/eglew.h
    lib/glew/include/GL/wglew.h

    src/main/cpp/core/math.cpp
    src/main/cpp/core/math.h

    src/main/cpp/core/bbox.cpp
    src/main/cpp/core/bbox.h
    src/main/cpp/core/color.cpp
    src/main/cpp/core/color.h
    src/main/cpp/core/common.h
    src/main/cpp/core/point.cpp
    src/main/cpp/core/point.h
    src/main/cpp/core/matrix.cpp
    src/main/cpp/core/matrix.h
    src/main/cpp/core/vector.cpp
    src/main/cpp/core/vector.h
    src/main/cpp/core/transform.cpp
    src/main/cpp/core/transform.h

    src/demo/cpp/bvh_cb_info.cpp
    src/demo/cpp/bvh_cb_info.h
    src/demo/cpp/bvh_defs.h
    src/demo/cpp/common.h
    src/demo/cpp/geom.h
    src/demo/cpp/joint.cpp
    src/demo/cpp/joint.h
    src/demo/cpp/joint_info.h
    src/demo/cpp/loader.h
    src/demo/cpp/mat.h
    src/demo/cpp/types.h
    src/demo/cpp/vec.h)

add_executable(ishi_animations
    src/demo/cpp/main.cpp
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

    src/test/cpp/core/math_test.cpp
    src/test/cpp/core/matrix_test.cpp
    src/test/cpp/core/point_test.cpp
    src/test/cpp/core/point_vector_test.cpp
    src/test/cpp/core/transform_point_test.cpp
    src/test/cpp/core/transform_test.cpp
    src/test/cpp/core/transform_vector_test.cpp
    src/test/cpp/core/vector_test.cpp)

add_executable(ishi_animations_test
    src/test/cpp/main.cpp
    ${SOURCE_FILES}
    ${TEST_FILES})

target_link_libraries(ishi_animations_test ${MAIN_LIBRARIES})

enable_testing()
add_test(MyTest ishi_animations_test)