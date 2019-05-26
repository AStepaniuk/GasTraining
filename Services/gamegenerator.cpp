#include "gamegenerator.h"


Model GameGenerator::GenerateTestModel()
{
    std::vector<Pipeline> pipelines =
    {
        {
            PressureType::G3,
            {
                {
                    PipeMaterial::St,
                    110,
                    {
                        { 0, 0, -0.12, NodeType::End, nullptr },
                        { 0, 50, -0.12, NodeType::Turn, nullptr },
                        { 50, 50, -0.12, NodeType::Turn, nullptr },
                        { 80, 20, -0.12, NodeType::Turn, nullptr },
                        { 80, -30, -0.12, NodeType::Turn, nullptr },
                        { 120, -40, -0.12, NodeType::End, nullptr }
                    },
                    nullptr
                }
            }
        }
    };

    Model result;
    result.setData(std::move(pipelines));

    return result;
}
