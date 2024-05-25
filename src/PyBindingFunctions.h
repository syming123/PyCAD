/*
* 进行Python绑定
* 基于pybind11内嵌模块
*/

#ifndef PYBINDINGFUNCTIONS_H
#define PYBINDINGFUNCTIONS_H

#include <iostream>
#include <sstream>
#include <vector>

#include <pybind11/stl.h>

#include "PythonConsole.h"
#include "Discrete_Shape.h"
#include "CADViewer.h"

#include <TopoDS.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>
#include <BRepAlgoAPI_Cut.hxx>
#include <BRepAlgoAPI_Common.hxx>
#include <BRepAlgoAPI_Fuse.hxx>
#include <TopExp_Explorer.hxx>
#include <STEPControl_Reader.hxx>
#include <STEPControl_Writer.hxx>

#include <vsg/all.h>

namespace py = pybind11;

PythonConsole* bindingPythonConsole;
CADViewer* cadViewer;

class PyConsoleOut
{
public:
    void write(std::string str)
    {
        //std::cout << "console out:" << str << std::endl;
        bindingPythonConsole->printOut(str);
    }
};

PYBIND11_EMBEDDED_MODULE(pyconsole, m)
{
    py::class_<PyConsoleOut>(m, "OUT")
        .def(py::init<>())
        .def("write", &PyConsoleOut::write);
    m.attr("out") = PyConsoleOut();
}

PYBIND11_EMBEDDED_MODULE(Gui, m)
{
    m.def("exit", []() {
        exit(0);
    });
}

PYBIND11_EMBEDDED_MODULE(OCCGeometry, m)
{
    py::class_<TopoDS_Shape>(m, "TopoDS_Shape");
    py::class_<TopoDS_Vertex>(m, "TopoDS_Vertex");
    py::class_<TopoDS_Edge>(m, "TopoDS_Edge");
    py::class_<TopoDS_Face>(m, "TopoDS_Face");

    // 建模
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
    m.def("makeSphere", [](float r) {
        TopoDS_Shape shape = BRepPrimAPI_MakeSphere(r).Shape();
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

    m.def("shapeCut", [](TopoDS_Shape shape1, TopoDS_Shape shape2) {
        TopoDS_Shape shape = BRepAlgoAPI_Cut(shape1, shape2);
        return shape;
    });
    m.def("shapeCommon", [](TopoDS_Shape shape1, TopoDS_Shape shape2) {
        TopoDS_Shape shape = BRepAlgoAPI_Common(shape1, shape2);
        return shape;
    });
    m.def("shapeFuse", [](TopoDS_Shape shape1, TopoDS_Shape shape2) {
        TopoDS_Shape shape = BRepAlgoAPI_Fuse(shape1, shape2);
        return shape;
    });

}

PYBIND11_EMBEDDED_MODULE(OCCTools, m)
{
    py::class_<Discrete_Shape>(m, "Discrete_Shape");

    // 离散化
    m.def("discretize", [](TopoDS_Shape shape) {
        Discrete_Shape dshape{};
        dshape.discretize(shape);  //该函数中实现了离散化的具体过程
        return dshape;
    });

    m.def("getVertices", [](Discrete_Shape dshape) {
        std::vector<std::vector<float>> vertices;
        for (int i = 0; i < dshape.vertices->size(); i++) {
            std::vector<float> vertice;
            vertice.push_back(dshape.vertices->at(i).x);
            vertice.push_back(dshape.vertices->at(i).y);
            vertice.push_back(dshape.vertices->at(i).z);
            vertices.push_back(vertice);
        }
        return vertices;
    });
    m.def("getNormals", [](Discrete_Shape dshape) {
        std::vector<std::vector<float>> normals;
        for (int i = 0; i < dshape.normals->size(); i++) {
            std::vector<float> normal;
            normal.push_back(dshape.normals->at(i).x);
            normal.push_back(dshape.normals->at(i).y);
            normal.push_back(dshape.normals->at(i).z);
            normals.push_back(normal);
        }
        return normals;
    });
    m.def("getIndexes", [](Discrete_Shape dshape) {
        std::vector<int> indexes;
        for (int i = 0; i < dshape.indexes->size(); i++) {
            indexes.push_back(dshape.indexes->at(i));
        }
        return indexes;
    });
}

PYBIND11_EMBEDDED_MODULE(Viewer, m)
{
    m.def("repaint", [](Discrete_Shape dshape) {
        // 该函数实现了在VSG中对输入的几何体进行渲染
        cadViewer->repaint(dshape);
    });
    m.def("setColor", [](float r, float g, float b, float a) {
        cadViewer->color = vsg::vec4Value::create(
            vsg::vec4{r,g,b,a}
        );
    });
 
    m.def("setMaterial", [](std::vector<float> ambient, std::vector<float> diffuse, std::vector<float> specular, float shininess) {
        cadViewer->material->value().ambient.set(ambient[0], ambient[1], ambient[2], ambient[3]);
        cadViewer->material->value().diffuse.set(diffuse[0], diffuse[1], diffuse[2], diffuse[3]);
        cadViewer->material->value().specular.set(specular[0], specular[1], specular[2], specular[3]);
        cadViewer->material->value().shininess = shininess;
    });
    // something else...
}

#endif
