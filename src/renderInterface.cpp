#include "renderInterface.h"
#include <glm/gtc/type_ptr.hpp>

#include <shaderManager.h>

renderInterface::renderInterface(/* args */)
{
    m_camera = std::make_shared<Camera>();
}

renderInterface::~renderInterface() {}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

bool renderInterface::initContex()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    m_window = window;
}

void renderInterface::render(
    std::unordered_map<int, std::vector<std::function<void(std::shared_ptr<Camera>)>>> cb_map)
{
    // -----------
    while (!glfwWindowShouldClose(m_window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        float deltaTime = currentFrame - m_lastFrame;
        m_lastFrame = currentFrame;
        m_camera->processInput(m_window, deltaTime);
        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        for (auto& c : cb_map) // 同一个shaderID 只调用一次
        {
            shaderManager::Instance().activeShader(c.first);
            for (auto& cb : c.second)
            {
                if (cb != nullptr)
                {

                    cb(m_camera); // draw primitive
                }
            }
        }
        // glBindVertexArray(0); // no need to unbind it every time

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
}

void renderInterface::drawPrimitive(int vao)
{

    glBindVertexArray(vao); // seeing as we only have a single VAO there's no need to bind it
                            // every time, but we'll do so to keep things a bit more organized
    glDrawArrays(GL_TRIANGLES, 0, 3);
}
int renderInterface::genAndBindingVAO()
{
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    return VAO;
}
int renderInterface::genAndBindingVBO()
{
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    return VBO;
}
void renderInterface::bindingLocAttr(const std::array<glm::vec3, 3>& positions)
{
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions),
                 positions.data(), // ✅ 指向连续内存
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, // location
                          3, // vec3
                          GL_FLOAT, GL_FALSE,
                          sizeof(glm::vec3), // ✅ 正确 stride
                          (void*)0);

    glEnableVertexAttribArray(0);
}

void renderInterface::bindingLocAttr(const std::vector<PrimitveData>& vertex_data)
{
    int totalSize = 0;
    for (auto const& data : vertex_data)
    {
        totalSize += data.dataSize;
    }
    if (totalSize == 0)
    {
        return;
    }

    glBufferData(GL_ARRAY_BUFFER, totalSize, nullptr, GL_STATIC_DRAW);
    for (auto const& data : vertex_data)
    {
        glBufferSubData(GL_ARRAY_BUFFER, data.offset, data.dataSize, data.data);
        glVertexAttribPointer(data.locationID, data.componentCount, GL_FLOAT, GL_FALSE, 0,
                              (void*)data.offset);
        glEnableVertexAttribArray(data.locationID);
    }
}