#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    pyconsole = new PythonConsole{};

    OCCGeometry occgeo;
    view = new PCBView{};
    view->createPipline(occgeo);

    QWidget* mainWidget = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(mainWidget);
    layout->addWidget(createWindowContainer(view->window),7);
    layout->addWidget(pyconsole,3);
    setCentralWidget(mainWidget);

    PySharing::showOCCShape = [=](TopoDS_Shape shape){
        OCCGeometry og(shape);
        view->repaint(og);
    };
}

MainWindow::~MainWindow() {
    delete pyconsole;
    delete view;
}
