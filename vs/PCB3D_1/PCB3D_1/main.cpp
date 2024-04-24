#include "PCB3D_1.h"
#include "PyBindingFunctions.h"
#include <QtWidgets/QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PCB3D_1 w;
    w.resize(1800, 1000);
    w.show();
    return a.exec();
}
