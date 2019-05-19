#include "quiz.h"

#include <random>
#include <algorithm>

Quiz::Quiz(Model* model, PicketsView* view, QObject *parent)
    : QObject(parent)
    , model { model }
    , view { view }
{

}

void Quiz::Start()
{
    activePicket = getNextActivePicket();
    view->setActivePicket(activePicket);
}

void Quiz::checkGuess(int guess)
{
    if (guess == activePicket)
    {
        view->markSucceed(activePicket);
    }
    else
    {
        view->markFailed(activePicket);
    }

    past.push_back(activePicket);
    if (past.size() < model->getPickets().size())
    {
        activePicket = getNextActivePicket();
        view->setActivePicket(activePicket);
    }
}

int Quiz::getNextActivePicket()
{
    const auto pickets = model->getPickets();
    do
    {
        std::mt19937 eng(static_cast<long unsigned int>(time(nullptr)));
        std::uniform_int_distribution<> distr(0, pickets.size()-1);

        const auto next = distr(eng);

        if (std::find(past.begin(), past.end(), next) == past.end())
        {
            return next;
        }
    }
    while(true);
}
