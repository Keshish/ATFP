cmake_minimum_required(VERSION 3.26)
project(opengl_setup)

set(CMAKE_CXX_STANDARD 23)

option(BUILD_TESTS "" ON)
if (BUILD_TESTS)
    message(STATUS "Building tests")
    add_subdirectory(tests)
endif()

find_package(OpenGL REQUIRED)
find_package(GLM REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenCV REQUIRED)


include_directories(
        include
)

FILE(GLOB SRC_FILES src/*.cpp)
add_executable(opengl_setup ${SRC_FILES})
target_link_libraries(opengl_setup PRIVATE
        ${OPENGL_LIBRARIES}
        ${OpenCV_LIBS}
        GLEW::GLEW
        glm
        glfw
        ws2_32
)

