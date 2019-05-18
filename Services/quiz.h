#ifndef QUIZ_H
#define QUIZ_H

#include <QObject>

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

private:
    Model* model;
    PicketsView* view;

    int activePicket = -1;
};

#endif // QUIZ_H
