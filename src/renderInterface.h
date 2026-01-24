#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <array>
#include "common.h"
#include "camera.h"
#include <unordered_map>

class renderInterface
{
  public:
    static renderInterface& Instance()
    {
        static renderInterface instance; // C++11 起：线程安全
        return instance;
    }

  public:
    void render(
        std::unordered_map<int, std::vector<std::function<void(std::shared_ptr<Camera>)>>> cb_map);

    bool initContex();

    int genAndBindingVAO();
    int genAndBindingVBO();
    void drawPrimitive(int vao);

    void bindingLocAttr(const std::array<glm::vec3, 3>& positions); // location类的属性绑定
    void bindingLocAttr(const std::vector<PrimitveData>& vertex_data);

  private:
    renderInterface(/* args */);
    ~renderInterface();

  private:
    GLFWwindow* m_window;
    float m_lastFrame;
    std::shared_ptr<Camera> m_camera;

    /* data */
};
