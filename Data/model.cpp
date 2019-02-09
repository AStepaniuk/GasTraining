#include "model.h"

Model::Model()
{
    nodes =
    {
        { 0, 0, -0.12, NodeType::End },
        { 0, 50, -0.12, NodeType::Turn },
        { 80, 50, -0.12, NodeType::Turn },
        { 80, 30, -0.12, NodeType::End }
    };
}

const std::vector<Node>& Model::getNodes() const
{
    return nodes;
}
