#include "renderInterface.h"
#include "node.h"
#include <vector>
#include "common.h "
#include <unordered_map>
#include <shaderManager.h>

int main()
{
    renderInterface::Instance().initContex();
    auto node = std::make_shared<Node>();
    // node->createPrimitive();
    node->createPrimitive(static_cast<primitiveType>(0));

    // std::vector<Callback> renderQueue;

    auto uniformFunc = [node](std::shared_ptr<Camera> camera)
    // 测试用，后续需要修改调用，如view矩阵等同一个shader只是传递一次
    {
        shaderManager::Instance().bindingUniAttr4mat(node->getShaderProgamID(), "view",
                                                     camera->getViewMatrix());
        shaderManager::Instance().bindingUniAttr4mat(node->getShaderProgamID(), "projection",
                                                     camera->getProjectionMatrix());
        glm::mat4 world = glm::mat4(1.0);
        if (node->getParent() != nullptr)
        {
            world = node->getParent()->getTransform()->getWorldMatrix();
        }
        if (node->getTransform() != nullptr)
        {
            world = world * node->getTransform()->getWorldMatrix();
        }
        shaderManager::Instance().bindingUniAttr4mat(node->getShaderProgamID(), "world", world);
    };
    std::unordered_map<int, std::vector<std::function<void(std::shared_ptr<Camera>)>>> renderQueue;
    std::vector<std::function<void(std::shared_ptr<Camera>)>> drawQueue;
    if (renderQueue.find(node->getShaderProgamID()) == renderQueue.end())
    {

        renderQueue[node->getShaderProgamID()] = drawQueue;
    }

    renderQueue[node->getShaderProgamID()].push_back(
        [node, uniformFunc](std::shared_ptr<Camera> camera)
        {
            uniformFunc(camera); // 执行矩阵绑定
            node->draw();        // 执行绘制
        });

    // renderQueue.push_back([node]() { node->draw(); });
    renderInterface::Instance().render(renderQueue);
}
