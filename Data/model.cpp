#include "model.h"

Model::Model()
{
    pipelines =
    {
        {
            PressureType::G2,
            {
                {
                    PipeMaterial::St,
                    110,
                    {
                        { 0, 0, -0.12, NodeType::End, nullptr },
                        { 0, 50, -0.12, NodeType::Turn, nullptr },
                        { 80, 50, -0.12, NodeType::Turn, nullptr },
                        { 80, -30, -0.12, NodeType::Turn, nullptr },
                        { 120, -30, -0.12, NodeType::End, nullptr }
                    },
                    nullptr
                }
            }
        }
    };

    for(auto& pipeline :pipelines)
    {
        for(auto& section : pipeline.sections)
        {
            for(auto& node : section.nodes)
            {
                nodes.push_back(&node);

                node.section = &section;
            }

            section.pipeline = &pipeline;
        }
    }
}

int Model::addPicket(const Picket &picket)
{
    pickets.push_back(picket);
    return pickets.size() - 1;
}

const std::vector<Pipeline>& Model::getPipelines() const
{
    return pipelines;
}

const std::vector<Node *> &Model::getNodes() const
{
    return nodes;
}

const std::vector<Picket> &Model::getPickets() const
{
    return pickets;
}
