cmake_minimum_required(VERSION 3.27.1)

project(vc-CHIP-8)

add_executable(${PROJECT_NAME}
	src/glad.c
	src/main.c
	src/program.c
	src/wm.c)

# CGLM
add_subdirectory(lib/cglm)
target_link_libraries(${PROJECT_NAME} PRIVATE cglm)

# GLFW
find_package(OpenGL REQUIRED)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(lib/glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
