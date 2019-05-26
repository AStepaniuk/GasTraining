#include "gamegenerator.h"

#include <map>
#include <utility>

#include "randomgenerator.h"

namespace GameGeneratorImpl
{
    Pipeline GeneratePipeline()
    {
        int pressure = RandomGenerator::Get(3);
        return { static_cast<PressureType>(pressure), {} };
    }

    PipeSection GeneratePipeSection(PressureType pressure)
    {
        using PipeRange = std::tuple<PipeMaterial, std::vector<int>>;

        static std::map<PressureType, std::vector<PipeRange>> availableSections =
        {
            {
                PressureType::G1,
                {
                    std::make_tuple(PipeMaterial::St, std::vector<int>{ 57, 89, 108, 159 })
                },
            },
            {
                PressureType::G2,
                {
                    std::make_tuple(PipeMaterial::St, std::vector<int>{ 57, 89, 108, 159, 180, 219, 273, 325 })
                },
            },
            {
                PressureType::G3,
                {
                    std::make_tuple(PipeMaterial::St, std::vector<int>{ 219, 273, 325, 377, 426, 530 })
                },
            },
            {
                PressureType::G4,
                {
                    std::make_tuple(PipeMaterial::St, std::vector<int>{ 377, 426, 530, 630, 720, 820, 920 })
                }
            }
        };

        const auto pipeRanges = availableSections[pressure];

        const auto rangeIndex = RandomGenerator::Get(pipeRanges.size() - 1);
        const auto range = pipeRanges[rangeIndex];

        const auto material = std::get<0>(range);
        const auto& diameters = std::get<1>(range);
        const auto diameterIndex = RandomGenerator::Get(diameters.size() - 1);
        const auto diameter = diameters[diameterIndex];

        return { material, diameter, {}, nullptr };
    }
}

using namespace GameGeneratorImpl;

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

Model GameGenerator::GenerateModel()
{
    auto pipeline = GeneratePipeline();

    auto section = GeneratePipeSection(pipeline.pressure);
    section.nodes =
    {
        { 0, 0, -0.12, NodeType::End, nullptr },
        { 0, 50, -0.12, NodeType::Turn, nullptr },
        { 50, 50, -0.12, NodeType::Turn, nullptr },
        { 80, 20, -0.12, NodeType::Turn, nullptr },
        { 80, -30, -0.12, NodeType::Turn, nullptr },
        { 120, -40, -0.12, NodeType::End, nullptr }
    };

    pipeline.sections = { std::move(section) };

    std::vector<Pipeline> pipelines { std::move(pipeline) };

    Model result;
    result.setData(std::move(pipelines));

    return result;
}
