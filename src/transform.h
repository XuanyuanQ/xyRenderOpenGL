#pragma once
#include <glm/glm.hpp>
class transform
{

  public:
    transform(/* args */);
    ~transform();

    void setTranslation(glm::vec3 v);
    void setTranslationX(float x);
    void setTranslationY(float y);
    void setTranslationZ(float z);
    void setRotation(glm::vec3 v);
    void setRotationX(float x);
    void setRotationY(float y);
    void setRotationZ(float z);
    void setScale(glm::vec3 v);
    void setScaleX(float x);
    void setScaleY(float y);
    void setScaleZ(float z);
    glm::mat4 getWorldMatrix();

    glm::vec3 getTranslation();

    float getTranslationX();

    float getTranslationY();

    float getTranslationZ();

    glm::vec3 getRotation();
    float getRotationX();
    float getRotationY();
    float getRotationZ();
    glm::vec3 getScale();
    float getScaleX();
    float getScaleY();
    float getScaleZ();

  private:
    void updateWorldMatrix();

  private:
    glm::mat4 m_world;
    glm::vec3 m_translation = glm::vec3(0.f);
    glm::vec3 m_roAngle = glm::vec3(0.f);
    glm::vec3 m_scale = glm::vec3(1.f);
};
