#include "model.h"

void Model::setData(std::vector<Pipeline> pipelines)
{
    this->pipelines = std::move(pipelines);

    for(auto& pipeline : this->pipelines)
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
