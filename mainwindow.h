#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "Data/model.h"
#include "Services/quiz.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

    Model model;
    Quiz quiz;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void exit();
    void restart();
};

#endif // MAINWINDOW_H
