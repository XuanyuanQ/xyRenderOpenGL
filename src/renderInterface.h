#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <array>
#include "common.h"

// 窗口大小
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

class renderInterface
{
  public:
    static renderInterface& Instance()
    {
        static renderInterface instance; // C++11 起：线程安全
        return instance;
    }

  public:
    void render(std::vector<Callback> cbs);

    bool initContex();
    int loadShader(const char* vertexShaderSource, const char* fragmentShaderSource);
    int genAndBindingVAO();
    int genAndBindingVBO();
    void bindingLocAttr(const std::array<glm::vec3, 3>& positions); // location类的属性绑定
  private:
    renderInterface(/* args */);
    ~renderInterface();

  private:
    GLFWwindow* m_window;
    /* data */
};
