#pragma once
#include "PythonConsole.h"
#include "PySharing.h"

#include"PCBView.h"

#include <QtWidgets/QMainWindow>
#include "ui_PCB3D_1.h"

#include <vsgQt/Window.h>


class PCB3D_1 : public QMainWindow
{
    Q_OBJECT

public:
    PCB3D_1(QWidget *parent = nullptr);
    ~PCB3D_1();
    void changeShape();

private:
    Ui::PCB3D_1Class ui;
    PythonConsole* pyconsole;
    PCBView* view;
    

};
