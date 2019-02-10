#include "mainwindow.h"

#include "View/mainview.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    auto view = new MainView();
    setCentralWidget(view);
    view->setModel(&model);

    showMaximized();
}

MainWindow::~MainWindow()
{

}
