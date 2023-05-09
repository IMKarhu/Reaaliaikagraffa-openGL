# Specify minimum supported cmake version
cmake_minimum_required(VERSION 3.5)

# set the project name
project(ReaaliaikagrafiikanOhjelmointi)

option(ASSIMP_BUILD_TESTS OFF)
option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)


# add_subdirectorys to be CMaked
add_subdirectory(ext/kgfw)
add_subdirectory(ext/glfw-3.3.2)
add_subdirectory(ext/glm-0.9.9.7)
add_subdirectory(ext/assimp-5.0.1)

# Specify include directories to include glad
include_directories("${GLFW_SOURCE_DIR}/deps")

# Specify the stb-image include directory
include_directories("ext/stb-master")

# Define glad source files
set(GLAD_GL "${GLFW_SOURCE_DIR}/deps/glad/gl.h" "${GLFW_SOURCE_DIR}/deps/glad_gl.c")

# add the executables
add_executable(simple_teapot src/main_simple_teapot.cpp src/shader.h src/shader.cpp src/gameobject.h src/gameobject.cpp src/camera.h src/texture.h src/texture.cpp src/teapot.h src/teapot.cpp src/teapot_data.h ${GLAD_GL})
add_executable(projection src/main_projection.cpp src/shader.h src/shader.cpp src/gameobject.h src/gameobject.cpp src/camera.h src/texture.h src/texture.cpp src/teapot.h src/teapot.cpp src/teapot_data.h ${GLAD_GL})
add_executable(lightning src/main_lightning.cpp src/shader.h src/shader.cpp src/gameobject.h src/gameobject.cpp src/camera.h src/texture.h src/texture.cpp src/teapot.h src/teapot.cpp src/teapot_data.h ${GLAD_GL})
add_executable( model_loading src/main_model_loading.cpp src/mesh.cpp src/mesh.h src/shader.h src/shader.cpp src/gameobject.h src/gameobject.cpp src/camera.h src/texture.h src/texture.cpp teapot.h src/teapot.cpp src/teapot_data.h ${GLAD_GL})
add_executable( practical_work src/main_model_loading.cpp src/mesh.cpp src/mesh.h src/shader.h src/shader.cpp src/gameobject.h src/gameobject.cpp src/camera.h src/texture.h src/texture.cpp src/teapot.h src/teapot.cpp src/teapot_data.h ${GLAD_GL})
# Link game_object-executables with needed libraries. Since the kgfw and glfw libraries 
# has public include directories we will use those link directories when building binaries.
target_link_libraries(simple_teapot PUBLIC kgfw PUBLIC glfw PUBLIC glm)
target_link_libraries(projection PUBLIC kgfw PUBLIC glfw PUBLIC glm)
target_link_libraries(lightning PUBLIC kgfw PUBLIC glfw PUBLIC glm)
target_link_libraries(model_loading PUBLIC kgfw PUBLIC glfw PUBLIC glm PUBLIC assimp)
target_link_libraries(practical_work PUBLIC kgfw PUBLIC glfw PUBLIC glm PUBLIC assimp)

