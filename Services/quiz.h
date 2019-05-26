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
    explicit Quiz(Model* model, QObject *parent = nullptr);

    void Start();

signals:
    void activePicketChanged(int picketIndex);

    void guessSucceeded(int activePicketIndex, size_t guessedPicketIndex);
    void guessFailed(int activePicketIndex, size_t guessedPicketIndex);

public slots:
    void checkGuess(size_t guess);

private:
    Model* model;

    int activePicket = -1;

    std::vector<int> unplayed;

    int getNextActivePicket();
};

#endif // QUIZ_H
