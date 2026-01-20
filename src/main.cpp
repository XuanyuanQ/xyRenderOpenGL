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
    node->createPrimitive();

    // std::vector<Callback> renderQueue;

    auto uniformFunc = [node](std::shared_ptr<Camera> camera)
    // 测试用，后续需要修改调用，如view矩阵等同一个shader只是传递一次
    {
        shaderManager::Instance().bindingUniAttr4mat(node->getShaderProgamID(), "view",
                                                     camera->getViewMatrix());
        shaderManager::Instance().bindingUniAttr4mat(node->getShaderProgamID(), "projection",
                                                     camera->getProjectionMatrix());
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
