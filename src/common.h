#pragma once
#include <String>
#include <glm/glm.hpp>

using Callback = std::function<void()>;

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
};

enum primitiveType
{
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
    Rendertranformation transform;
    std::string shaderPath;
    // shader attribute
};

struct Shader
{
    std::string shaderPath;
    int shaderID;
};
