#include "node.h"
#include "renderInterface.h"
#include <shaderManager.h>
#include <array>

Node::Node(/* args */)
{
    m_transform = std::make_shared<transform>();
}

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
        shaderManager::Instance().loadShader("shader/defualt.vs", "shader/defualt.fs");
    return content;
}

void Node::draw()
{
    renderInterface::Instance().drawPrimitive(m_vao);
}

void Node::setParent(Node* parent)
{
    if (m_parent == parent)
    {
        return;
    }
    if (nullptr != m_parent)
    {
        auto& children = m_parent->getChildren();
        children.erase(std::remove(children.begin(), children.end(), this), children.end());
    }
    m_parent = parent;
    if (nullptr != m_parent)
    {
        parent->getChildren().push_back(this);
    }
}
void Node::addChildren(std::vector<Node*>& children)
{
    for (auto& child : children)
    {
        if (child)
        {
            child->setParent(this);
        }
    }
}
