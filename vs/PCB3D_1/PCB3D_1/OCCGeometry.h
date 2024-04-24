#pragma once
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeWedge.hxx>
#include<gp_Trsf.hxx>
#include<TopLoc_Location.hxx>


#include<vector>

#include<vsg/all.h>

typedef enum ShapeType
{
	LINE = 1,
	BOX = 2,
	Cylinder = 3,
	Wedge=4
}ShapeType;



class OCCGeometry {
private:
	TopoDS_Shape Shape;
	enum ShapeType type;

public: 
	OCCGeometry();
	OCCGeometry(TopoDS_Shape shape);
	~OCCGeometry() {};

	void changeShapeType(enum ShapeType thetype);
	void makeShape();
	TopoDS_Shape getShape();
	void ShapeCut(TopoDS_Shape hole);
	void createMesh();
	void createStepFile();
	
	
	vsg::ref_ptr<vsg::vec3Array> vertices;
	vsg::ref_ptr < vsg::vec2Array> coordinates;
	vsg::ref_ptr < vsg::vec3Array> normals;
	vsg::ref_ptr < vsg::uintArray> indexes;
};