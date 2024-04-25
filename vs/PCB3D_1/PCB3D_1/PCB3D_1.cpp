#include "PCB3D_1.h"

#include<QWidget>
#include<QLayout>
#include<QVBoxLayout>

PCB3D_1::PCB3D_1(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    pyconsole = new PythonConsole{};

    OCCGeometry occgeo;
    view = new PCBView{};
    view->createPipline(occgeo);

    QWidget* mainWidget = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(mainWidget);
    layout->addWidget(createWindowContainer(view->window), 7);
    layout->addWidget(pyconsole, 3);
    setCentralWidget(mainWidget);

   PySharing::showOCCShape = [=](TopoDS_Shape shape) {
       //TopoDS_Shape shape1 = BRepPrimAPI_MakeBox(5, 12, 5).Shape();
       //changeShape();
       OCCGeometry og(shape);
       view->repaint(og);
   };
}

PCB3D_1::~PCB3D_1()
{
    delete pyconsole;
}

