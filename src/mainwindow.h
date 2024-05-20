/*
* Ö÷½çÃæUI
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "PythonConsole.h"

#include "PCBView.h"

#include <QtWidgets/QMainWindow>
#include <QWidget>
#include <QLayout>
#include <QVBoxLayout>

#include <vsgQt/Window.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    PythonConsole* pyconsole;
    PCBView* view;

};

#endif