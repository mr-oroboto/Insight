cmake_minimum_required(VERSION 3.5)
project(Insight)

find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)
find_package(Freetype REQUIRED)

include_directories(. ${SDL2_INCLUDE_DIR} ${GLEW_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR} ${GLM_INCLUDE_DIR} ${FREETYPE_INCLUDE_DIR} /usr/include/freetype2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp core/DisplayManager.cpp core/DisplayManager.h shader/vertex.inc shader/fragment.inc primitive/Cube.h primitive/PrimitiveCollection.h shader/ShaderCollection.h primitive/PrimitiveCollection.cpp primitive/Primitive.h primitive/Cube.cpp primitive/Primitive.cpp primitive/Triangle.cpp primitive/Triangle.h core/FrameQueue.cpp core/FrameQueue.h core/Frame.cpp core/Frame.h core/SceneObject.cpp core/SceneObject.h decorator/HeapDecorator.cpp decorator/HeapDecorator.h decorator/Decorator.cpp decorator/Decorator.h adt/HeapArray.cpp adt/HeapArray.h scenario/MinHeap.cpp scenario/MinHeap.h scenario/Scenario.cpp scenario/Scenario.h shader/vertex_text.inc shader/fragment_text.inc primitive/Line.cpp primitive/Line.h primitive/Quad.cpp primitive/Quad.h primitive/Tesselation.cpp primitive/Tesselation.h primitive/Rectangle.cpp primitive/Rectangle.h scenario/RotationTest.cpp scenario/RotationTest.h)

add_executable(Insight ${SOURCE_FILES})

target_link_libraries(Insight ${SDL2_LIBRARIES} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES} ${FREETYPE_LIBRARIES})
