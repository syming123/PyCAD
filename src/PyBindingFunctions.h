/*
* 进行Python绑定
* 基于pybind11内嵌模块
*/

#ifndef PYBINDINGFUNCTIONS_H
#define PYBINDINGFUNCTIONS_H

#include <iostream>
#include <sstream>
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#include "PySharing.h"

#include <TopoDS.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <TopExp_Explorer.hxx>
#include <STEPControl_Reader.hxx>
#include <STEPControl_Writer.hxx>

#include "OCCGeometry.h"

namespace py = pybind11;

class PyConsoleOut
{
public:
    void write(std::string str)
    {
        //std::cout << "console out:" << str << std::endl;
        PySharing::printOut(str);
    }
};

PYBIND11_EMBEDDED_MODULE(pyconsole, m)
{
    py::class_<PyConsoleOut>(m, "OUT")
        .def(py::init<>())
        .def("write", &PyConsoleOut::write);
    m.attr("out") = PyConsoleOut();
}


PYBIND11_EMBEDDED_MODULE(gui, m)
{
    m.def("exit", []() {
        exit(0);
    });
    m.def("show", [](TopoDS_Shape shape) {
        PySharing::showOCCShape(shape);
    });
}

PYBIND11_EMBEDDED_MODULE(occ, m)
{
    py::class_<TopoDS_Shape>(m, "TopoDS_Shape");

    m.def("makeBox", [](float x, float y, float z) {
        TopoDS_Shape shape = BRepPrimAPI_MakeBox(x,y,z).Shape();
        return shape;
    });
    m.def("makeCylinder", [](float x, float y) {
        TopoDS_Shape shape = BRepPrimAPI_MakeCylinder(x,y).Shape();
        return shape;
    });
    m.def("makeWedgex", [](float x0, float y0, float x1, float y1) {
        TopoDS_Shape shape = BRepPrimAPI_MakeWedge(x0,y0,x1,y1).Shape();
        return shape;
    });

    m.def("readFile", [](std::string path) {
        STEPControl_Reader reader;
        reader.ReadFile(path.c_str());
        Standard_Integer NbRoots = reader.NbRootsForTransfer();
        Standard_Integer NbTrans = reader.TransferRoots();
        TopoDS_Shape result = reader.OneShape();
        return result;
    });

    m.def("getInfo", [](TopoDS_Shape shape) {
        OCCGeometry og(shape);
        PySharing::printOut("vertices:");
        for (int i = 0; i < og.vertices->size(); i++) {
            std::stringstream ss;
            int x = og.vertices->at(i).x;
            int y = og.vertices->at(i).y;
            int z = og.vertices->at(i).z;
            ss << "(" << x << "," << y << "," << z << ")";
            PySharing::printOut(ss.str());
        }
        PySharing::printOut("indexes:");
        for (int i = 0; i < og.indexes->size(); i++) {
            std::stringstream ss;
            int idx = og.indexes->at(i);
            ss << idx;
            PySharing::printOut(ss.str());
        }
        PySharing::printOut("normals:");
        for (int i = 0; i < og.normals->size(); i++) {
            std::stringstream ss;
            int x = og.normals->at(i).x;
            int y = og.normals->at(i).y;
            int z = og.normals->at(i).z;
            ss << "(" << x << "," << y << "," << z << ")";
            PySharing::printOut(ss.str());
        }
    });
}

#endif
