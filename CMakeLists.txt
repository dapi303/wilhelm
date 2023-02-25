cmake_minimum_required(VERSION 3.3)
project(main)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(GLOB SOURCES src/*.cpp src/*.h src/*/*.cpp src/*/*.h)
add_executable(main ${SOURCES})

find_package(SDL2 CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm CONFIG REQUIRED)
set(OpenGL_GL_PREFERENCE LEGACY)
find_package(OpenGL REQUIRED)

target_link_libraries(main
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
)

add_definitions(-DGLEW_STATIC)
target_link_libraries(main GLEW::GLEW)
target_link_libraries(main glm::glm)
target_link_libraries(main OpenGL::GL)
target_link_libraries(main -lGLU)

add_custom_target(start COMMAND cd bin && ./main)
