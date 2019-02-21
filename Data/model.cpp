#include "model.h"

Model::Model()
{
    nodes =
    {
        { 0, 0, -0.12, NodeType::End },
        { 0, 50, -0.12, NodeType::Turn },
        { 80, 50, -0.12, NodeType::Turn },
        { 80, -30, -0.12, NodeType::Turn },
        { 120, -30, -0.12, NodeType::End }
    };
}

size_t Model::addPicket(const Picket &picket)
{
    pickets.push_back(picket);
    return pickets.size() - 1;
}

const std::vector<Node>& Model::getNodes() const
{
    return nodes;
}

const std::vector<Picket> &Model::getPickets() const
{
    return pickets;
}
