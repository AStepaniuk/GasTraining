#include "mainwindow.h"

#include "View/mainview.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    auto view = new MainView();
    view->setModel(&model);
    setCentralWidget(view);

    showMaximized();
}

MainWindow::~MainWindow()
{

}
