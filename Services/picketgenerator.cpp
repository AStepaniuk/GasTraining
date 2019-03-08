#include "picketgenerator.h"

PicketGenerator::PicketGenerator()
{

}

void PicketGenerator::generatePickets(Model *model)
{
    for(const auto& pipeline : model->getPipelines())
    {
        for(const auto& section : pipeline.sections)
        {
            for(const auto& node : section.nodes)
            {
                model->addPicket(Picket { &node });
            }
        }
    }
}
