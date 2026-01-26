#include "renderInterface.h"
#include "node.h"
#include <vector>
#include "common.h "
#include <unordered_map>
#include <shaderManager.h>
std::unordered_map<int, std::vector<std::function<void(std::shared_ptr<Camera>)>>> renderQueue;
std::function<void(std::shared_ptr<Camera>)> testFuncCb(std::shared_ptr<Node> node)
{
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
    return uniformFunc;
}

void testRenderQueue(std::shared_ptr<Node> node, std::function<void(std::shared_ptr<Camera>)> cb)
{
    std::vector<std::function<void(std::shared_ptr<Camera>)>> drawQueue;
    if (renderQueue.find(node->getShaderProgamID()) == renderQueue.end())
    {

        renderQueue[node->getShaderProgamID()] = drawQueue;
    }

    renderQueue[node->getShaderProgamID()].push_back(
        [node, cb](std::shared_ptr<Camera> camera)
        {
            cb(camera);   // 执行矩阵绑定
            node->draw(); // 执行绘制
        });
}
int main()
{
    renderInterface::Instance().initContex();
    auto node = std::make_shared<Node>();
    node->createPrimitive(static_cast<primitiveType>(0));
    node->getTransform()->setTranslation(glm::vec3(0.7f, -1.f, 0.f));
    auto uniformFunc = testFuncCb(node);
    testRenderQueue(node, uniformFunc);

    auto rectangle = std::make_shared<Node>();
    rectangle->createPrimitive(static_cast<primitiveType>(1));
    rectangle->getTransform()->setTranslation(glm::vec3(-0.7f, 1.f, 0.f));
    uniformFunc = testFuncCb(rectangle);
    testRenderQueue(rectangle, uniformFunc);

    auto circle = std::make_shared<Node>();
    // circle->createPrimitive(primitiveType::Circle);
    // circle->createPrimitive(primitiveType::Cube);
    uniformFunc = testFuncCb(circle);
    testRenderQueue(circle, uniformFunc);
    renderInterface::Instance().render(renderQueue);
}
