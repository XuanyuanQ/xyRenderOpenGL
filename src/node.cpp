#include "node.h"
#include "renderInterface.h"
#include <array>

const char* vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "uniform mat4 view;\n"
                                 "uniform mat4 projection;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = projection*view*vec4(aPos, 1.0);\n"
                                 "}\0";
const char* fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                   "}\n\0";

Node::Node(/* args */) {}

Node::~Node() {}

primitive Node::createPrimitive()
{
    primitive content;
    Vertex triangle;
    // triangle.position = {
    //     {-0.5f, -0.5f, 0.0f}, // left
    //     {0.5f, -0.5f, 0.0f},  // right
    //     {0.0f, 0.5f, 0.0f}    // top
    // };
    std::array<glm::vec3, 3> position{{
        {-0.5f, -0.5f, 0.0f}, // left
        {0.5f, -0.5f, 0.0f},  // right
        {0.0f, 0.5f, 0.0f}    // top
    }};

    m_vao = renderInterface::Instance().genAndBindingVAO();
    m_vbo = renderInterface::Instance().genAndBindingVBO();
    renderInterface::Instance().bindingLocAttr(position);
    m_shaderProgamID =
        renderInterface::Instance().loadShader(vertexShaderSource, fragmentShaderSource);
    return content;
}

void Node::draw()
{
    renderInterface::Instance().drawPrimitive(m_vao);
}