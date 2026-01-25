#pragma once
#include "common.h"
#include <algorithm>
#include <String>
#include <memory>
#include "transform.h"

class Node
{

  public:
    Node(/* args */);
    ~Node();
    primitive createPrimitive();
    void createPrimitive(primitiveType type);
    void draw();
    int getShaderProgamID()
    {
        return m_shaderProgamID;
    }
    Node* getParent()
    {
        return m_parent;
    }
    std::vector<Node*>& getChildren()
    {
        return m_children;
    }

    void setParent(Node* parent);
    void addChildren(std::vector<Node*>& children);
    transform* getTransform()
    {
        return m_transform.get();
    }

  private:
    void createTriangle(float width = 1.f, float height = 1.f);
    void createRectangle(float width = 1.f, float height = 1.f);
    void createCircel(float r = 1.f);
    void createSphere(float width = 1.f, float height = 1.f);
    void createCube(float width = 1.f, float height = 1.f, float depth = 1.f);

  private:
    primitive m_content;
    int m_vbo;
    int m_vao;
    int m_ibo = 0;
    int m_indices_nb = 0;
    int m_shaderProgamID;
    Node* m_parent = nullptr;
    std::vector<Node*> m_children;
    std::shared_ptr<transform> m_transform;
};
