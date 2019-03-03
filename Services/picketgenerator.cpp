#include "picketgenerator.h"

PicketGenerator::PicketGenerator()
{

}

void PicketGenerator::generatePickets(Model *model)
{
    auto& nodes = model->getNodes();

    for(int i = 0; i < nodes.size(); ++i)
    {
        model->addPicket(Picket { &nodes[i] });
    }
}
