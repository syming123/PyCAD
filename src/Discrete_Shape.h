/*
* 使用OCC对几何体进行离散化
*/

#ifndef OCCGEOMETRY_H
#define OCCGEOMETRY_H

#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeWedge.hxx>
#include <gp_Trsf.hxx>
#include <TopLoc_Location.hxx>
#include <BRepMesh_IncrementalMesh.hxx>
#include <TopTools_IndexedMapOfShape.hxx>
#include <TopExp.hxx>
#include <TopoDS.hxx>
#include <BRepAlgoAPI_Cut.hxx>
#include <STEPControl_Writer.hxx>
#include <STEPControl_Reader.hxx>
#include <IFSelect_ReturnStatus.hxx>

#include <vector>

#include <vsg/all.h>

class Discrete_Shape 
{
public: 
	Discrete_Shape() {};
	Discrete_Shape(TopoDS_Shape shape);
	~Discrete_Shape() {};

	void discretize(TopoDS_Shape shape);
	
	vsg::ref_ptr <vsg::vec3Array> vertices;
	vsg::ref_ptr <vsg::vec2Array> coordinates;
	vsg::ref_ptr <vsg::vec3Array> normals;
	vsg::ref_ptr <vsg::uintArray> indexes;
};

#endif