#pragma once
#include "common.h"

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

  private:
    primitive m_content;
    int m_vbo;
    int m_vao;
    int m_shaderProgamID;
};
