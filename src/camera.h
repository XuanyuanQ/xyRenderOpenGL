#pragma once
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class Camera
{
  private:
    /* data */
  public:
    Camera(/* args */);
    ~Camera();
    void processInput(GLFWwindow* window, float deltaTime);
    glm::mat4 getViewMatrix()
    {
        return m_view;
    }

    glm::mat4 getProjectionMatrix()
    {
        return m_projection;
    }

  private:
    void updateView();

  private:
    glm::mat4 m_view;
    glm::mat4 m_projection;
    glm::vec3 m_cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 m_cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 m_cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    float m_fov = 45.0f;
};
