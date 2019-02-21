#include "mainwindow.h"

#include <QVBoxLayout>

#include "View/mainview.h"
#include "View/picketsview.h"
#include "Services/picketgenerator.h"

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

    showMaximized();
}

MainWindow::~MainWindow()
{

}
