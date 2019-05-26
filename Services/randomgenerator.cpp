#include "randomgenerator.h"

#include <random>

namespace RandomGeneratorImpl
{
    std::mt19937& GetRandomEngine()
    {
        static std::mt19937 eng(static_cast<long unsigned int>(time(nullptr)));
        return eng;
    }
}

using namespace RandomGeneratorImpl;

int RandomGenerator::Get(int max)
{
    std::uniform_int_distribution<> distr(0, max);
    return distr(GetRandomEngine());
}
