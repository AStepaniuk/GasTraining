#include "mainwindow.h"

#include <QVBoxLayout>

#include "View/mainview.h"
#include "View/picketsview.h"
#include "Services/picketgenerator.h"
#include "Services/quiz.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    PicketGenerator picketGenerator;
    picketGenerator.generatePickets(&model);

    auto view = new MainView();
    view->setModel(&model);

    auto pickets = new PicketsView();
    pickets->setPickets(model.getPickets());

    auto layout = new QVBoxLayout();
    layout->addWidget(pickets, 1);
    layout->addWidget(view, 5);

    layout->setMargin(0);

    auto centralWidget = new QWidget();
    centralWidget->setLayout(layout);

    setCentralWidget(centralWidget);

    auto quiz = new Quiz(&model, pickets);

    QObject::connect(view, &MainView::picketClicked, quiz, &Quiz::checkGuess);

    QObject::connect(quiz, &Quiz::activePicketChanged, pickets, &PicketsView::setActivePicket);
    QObject::connect(quiz, &Quiz::guessSucceeded, [pickets](auto active, auto /*guessed*/) { pickets->markSucceed(active); });
    QObject::connect(quiz, &Quiz::guessFailed, [pickets](auto active, auto /*guessed*/) { pickets->markFailed(active); });

    QObject::connect(quiz, &Quiz::guessSucceeded, [view](auto /*active*/, auto guessed) { view->markPicketSucceed(guessed); });
    QObject::connect(quiz, &Quiz::quizEnd, view, &MainView::stopInteractivety);

    showMaximized();

    quiz->Start();
}

MainWindow::~MainWindow()
{

}
