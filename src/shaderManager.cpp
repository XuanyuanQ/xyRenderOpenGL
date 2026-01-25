#include "shaderManager.h"
#include <fstream>
#include <sstream>

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

shaderManager::shaderManager(/* args */) {}

shaderManager::~shaderManager() {}

void shaderManager::bindingUniAttr1i(int shaderID, const std::string& name, int value)
{
    GLint location = glGetUniformLocation(shaderID, name.c_str());
    if (location != -1)
        glUniform1i(location, value);
}

void shaderManager::bindingUniAttr1f(int shaderID, const std::string& name, float value)
{
    GLint location = glGetUniformLocation(shaderID, name.c_str());
    if (location != -1)
        glUniform1f(location, value);
}

void shaderManager::bindingUniAttr4mat(int shaderID, const std::string& name,
                                       const glm::mat4& value)
{
    GLint location = glGetUniformLocation(shaderID, name.c_str());
    if (location != -1)
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void shaderManager::bindingUniAttr4f(int shaderID, const std::string& name, const glm::vec4& value)
{
    GLint location = glGetUniformLocation(shaderID, name.c_str());
    if (location != -1)
        glUniform4fv(location, 1, glm::value_ptr(value));
}

void shaderManager::activeShader(int shaderProgamID)
{
    glUseProgram(shaderProgamID);
    // bindingUniAttr4mat(shaderProgamID, "view", m_camera->getViewMatrix());
    // bindingUniAttr4mat(shaderProgamID, "projection", m_camera->getProjectionMatrix());
}

std::string shaderManager::loadShaderSource(const std::string& filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        std::cerr << "Failed to open shader file: " << filePath << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int shaderManager::loadShader(const std::string& vertexShaderPath,
                              const std::string& fragmentShaderPath)
{

    auto it = m_shaderIDMap.find({vertexShaderPath, fragmentShaderPath});
    if (it != m_shaderIDMap.end())
    {

        return it->second; // 用 ->second 获取 value
    }
    // std::cout << "loadShader" << std::endl;
    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    const std::string prefix = "#version 330 core\n";
    std::string VS = prefix + loadShaderSource(vertexShaderPath);
    // std::cout << VS << std::endl;
    const char* sourceVS = VS.c_str();

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &sourceVS, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        return -1;
    }
    // fragment shader
    std::string FS = prefix + loadShaderSource(fragmentShaderPath);
    const char* sourceFS = FS.c_str();
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &sourceFS, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        return -1;
    }
    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        return -1;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    m_shaderIDMap[{vertexShaderPath, fragmentShaderPath}] = shaderProgram;
    return shaderProgram;
}