#include "renderInterface.h"
#include <glm/gtc/type_ptr.hpp>

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

void renderInterface::render(std::unordered_map<int, std::vector<Callback>> cb_map)
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
            activeShader(c.first);
            for (auto& cb : c.second)
            {
                if (cb != nullptr)
                {

                    cb();
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

void renderInterface::bindingUniAttr1i(int shaderID, const std::string& name, int value)
{
    GLint location = glGetUniformLocation(shaderID, name.c_str());
    if (location != -1)
        glUniform1i(location, value);
}

void renderInterface::bindingUniAttr1f(int shaderID, const std::string& name, float value)
{
    GLint location = glGetUniformLocation(shaderID, name.c_str());
    if (location != -1)
        glUniform1f(location, value);
}

void renderInterface::bindingUniAttr4mat(int shaderID, const std::string& name,
                                         const glm::mat4& value)
{
    GLint location = glGetUniformLocation(shaderID, name.c_str());
    if (location != -1)
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void renderInterface::bindingUniAttr4f(int shaderID, const std::string& name,
                                       const glm::vec4& value)
{
    GLint location = glGetUniformLocation(shaderID, name.c_str());
    if (location != -1)
        glUniform4fv(location, 1, glm::value_ptr(value));
}
void renderInterface::drawPrimitive(int vao)
{

    glBindVertexArray(vao); // seeing as we only have a single VAO there's no need to bind it
                            // every time, but we'll do so to keep things a bit more organized
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void renderInterface::activeShader(int shaderProgamID)
{
    glUseProgram(shaderProgamID);
    bindingUniAttr4mat(shaderProgamID, "view", m_camera->getViewMatrix());
    bindingUniAttr4mat(shaderProgamID, "projection", m_camera->getProjectionMatrix());
}

int renderInterface::loadShader(const char* vertexShaderSource, const char* fragmentShaderSource)
{
    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
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
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
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
    return shaderProgram;
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
