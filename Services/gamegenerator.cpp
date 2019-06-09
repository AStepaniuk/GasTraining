#include "gamegenerator.h"

#include <map>
#include <utility>
#include <cmath>

#include "randomgenerator.h"

namespace GameGeneratorImpl
{
    constexpr auto minSensitiveAngle = 7;

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

    int GeneratePipeAngle(int minAngle, int maxAngle, int normalProbabilityPercents)
    {
        const int isNormal = RandomGenerator::Get(100) <= normalProbabilityPercents;
        if (isNormal)
        {
            const auto minNormalDiff = minAngle % 90;
            const auto minNormalAngle = minNormalDiff == 0
                ? minAngle
                : minAngle > 0 ? minAngle + 90 - minNormalDiff : minAngle - minNormalDiff;

            const auto maxNormalDiff = maxAngle % 90;
            const auto maxNormalAngle = maxNormalDiff == 0
                ? maxAngle
                : maxAngle > 0 ? maxAngle - maxNormalDiff : maxAngle + 90 - maxNormalDiff;

            const auto delta = maxNormalAngle - minNormalAngle;
            auto sections = delta / 90;

            return minNormalAngle + RandomGenerator::Get(sections) * 90;
        }
        else
        {
            const auto delta = maxAngle - minAngle;
            auto sections = delta / 90;

            const auto lastSectionRest = delta % 90;
            if (lastSectionRest != 0)
            {
                sections++;
            }

            const auto section = RandomGenerator::Get(sections - 1);

            const auto maxA = (section == sections - 1) && (lastSectionRest > 0)
                ? lastSectionRest
                : 90 - minSensitiveAngle;

            const auto minA = minSensitiveAngle < maxA ? minSensitiveAngle : maxA;

            return minAngle + section * 90 + minA + RandomGenerator::Get(maxA - minA);
        }
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
                        { -1, 50, -0.12, NodeType::Turn, nullptr },
                        { 100, 70, -0.12, NodeType::End, nullptr }

                        // { 0, 0, -0.12, NodeType::End, nullptr },
                        // { 0, 50, -0.12, NodeType::Turn, nullptr },
                        // { 50, 50, -0.12, NodeType::Turn, nullptr },
                        // { 80, 20, -0.12, NodeType::Turn, nullptr },
                        // { 80, -30, -0.12, NodeType::Turn, nullptr },
                        // { 120, -40, -0.12, NodeType::End, nullptr }
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

    auto prevDirection = GeneratePipeAngle(-180, 180, 95);
    auto prevX = 0;
    auto prevY = 0;

    auto nodesNum = 4 + RandomGenerator::Get(3);
    section.nodes.push_back({ prevX, prevY, -0.12, NodeType::End, nullptr });

    for(int i = 0; i < nodesNum; ++i)
    {
        const auto len = 1 + RandomGenerator::Get(19);
        auto dir = GeneratePipeAngle(prevDirection - 90, prevDirection + 90, 90);

        auto da = std::abs(dir - prevDirection);
        while(da <= minSensitiveAngle || da > 90)
        {
            dir = GeneratePipeAngle(prevDirection - 90, prevDirection + 90, 90);
            da = std::abs(dir - prevDirection);
        }

        const auto x = prevX + len * std::cos(dir / 180.0 * M_PI);
        const auto y = prevY + len * std::sin(dir / 180.0 * M_PI);

        const auto type = i == nodesNum - 1 ? NodeType::End : NodeType::Turn;

        section.nodes.push_back({ x, y, -0.12, type, nullptr });

        prevDirection = dir;
        prevX = x;
        prevY = y;
    }

    pipeline.sections = { std::move(section) };

    std::vector<Pipeline> pipelines { std::move(pipeline) };

    Model result;
    result.setData(std::move(pipelines));

    return result;
}
