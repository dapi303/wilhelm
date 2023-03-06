cmake_minimum_required(VERSION 3.3)
project(main)
set(CMAKE_EXPORT_COMPILE_COMMANDS True)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(GLOB SOURCES src/*.cpp include/*.h src/*/*.cpp include/*/*.h)
add_executable(main ${SOURCES})


find_package(GLEW REQUIRED)
find_package(glm CONFIG REQUIRED)
set(OpenGL_GL_PREFERENCE LEGACY)
find_package(OpenGL REQUIRED)
find_package(Freetype REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

target_include_directories(main PRIVATE "include")

add_definitions(-DGLEW_STATIC)
target_link_libraries(main glfw)
target_link_libraries(main GLEW::GLEW)
target_link_libraries(main glm::glm)
target_link_libraries(main OpenGL::GL)
target_link_libraries(main -lGLU)
target_link_libraries(main Freetype::Freetype)
target_link_libraries(main glad::glad)
target_link_libraries(main -static-libstdc++ -static-libgcc)

file(COPY media DESTINATION bin)
file(COPY shaders DESTINATION bin)
add_custom_target(start COMMAND cd bin && ./main)
