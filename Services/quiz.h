#ifndef QUIZ_H
#define QUIZ_H

#include <QObject>

#include <vector>

#include "Data/model.h"
#include "View/picketsview.h"

class Quiz : public QObject
{
    Q_OBJECT
public:
    explicit Quiz(Model* model, PicketsView* view, QObject *parent = nullptr);

    void Start();

signals:

public slots:
    void checkGuess(int guess);

private:
    Model* model;
    PicketsView* view;

    int activePicket = -1;

    std::vector<int> unplayed;

    int getNextActivePicket();
};

#endif // QUIZ_H
