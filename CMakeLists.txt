cmake_minimum_required(VERSION 3.5)
project(Insight)

find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)

include_directories(${SDL2_INCLUDE_DIR} ${GLEW_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR} ${GLM_INCLUDE_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp DisplayManager.cpp DisplayManager.h shaders/vertex.h shaders/fragment.h primitives/Cube.h primitives/PrimitiveCollection.h shaders/ShaderCollection.h primitives/PrimitiveCollection.cpp primitives/Primitive.h primitives/Cube.cpp primitives/Primitive.cpp primitives/Triangle.cpp primitives/Triangle.h)

add_executable(Insight ${SOURCE_FILES})

target_link_libraries(Insight ${SDL2_LIBRARIES} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES})
