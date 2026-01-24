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

void Node::createPrimitive(primitiveType type)
{
    switch (type)
    {
    case Triangle:
        createTriangle();
        break;

    default:
        break;
    }
}

void Node::createTriangle(float width, float height)
{
    int vertices_nb = 3;
    auto vertices = std::vector<glm::vec3>(vertices_nb);
    auto normals = std::vector<glm::vec3>(vertices_nb);
    auto texcoords = std::vector<glm::vec2>(vertices_nb);
    auto tangents = std::vector<glm::vec3>(vertices_nb);
    auto binormals = std::vector<glm::vec3>(vertices_nb);

    vertices = {
        glm::vec3(-0.5f * width, -0.5f * height, 0.0f), // left
        glm::vec3(0.5f * width, -0.5f * height, 0.0f),  // right
        glm::vec3(0.0f, 0.5f * height, 0.0f)            // top
    };

    normals = {glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f),
               glm::vec3(0.0f, 0.0f, 1.0f)};
    tangents = {glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f),
                glm::vec3(1.0f, 0.0f, 0.0f)};

    binormals = {glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
                 glm::vec3(0.0f, 1.0f, 0.0f)};
    texcoords = {
        glm::vec2(0.0f, 0.0f), // left
        glm::vec2(1.0f, 0.0f), // right
        glm::vec2(0.0f, 1.0f), // top
    };

    size_t offset = 0;

    auto vertices_offset = offset;
    auto vertices_size = vertices.size() * sizeof(glm::vec3);
    offset += vertices_size;

    auto normals_offset = offset;
    auto normals_size = normals.size() * sizeof(glm::vec3);
    offset += normals_size;

    auto texcoords_offset = offset;
    auto texcoords_size = texcoords.size() * sizeof(glm::vec2);
    offset += texcoords_size;

    auto tangents_offset = offset;
    auto tangents_size = tangents.size() * sizeof(glm::vec3);
    offset += tangents_size;

    auto binormals_offset = offset;
    auto binormals_size = binormals.size() * sizeof(glm::vec3);
    offset += binormals_size;

    auto bo_size = offset;
    std::vector<PrimitveData> allVertex;
    PrimitveData wPos;
    wPos.data = vertices.data();
    wPos.dataSize = vertices_size;
    wPos.componentCount = 3;
    wPos.offset = vertices_offset;
    wPos.locationID = 0;
    allVertex.push_back(wPos);

    PrimitveData wNormal;
    wNormal.data = normals.data();
    wNormal.dataSize = normals_size;
    wNormal.componentCount = 3;
    wNormal.offset = normals_offset;
    wNormal.locationID = 1;
    allVertex.push_back(wNormal);

    PrimitveData wTangent;
    wTangent.data = tangents.data();
    wTangent.dataSize = tangents_size;
    wTangent.componentCount = 3;
    wTangent.offset = tangents_offset;
    wTangent.locationID = 2;
    allVertex.push_back(wTangent);

    PrimitveData wBinormal;
    wBinormal.data = binormals.data();
    wBinormal.dataSize = binormals_size;
    wBinormal.componentCount = sizeof(glm::vec3);
    wBinormal.offset = binormals_offset;
    wBinormal.locationID = 3;
    allVertex.push_back(wBinormal);

    PrimitveData wTexCood;
    wTexCood.data = texcoords.data();
    wTexCood.dataSize = texcoords_size;
    wTexCood.componentCount = 2;
    wTexCood.offset = texcoords_offset;
    wTexCood.locationID = 4;
    allVertex.push_back(wTexCood);

    m_vao = renderInterface::Instance().genAndBindingVAO();
    m_vbo = renderInterface::Instance().genAndBindingVBO();

    renderInterface::Instance().bindingLocAttr(allVertex);
    m_shaderProgamID =
        shaderManager::Instance().loadShader("shader/defualt.vs", "shader/defualt.fs");
}
void Node::createRectangle(float width, float height) {}
void Node::createCircel(float width, float height) {}
void Node::createSphere(float width, float height) {}
void Node::createCube(float width, float height, float depth) {}