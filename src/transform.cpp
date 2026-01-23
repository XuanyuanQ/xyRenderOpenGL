#include "transform.h"
#include <glm/gtc/matrix_transform.hpp>
transform::transform(/* args */)
{
    m_world = glm::mat4(1.0);
}

transform::~transform() {}

void transform::updateWorldMatrix()
{
    glm::mat4 model = glm::mat4(1.0f);
    auto scaleM = glm::scale(model, m_scale);
    auto tranM = glm::translate(model, m_translation);
    auto rotaMx = glm::rotate(model, glm::radians(m_roAngle.x), glm::vec3(1, 0, 0));
    auto rotaMy = glm::rotate(model, glm::radians(m_roAngle.y), glm::vec3(0, 1, 0));
    auto rotaMz = glm::rotate(model, glm::radians(m_roAngle.z), glm::vec3(0, 0, 1));

    m_world = tranM * rotaMz * rotaMy * rotaMx * scaleM;
}
void transform::setTranslation(glm::vec3 v)
{
    m_translation = v;
}
void transform::setTranslationX(float x)
{
    m_translation.x = x;
}
void transform::setTranslationY(float y)
{
    m_translation.y = y;
}
void transform::setTranslationZ(float z)
{
    m_translation.z = z;
}
void transform::setRotation(glm::vec3 v)
{
    m_roAngle = v;
}
void transform::setRotationX(float x)
{
    m_roAngle.x = x;
}
void transform::setRotationY(float y)
{
    m_roAngle.y = y;
}
void transform::setRotationZ(float z)
{
    m_roAngle.y = z;
}
void transform::setScale(glm::vec3 v)
{
    m_scale = v;
}
void transform::setScaleX(float x)
{
    m_scale.x = x;
}
void transform::setScaleY(float y)
{
    m_scale.y = y;
}
void transform::setScaleZ(float z)
{
    m_scale.y = z;
}
glm::mat4 transform::getWorldMatrix()
{
    updateWorldMatrix();
    return m_world;
}
glm::vec3 transform::getTranslation()
{
    return m_translation;
}
float transform::getTranslationX()
{
    return m_translation.x;
}
float transform::getTranslationY()
{
    return m_translation.y;
}
float transform::getTranslationZ()
{
    return m_translation.z;
}
glm::vec3 transform::getRotation()
{
    return m_roAngle;
}
float transform::getRotationX()
{
    return m_roAngle.x;
}
float transform::getRotationY()
{
    return m_roAngle.y;
}
float transform::getRotationZ()
{
    return m_roAngle.z;
}
glm::vec3 transform::getScale()
{
    return m_scale;
}
float transform::getScaleX()
{
    return m_scale.x;
}
float transform::getScaleY()
{
    return m_scale.y;
}
float transform::getScaleZ()
{
    return m_scale.z;
}