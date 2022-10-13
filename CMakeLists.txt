cmake_minimum_required(VERSION 3.3)
project(app)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(SDL2 REQUIRED)
find_package(glm REQUIRED)
find_package(SDL2_image REQUIRED)

file(GLOB SOURCES src/*.cpp src/*.h src/*/*.cpp src/*/*.h)
add_executable(app ${SOURCES})

include_directories(app ${SDL2_INCLUDE_DIR}/..)
include_directories(app ${SDL2IMAGE_INCLUDE_DIR}/..)

add_definitions(-DGLEW_STATIC)
target_link_libraries(app glm::glm)
target_link_libraries(app -lmingw32)
target_link_libraries(app -lglu32)
target_link_libraries(app -lglew32)
target_link_libraries(app -lopengl32)
target_link_libraries(app ${SDL2_LIBRARIES})
target_link_libraries(app SDL2_image::SDL2_image)
target_link_libraries(app -static-libstdc++ -static-libgcc)

set(SDL2_SHARED_LIBRARY ${SDL2_BINDIR}/*.dll)

add_custom_target(copylib ALL
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SDL2_SHARED_LIBRARY} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/
    DEPENDS ${SDL2_SHARED_LIBRARY}
    COMMENT "Copying SDL2 shared library to bin directory")

add_custom_target(start COMMAND cd bin && ./app.exe)
