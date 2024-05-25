#include "PyBindingFunctions.h"
#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    pyconsole = new PythonConsole{};

    TopoDS_Shape shape = BRepPrimAPI_MakeBox(20, 20, 20);
    Discrete_Shape dshape(shape);
    view = new CADViewer{};
    view->createPipline(dshape);

    QWidget* mainWidget = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(mainWidget);
    layout->addWidget(createWindowContainer(view->window),7);
    layout->addWidget(pyconsole,3);
    setCentralWidget(mainWidget);

    bindingPythonConsole = pyconsole;
    cadViewer = view;
    /*PySharing::showOCCShape = [=](TopoDS_Shape shape){
        Discrete_Shape og(shape);
        view->repaint(og);
    };*/
}

MainWindow::~MainWindow() {
    delete pyconsole;
    delete view;
}
