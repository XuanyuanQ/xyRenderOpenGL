#include "renderInterface.h"
#include "node.h"
#include <vector>
#include "common.h "
#include <unordered_map>

int main()
{
    renderInterface::Instance().initContex();
    auto node = std::make_shared<Node>();
    node->createPrimitive();

    // std::vector<Callback> renderQueue;
    std::unordered_map<int, std::vector<Callback>> renderQueue;
    if (renderQueue.find(node->getShaderProgamID()) == renderQueue.end())
    {
        std::vector<Callback> drawQueue;
        drawQueue.push_back([node]() { node->draw(); });
        renderQueue[node->getShaderProgamID()] = drawQueue;
    }
    else
    {
        auto& drawQueue = renderQueue[node->getShaderProgamID()];
        drawQueue.push_back([node]() { node->draw(); });
    }

    // renderQueue.push_back([node]() { node->draw(); });
    renderInterface::Instance().render(renderQueue);
}
