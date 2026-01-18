#include "renderInterface.h"
#include "node.h"
#include <vector>
#include "common.h "

int main()
{
    renderInterface::Instance().initContex();
    auto node = std::make_shared<Node>();
    node->createPrimitive();
    std::vector<Callback> renderQueue;
    renderQueue.push_back([node]() { node->draw(); });
    renderInterface::Instance().render(renderQueue);
}
