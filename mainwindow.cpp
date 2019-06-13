#include "mainwindow.h"

#include <QVBoxLayout>
#include <QMenuBar>
#include <QAction>
#include <QApplication>

#include "View/mainview.h"
#include "View/picketsview.h"
#include "Services/picketgenerator.h"
#include "Services/quiz.h"
#include "Services/gamegenerator.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow { parent }
    //, model { GameGenerator::GenerateTestModel() }
    , model { GameGenerator::GenerateModel() }
    , quiz { &model, this }
{
    setWindowTitle(tr("Привязочные знаки"));

    auto restartAction = new QAction(tr("&Начать"), this);
    auto exitAction = new QAction(tr("&Выход"), this);

    auto trainingMenu = menuBar()->addMenu(tr("&Тренировка"));
    trainingMenu->addAction(restartAction);
    trainingMenu->addSeparator();
    trainingMenu->addAction(exitAction);

    QObject::connect(restartAction, &QAction::triggered, this, &MainWindow::restart);
    QObject::connect(exitAction, &QAction::triggered, this, &MainWindow::exit);

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

    QObject::connect(view, &MainView::picketClicked, &quiz, &Quiz::checkGuess);

    QObject::connect(&quiz, &Quiz::activePicketChanged, pickets, &PicketsView::setActivePicket);
    QObject::connect(&quiz, &Quiz::guessSucceeded, [pickets](auto active, auto /*guessed*/) { pickets->markSucceed(active); });
    QObject::connect(&quiz, &Quiz::guessFailed, [pickets](auto active, auto /*guessed*/) { pickets->markFailed(active); });

    QObject::connect(&quiz, &Quiz::guessSucceeded, [view](auto /*active*/, auto guessed) { view->markPicketSucceed(guessed); });
    QObject::connect(&quiz, &Quiz::quizEnd, view, &MainView::stopInteractivety);

    showMaximized();

    quiz.Start();
}

MainWindow::~MainWindow()
{

}

void MainWindow::exit()
{
    QApplication::exit();
}

void MainWindow::restart()
{
    model = GameGenerator::GenerateModel();
    PicketGenerator picketGenerator;
    picketGenerator.generatePickets(&model);

    quiz.Start();
}
