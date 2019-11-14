cmake_minimum_required(VERSION 3.0)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 14)
add_compile_options(-w)

# Project Variables:
set(PROJECT_NAME "gfx-playground")
set(THIRDPARTY_DIR "deps")

# Project and Source Groups:
project(${PROJECT_NAME})

# OpenGL:
find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} OpenGL::GL)

# GLAD:
add_subdirectory(${THIRDPARTY_DIR}/glad)
target_include_directories(${PROJECT_NAME} PRIVATE ${THIRDPARTY_DIR}/glad/include)
target_link_libraries(${PROJECT_NAME} glad)

# GLFW:
add_subdirectory(${THIRDPARTY_DIR}/glfw)
target_include_directories(${PROJECT_NAME} PRIVATE ${THIRDPARTY_DIR}/glfw/include)
target_link_libraries(${PROJECT_NAME} glfw)

# GLM:
add_subdirectory(${THIRDPARTY_DIR}/glm)
target_include_directories(${PROJECT_NAME} PRIVATE ${THIRDPARTY_DIR}/glm/include)
target_link_libraries(${PROJECT_NAME} glm)

# Visual Studio Convenience:
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

target_compile_definitions(${PROJECT_NAME} PRIVATE
 														UNICODE
 														_UNICODE
 														_CRT_SECURE_NO_WARNINGS)
