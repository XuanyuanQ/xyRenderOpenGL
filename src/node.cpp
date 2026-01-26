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
    renderInterface::Instance().drawPrimitive(m_vao, m_ibo, m_indices_nb);
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
    case Rectangle:
        createRectangle();
        break;
    case Cube:
        createCube();
        break;
    case Circle:
        createCircel();
        break;
    case Sphere:
        createSphere();
        break;
        // case Model:
        //     createTriangle();
        //     break;

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
void Node::createRectangle(float width, float height)
{
    int vertices_nb = 4;
    auto const indexs_nb = 2;
    auto vertices = std::vector<glm::vec3>(vertices_nb);
    auto normals = std::vector<glm::vec3>(vertices_nb);
    auto texcoords = std::vector<glm::vec2>(vertices_nb);
    auto tangents = std::vector<glm::vec3>(vertices_nb);
    auto binormals = std::vector<glm::vec3>(vertices_nb);
    auto index_sets = std::vector<glm::uvec3>(indexs_nb);

    vertices = {glm::vec3(-0.5f * width, -0.5f * height, 0.0f), // left
                glm::vec3(0.5f * width, -0.5f * height, 0.0f),  // right
                glm::vec3(0.5f * width, 0.5f * height, 0.0f),   // top
                glm::vec3(-0.5f * width, 0.5f * height, 0.0f)};
    index_sets = {
        glm::uvec3(0, 1, 2),
        glm::uvec3(0, 2, 3),
    };
    normals = {glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f),
               glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f)};
    tangents = {glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f),
                glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)};

    binormals = {glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
                 glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)};
    texcoords = {
        glm::vec2(0.0f, 0.0f), // left
        glm::vec2(1.0f, 0.0f), // right
        glm::vec2(1.0f, 1.0f),
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
    m_ibo = renderInterface::Instance().genAndBindingIBO(index_sets.size(), index_sets.data());
    m_indices_nb = indexs_nb * 3;
    m_shaderProgamID =
        shaderManager::Instance().loadShader("shader/defualt.vs", "shader/defualt.fs");
}
void Node::createCircel(float r)
{
    // std::cout << "createCircel" << std::endl;
    glm::vec3 center = glm::vec3(0.f, 0.f, 0.f);
    int vertices_nb = 1 + 100;
    auto const indexs_nb = 100;
    auto vertices = std::vector<glm::vec3>(vertices_nb);
    auto normals = std::vector<glm::vec3>(vertices_nb);
    auto texcoords = std::vector<glm::vec2>(vertices_nb);
    auto tangents = std::vector<glm::vec3>(vertices_nb);
    auto binormals = std::vector<glm::vec3>(vertices_nb);
    auto index_sets = std::vector<glm::uvec3>(indexs_nb);

    vertices.at(0) = center;
    normals.at(0) = glm::vec3(0.0f, 0.0f, 1.0f);
    tangents.at(0) = glm::vec3(1.0f, 0.0f, 0.0f);
    binormals.at(0) = glm::vec3(0.0f, 1.0f, 0.0f);
    texcoords.at(0) = glm::vec3(0.0f, 0.0f, 1.0f);
    for (int i = 0; i < vertices_nb - 1; ++i)
    {
        float totalAngle = 360.f;
        auto theta = glm::radians(i * totalAngle / (vertices_nb * 1.f));
        vertices.at(i + 1) = glm::vec3(r / 2.0 * cos(theta), r / 2.0 * sin(theta), 0.f);
        normals.at(i + 1) = glm::vec3(0.0f, 0.0f, 1.0f);
        tangents.at(i + 1) = glm::vec3(1.0f, 0.0f, 0.0f);
        binormals.at(i + 1) = glm::vec3(0.0f, 1.0f, 0.0f);
        texcoords.at(i + 1) = glm::vec3(cos(theta), sin(theta), 1.0f);
    }
    int index = 0;
    for (int i = 1; i < vertices_nb; ++i)
    {
        if (i == vertices_nb - 1)
        {
            index_sets.at(index++) = glm::uvec3(0, i, 1);
        }
        else
        {
            index_sets.at(index++) = glm::uvec3(0, i, i + 1);
        }
    }

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
    m_ibo = renderInterface::Instance().genAndBindingIBO(index_sets.size(), index_sets.data());
    m_indices_nb = indexs_nb * 3;
    m_shaderProgamID =
        shaderManager::Instance().loadShader("shader/defualt.vs", "shader/defualt.fs");
}
void Node::createSphere(float r)
{
    // Longitude and latitude
    int longitude_nb = 50;
    int latitude_nb = 2 + (longitude_nb - 2); // 你原来就这么写的，先不动

    int vertices_nb = 2 + (latitude_nb - 2) * longitude_nb;

    // 每个三角形是 1 个 index_sets
    auto const indexs_nb = longitude_nb * 2                        // 顶部 + 底部
                           + (latitude_nb - 3) * longitude_nb * 2; // 中间
    auto vertices = std::vector<glm::vec3>(vertices_nb);
    auto normals = std::vector<glm::vec3>(vertices_nb);
    auto texcoords = std::vector<glm::vec2>(vertices_nb);
    auto tangents = std::vector<glm::vec3>(vertices_nb);
    auto binormals = std::vector<glm::vec3>(vertices_nb);
    auto index_sets = std::vector<glm::uvec3>(indexs_nb);
    int count = 0;

    for (int i = 0; i < latitude_nb; ++i)
    {
        for (int j = 0; j < longitude_nb; ++j)
        {
            // 顶部极点
            if (i == 0 && j > 0)
                continue;

            // 底部极点
            if (i == latitude_nb - 1 && j > 0)
                continue;

            float theta_unit = 360.f / longitude_nb;
            float alpha_unit = 180.f / (latitude_nb - 1);

            float theta = glm::radians(theta_unit * j);
            float alpha = glm::radians(alpha_unit * i);

            glm::vec3 pos(r * cos(theta) * sin(alpha), r * cos(alpha), r * sin(theta) * sin(alpha));

            if (count >= vertices_nb)
            {
                std::cout << "vertex overflow\n";
                break;
            }

            vertices[count] = pos;
            normals[count] = glm::normalize(pos);
            texcoords[count] = glm::vec2(float(j) / longitude_nb, float(i) / (latitude_nb - 1));

            tangents[count] = glm::vec3(1, 0, 0);
            binormals[count] = glm::vec3(0, 1, 0);

            ++count;
        }
    }
    int index = 0;

    // ---------- 顶部 ----------
    for (int j = 0; j < longitude_nb; ++j)
    {
        int a = 0;
        int b = 1 + j;
        int c = 1 + (j + 1) % longitude_nb;

        index_sets[index++] = glm::uvec3(a, b, c);
    }

    // ---------- 中间 ----------
    for (int i = 0; i < latitude_nb - 3; ++i)
    {
        int ring0 = 1 + i * longitude_nb;
        int ring1 = ring0 + longitude_nb;

        for (int j = 0; j < longitude_nb; ++j)
        {
            int a = ring0 + j;
            int b = ring1 + j;
            int c = ring1 + (j + 1) % longitude_nb;
            int d = ring0 + (j + 1) % longitude_nb;

            index_sets[index++] = glm::uvec3(a, b, c);
            index_sets[index++] = glm::uvec3(a, c, d);
        }
    }

    // ---------- 底部 ----------
    int bottom = vertices_nb - 1;
    int lastRing = bottom - longitude_nb;

    for (int j = 0; j < longitude_nb; ++j)
    {
        int a = lastRing + j;
        int b = bottom;
        int c = lastRing + (j + 1) % longitude_nb;

        index_sets[index++] = glm::uvec3(a, c, b);
    }

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
    m_ibo = renderInterface::Instance().genAndBindingIBO(index_sets.size(), index_sets.data());
    m_indices_nb = indexs_nb * 3;
    m_shaderProgamID =
        shaderManager::Instance().loadShader("shader/defualt.vs", "shader/defualt.fs");
}
void Node::createCube(float width, float height, float depth)
{
    int vertices_nb = 8;
    auto const indexs_nb = 6 * 2;
    auto vertices = std::vector<glm::vec3>(vertices_nb);
    auto normals = std::vector<glm::vec3>(vertices_nb);
    auto texcoords = std::vector<glm::vec2>(vertices_nb);
    auto tangents = std::vector<glm::vec3>(vertices_nb);
    auto binormals = std::vector<glm::vec3>(vertices_nb);
    auto index_sets = std::vector<glm::uvec3>(indexs_nb);

    float hw = 0.5f * width;
    float hh = 0.5f * height;
    float hd = 0.5f * depth;

    // ================= 顶点 =================
    vertices = {
        // Front (+Z)
        glm::vec3(-hw, -hh, hd),
        glm::vec3(hw, -hh, hd),
        glm::vec3(hw, hh, hd),
        glm::vec3(-hw, hh, hd),

        // Back (-Z)
        glm::vec3(hw, -hh, -hd),
        glm::vec3(-hw, -hh, -hd),
        glm::vec3(-hw, hh, -hd),
        glm::vec3(hw, hh, -hd),

        // Left (-X)
        glm::vec3(-hw, -hh, -hd),
        glm::vec3(-hw, -hh, hd),
        glm::vec3(-hw, hh, hd),
        glm::vec3(-hw, hh, -hd),

        // Right (+X)
        glm::vec3(hw, -hh, hd),
        glm::vec3(hw, -hh, -hd),
        glm::vec3(hw, hh, -hd),
        glm::vec3(hw, hh, hd),

        // Top (+Y)
        glm::vec3(-hw, hh, hd),
        glm::vec3(hw, hh, hd),
        glm::vec3(hw, hh, -hd),
        glm::vec3(-hw, hh, -hd),

        // Bottom (-Y)
        glm::vec3(-hw, -hh, -hd),
        glm::vec3(hw, -hh, -hd),
        glm::vec3(hw, -hh, hd),
        glm::vec3(-hw, -hh, hd),
    };

    // ================= 索引 =================
    index_sets = {
        glm::uvec3(0, 1, 2),    glm::uvec3(0, 2, 3),    // Front
        glm::uvec3(4, 5, 6),    glm::uvec3(4, 6, 7),    // Back
        glm::uvec3(8, 9, 10),   glm::uvec3(8, 10, 11),  // Left
        glm::uvec3(12, 13, 14), glm::uvec3(12, 14, 15), // Right
        glm::uvec3(16, 17, 18), glm::uvec3(16, 18, 19), // Top
        glm::uvec3(20, 21, 22), glm::uvec3(20, 22, 23), // Bottom
    };

    // ================= 法线 =================
    normals = {
        {0, 0, 1},  {0, 0, 1},  {0, 0, 1},  {0, 0, 1},  {0, 0, -1}, {0, 0, -1},
        {0, 0, -1}, {0, 0, -1}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
        {1, 0, 0},  {1, 0, 0},  {1, 0, 0},  {1, 0, 0},  {0, 1, 0},  {0, 1, 0},
        {0, 1, 0},  {0, 1, 0},  {0, -1, 0}, {0, -1, 0}, {0, -1, 0}, {0, -1, 0},
    };

    // ================= Tangent =================
    tangents = {
        {1, 0, 0}, {1, 0, 0}, {1, 0, 0}, {1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
        {0, 0, 1}, {0, 0, 1}, {0, 0, 1}, {0, 0, 1}, {0, 0, -1}, {0, 0, -1}, {0, 0, -1}, {0, 0, -1},
        {1, 0, 0}, {1, 0, 0}, {1, 0, 0}, {1, 0, 0}, {1, 0, 0},  {1, 0, 0},  {1, 0, 0},  {1, 0, 0},
    };

    // ================= Binormal =================
    binormals = {
        {0, 1, 0},  {0, 1, 0},  {0, 1, 0},  {0, 1, 0},  {0, 1, 0}, {0, 1, 0}, {0, 1, 0}, {0, 1, 0},
        {0, 1, 0},  {0, 1, 0},  {0, 1, 0},  {0, 1, 0},  {0, 1, 0}, {0, 1, 0}, {0, 1, 0}, {0, 1, 0},
        {0, 0, -1}, {0, 0, -1}, {0, 0, -1}, {0, 0, -1}, {0, 0, 1}, {0, 0, 1}, {0, 0, 1}, {0, 0, 1},
    };

    // ================= UV =================
    texcoords = {
        {0, 0}, {1, 0}, {1, 1}, {0, 1}, {0, 0}, {1, 0}, {1, 1}, {0, 1},
        {0, 0}, {1, 0}, {1, 1}, {0, 1}, {0, 0}, {1, 0}, {1, 1}, {0, 1},
        {0, 0}, {1, 0}, {1, 1}, {0, 1}, {0, 0}, {1, 0}, {1, 1}, {0, 1},
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
    m_ibo = renderInterface::Instance().genAndBindingIBO(index_sets.size(), index_sets.data());
    m_indices_nb = indexs_nb * 3;
    m_shaderProgamID =
        shaderManager::Instance().loadShader("shader/defualt.vs", "shader/defualt.fs");
}