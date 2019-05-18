#include "quiz.h"

#include <random>

Quiz::Quiz(Model* model, PicketsView* view, QObject *parent)
    : QObject(parent)
    , model { model }
    , view { view }
{

}

void Quiz::Start()
{
    const auto pickets = model->getPickets();

    std::random_device rd;
    std::mt19937 eng(rd());
    std::uniform_int_distribution<> distr(0, pickets.size());

    activePicket = distr(eng);

    view->setActivePicket(activePicket);
}
