#pragma once
#include <map>     // std::map
#include <string>  // std::string
#include <utility> // std::pair
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
class shaderManager
{
  public:
    static shaderManager& Instance()
    {
        static shaderManager instance; // C++11 起：线程安全
        return instance;
    }

  public:
    int loadShader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
    void activeShader(int shaderProgamID);
    void bindingUniAttr1i(int shaderID, const std::string& name, int value);
    void bindingUniAttr1f(int shaderID, const std::string& name, float value);
    void bindingUniAttr4mat(int shaderID, const std::string& name, const glm::mat4& value);
    void bindingUniAttr4f(int shaderID, const std::string& name, const glm::vec4& value);

  private:
    shaderManager(/* args */);
    ~shaderManager();
    std::string loadShaderSource(const std::string& filePath);

  private:
    std::map<std::pair<std::string, std::string>, int> m_shaderIDMap;
};
