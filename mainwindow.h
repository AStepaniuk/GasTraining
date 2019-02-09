#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "Data/model.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

    Model model;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};

#endif // MAINWINDOW_H
