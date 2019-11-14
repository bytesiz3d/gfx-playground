cmake_minimum_required(VERSION 3.0)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 14)
add_compile_options(-w)

# Project Variables:
set(PROJECT_NAME "gfx-playground")
set(THIRDPARTY_DIR "deps")

# Project and Source Groups:
project(${PROJECT_NAME})
add_executable(${PROJECT_NAME} solar_system.cpp)

# OpenGL:
find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} OpenGL::GL)

# GLAD:
add_subdirectory(${THIRDPARTY_DIR}/glad)
# target_include_directories(${PROJECT_NAME} PRIVATE ${THIRDPARTY_DIR}/glad/include)
target_link_libraries(${PROJECT_NAME} glad)

# GLFW:
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS    OFF)
option(GLFW_BUILD_DOCS 	   OFF)
option(GLFW_INSTALL	       OFF)
option(GLFW_VULKAN_STATIC  OFF)

add_subdirectory(${THIRDPARTY_DIR}/glfw)
target_include_directories(${PROJECT_NAME} PRIVATE ${THIRDPARTY_DIR}/glfw/include)
target_link_libraries(${PROJECT_NAME} glfw)

# GLM:
option(GLM_TEST_ENABLE OFF)

add_subdirectory(${THIRDPARTY_DIR}/glm)
target_include_directories(${PROJECT_NAME} PRIVATE ${THIRDPARTY_DIR}/glm)
target_link_libraries(${PROJECT_NAME} glm_static)

# Options:
set(BUILD_SHARED_LIBS FALSE CACHE BOOL "Enforce static linking." FORCE)
set(BUILD_STATIC_LIBS TRUE  CACHE BOOL "Enforce static linking." FORCE)

# Visual Studio Convenience:
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

target_compile_definitions(${PROJECT_NAME} PRIVATE
 														UNICODE
 														_UNICODE
 														_CRT_SECURE_NO_WARNINGS)
