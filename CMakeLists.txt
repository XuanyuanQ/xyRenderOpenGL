cmake_minimum_required(VERSION 3.15)

project(OpenGLWindow
        VERSION 1.0
        LANGUAGES C CXX)

# =====================
# C++ 标准
# =====================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# =====================
# OpenGL
# =====================
find_package(OpenGL REQUIRED)

# =====================
# GLAD（源码方式）
# =====================
add_library(glad
    external/glad/src/glad.c
)

target_include_directories(glad
    PUBLIC external/glad/include
)


# =====================
# GLFW（源码）
# =====================
# 关闭不需要的内容（强烈推荐）
set(GLM_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLM_BUILD_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(external/glm)

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)

add_subdirectory(external/glfw-3.4)

# =====================
# 可执行程序
# =====================
file(GLOB_RECURSE SRC_FILES
    src/*.cpp
    src/*.h
)

add_executable(OpenGLWindow
    ${SRC_FILES}
)
target_include_directories(OpenGLWindow PRIVATE src)

target_link_libraries(OpenGLWindow
    PRIVATE
        glfw
        glad
        glm
        OpenGL::GL
)

# =====================
# Windows 额外系统库
# =====================
if (WIN32)
    target_link_libraries(OpenGLWindow
        PRIVATE
            user32
            gdi32
            shell32
    )

    # 如果是 DLL，构建后拷贝到可执行文件目录
    if (EXISTS ${GLFW_DLL_PATH})
        add_custom_command(TARGET OpenGLWindow POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${GLFW_DLL_PATH}
            $<TARGET_FILE_DIR:OpenGLWindow>
        )
    endif()
endif()

# 定义目标 shader 目录
set(SHADER_SRC "${CMAKE_SOURCE_DIR}/shader")
set(SHADER_DST "$<TARGET_FILE_DIR:OpenGLWindow>/shader")

# 确保创建目标目录
add_custom_command(TARGET OpenGLWindow POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${SHADER_DST}
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${SHADER_SRC} ${SHADER_DST}
)