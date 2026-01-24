#pragma once
#include <String>
#include <glm/glm.hpp>

// 窗口大小
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

using Callback = std::function<void()>;

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
};

enum primitiveType
{
    Triangle = 0,
    Rectangle,
    Cube,
    Circal,
    Sphere,
    Model
};

struct Postion
{
    float x;
    float y;
    float z;
};

struct Scale
{
    float x;
    float y;
    float z;
};

struct Rotation
{
    float x;
    float y;
    float z;
};

struct Rendertranformation
{
    Postion pos;
    Scale scale;
    Rotation rotation;
};

struct primitive
{
    primitiveType type;
    // Rendertranformation transform;
    std::string shaderPath;
    // shader attribute
};

struct PrimitveData
{
    void* data;         // attribute 数据
    int dataSize;       // 该 attribute 的总字节数
    int locationID;     // layout(location = x)
    int componentCount; // 3 for vec3, 2 for vec2
    int offset;         // 在 VBO 中的偏移
};

struct Shader
{
    std::string shaderPath;
    int shaderID;
};
