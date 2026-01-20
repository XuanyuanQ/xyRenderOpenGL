#pragma once
#include "common.h"
#include <algorithm>
#include <String>

class Node
{

  public:
    Node(/* args */);
    ~Node();
    primitive createPrimitive();
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

  private:
    primitive m_content;
    int m_vbo;
    int m_vao;
    int m_shaderProgamID;
    Node* m_parent;
    std::vector<Node*> m_children;
};
