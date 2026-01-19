#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <array>
#include "common.h"
#include "camera.h"

class renderInterface
{
  public:
    static renderInterface& Instance()
    {
        static renderInterface instance; // C++11 起：线程安全
        return instance;
    }

  public:
    void render(std::unordered_map<int, std::vector<Callback>> cb_map);

    bool initContex();
    int loadShader(const char* vertexShaderSource, const char* fragmentShaderSource);
    int genAndBindingVAO();
    int genAndBindingVBO();
    void drawPrimitive(int vao);
    void activeShader(int shaderProgamID);
    void bindingLocAttr(const std::array<glm::vec3, 3>& positions); // location类的属性绑定

    void bindingUniAttr1i(int shaderID, const std::string& name, int value);
    void bindingUniAttr1f(int shaderID, const std::string& name, float value);
    void bindingUniAttr4mat(int shaderID, const std::string& name, const glm::mat4& value);
    void bindingUniAttr4f(int shaderID, const std::string& name, const glm::vec4& value);

  private:
    renderInterface(/* args */);
    ~renderInterface();

  private:
    GLFWwindow* m_window;
    float m_lastFrame;
    std::shared_ptr<Camera> m_camera;

    /* data */
};
