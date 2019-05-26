#include "quiz.h"

#include <random>
#include <algorithm>

#include "turngeometry.h"

namespace QuizImpl
{
    bool arePipesEqual(const Pipeline* pipeline1, const Pipeline* pipeline2)
    {
        if (pipeline1 == pipeline2)
        {
            return true;
        }

        if (pipeline1->pressure != pipeline2->pressure)
        {
            return false;
        }

        return true;
    }

    bool areSectionsEqual(const PipeSection* section1, const PipeSection* section2)
    {
        if (section1 == section2)
        {
            return true;
        }

        if (!arePipesEqual(section1->pipeline, section2->pipeline))
        {
            return false;
        }

        if (section1->material != section2->material)
        {
            return false;
        }

        if (section1->diameter != section2->diameter)
        {
            return false;
        }

        return true;
    }

    bool areNodesEqual(const Node* node1, const Node* node2)
    {
        if (node1 == node2)
        {
            return true;
        }

        if (!areSectionsEqual(node1->section, node2->section))
        {
            return false;
        }

        if (node1->type != node2->type)
        {
            return false;
        }

        if (node1->type == NodeType::Turn)
        {
            const auto geometry1 = TurnGeometry::Calculate(*node1);
            const auto geometry2 = TurnGeometry::Calculate(*node2);

            if (geometry1.picketTurnAngle != geometry2.picketTurnAngle)
            {
                return false;
            }

            int angleMin1 = geometry1.picketAngle1 < geometry1.picketAngle2
                ? geometry1.picketAngle1
                : geometry1.picketAngle2;

            int angleMin2 = geometry2.picketAngle1 < geometry2.picketAngle2
                ? geometry2.picketAngle1
                : geometry2.picketAngle2;

            if (angleMin1 != angleMin2)
            {
                return false;
            }
        }

        return true;
    }

    bool arePicketsEqual(const Picket& picket1, const Picket& picket2)
    {
        const auto node1 = picket1.node;
        const auto node2 = picket2.node;

        return areNodesEqual(node1, node2);
    }
}

using namespace QuizImpl;

Quiz::Quiz(Model* model, QObject *parent)
    : QObject(parent)
    , model { model }
{

}

void Quiz::Start()
{
    unplayed.clear();

    for (int i = 0; i < model->getPickets().size(); ++i)
    {
        unplayed.push_back(i);
    }

    activePicket = getNextActivePicket();
    emit activePicketChanged(activePicket);
}

void Quiz::checkGuess(size_t guess)
{
    if (activePicket < 0)
    {
        return;
    }

    const auto& actualPicket =  model->getPickets()[activePicket];
    const auto& guessedPicket =  model->getPickets()[guess];

    if (arePicketsEqual(actualPicket, guessedPicket))
    {
        emit guessSucceeded(activePicket, guess);
    }
    else
    {
        emit guessFailed(activePicket, guess);
    }

    activePicket = getNextActivePicket();
    if (activePicket >= 0)
    {
        emit activePicketChanged(activePicket);
    }
}

int Quiz::getNextActivePicket()
{
    if (!unplayed.empty())
    {
        std::mt19937 eng(static_cast<long unsigned int>(time(nullptr)));
        std::uniform_int_distribution<> distr(0, unplayed.size()-1);

        const auto next = distr(eng);

        const auto res = unplayed[next];
        unplayed.erase(unplayed.begin() + next);

        return res;
    }
    else
    {
        return -1;
    }
}
