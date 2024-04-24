#include"OCCGeometry.h"


#include <BRepMesh_IncrementalMesh.hxx>
#include <TopTools_IndexedMapOfShape.hxx>
#include<TopExp.hxx>
#include<TopoDS.hxx>
#include<BRepAlgoAPI_Cut.hxx>
#include <STEPControl_Writer.hxx>
#include <STEPControl_Reader.hxx>
#include<IFSelect_ReturnStatus.hxx>

OCCGeometry::OCCGeometry() {
    type = BOX;
    //makeShape();
    gp_Trsf trsf;
    trsf.SetValues(1, 0, 0, 3,
        0, 1, 0, 3,
        0, 0, 1, 0);

    TopLoc_Location L(trsf);
    Shape = BRepPrimAPI_MakeBox(50, 50, 50).Shape();
    //Shape = BRepPrimAPI_MakeCylinder(5, 5).Shape();
    //Shape = BRepPrimAPI_MakeWedge(10, 8, 4, 5).Shape();

    createMesh();
};

OCCGeometry::OCCGeometry(TopoDS_Shape shape) {
    type = BOX;
    //makeShape();
    gp_Trsf trsf;
    trsf.SetValues(1, 0, 0, 3,
        0, 1, 0, 3,
        0, 0, 1, 0);

    TopLoc_Location L(trsf);
    Shape = shape;

    createMesh();
};

void OCCGeometry::changeShapeType(enum ShapeType thetype) {
	type = thetype;
    makeShape();
    createMesh();
}

void OCCGeometry::makeShape() {

	if (type == BOX) Shape = BRepPrimAPI_MakeBox(10, 12, 0.5);
	else if (type == Cylinder) Shape = BRepPrimAPI_MakeCylinder(2, 5);
}


TopoDS_Shape OCCGeometry::getShape() {
	return Shape;
}
void OCCGeometry::ShapeCut(TopoDS_Shape hole) {


    TopoDS_Shape ShapeCut = BRepAlgoAPI_Cut(Shape, hole);
    Shape = ShapeCut;
}

void OCCGeometry::createMesh() {//离散化
    BRepMesh_IncrementalMesh(Shape, Standard_True);

    Standard_Integer aNbTriangles(0);

    TopLoc_Location aLoc;
    Shape.Location(aLoc);

    TopTools_IndexedMapOfShape faceMap;
    TopExp::MapShapes(Shape, TopAbs_FACE, faceMap);
    int Triangnumber = 0;
    int Nodenumber = 0;

    //std::cout << faceMap.Extent() << std::endl;

    for (int i = 1; i <= faceMap.Extent(); i++)
    {
        Handle(Poly_Triangulation) aTriangulation = BRep_Tool::Triangulation(TopoDS::Face(faceMap(i)), aLoc);
        if (!aTriangulation.IsNull())
        {
            Nodenumber += aTriangulation->NbNodes();
            Triangnumber += aTriangulation->NbTriangles();
            //std::cout << aTriangulation->NbNodes() << ":" << aTriangulation->NbTriangles() << std::endl;
        }
    }

    vertices = vsg::vec3Array::create(Nodenumber);

    //normals = vsg::vec3Array::create(Nodenumber,vsg::vec3{0.0f,0.0f,1.0f});
    coordinates = vsg::vec2Array::create(Nodenumber);
    normals = vsg::vec3Array::create(Triangnumber * 3);
    indexes = vsg::uintArray::create(Triangnumber * 3);

    int verticesnumber = 0;
    int indexesnumber = 0;

    for (int i = 1; i <= faceMap.Extent(); i++)
    {

        Handle(Poly_Triangulation) aTriangulation = BRep_Tool::Triangulation(TopoDS::Face(faceMap(i)), aLoc);

        if (!aTriangulation.IsNull())
        {
            int number = aTriangulation->NbNodes();

            for (int k = 1; k <= number; k++)
            {

                vertices->at(verticesnumber) = vsg::vec3(float(aTriangulation->Node(k).Transformed(aLoc).X()),
                    float(aTriangulation->Node(k).Transformed(aLoc).Y()),
                    float(aTriangulation->Node(k).Transformed(aLoc).Z()));

                /* normals->at(verticesnumber)  = vsg::vec3(float( aTriangulation->Normal(k).X()),
                                                        float( aTriangulation->Normal(k).Y()),
                                                        float( aTriangulation->Normal(k).Z()));*/
                verticesnumber++;
            }

            for (Standard_Integer j = 1; j <= aTriangulation->NbTriangles(); j++)
            {

                gp_Pnt Vertex;
                const Poly_Triangle trian = aTriangulation->Triangle(j); // 获取第i个三角形
                Standard_Integer index1, index2, index3;
                trian.Get(index1, index2, index3);
                int finallindex1 = verticesnumber + index1 - number - 1;
                int finallindex2 = verticesnumber + index2 - number - 1;
                int finallindex3 = verticesnumber + index3 - number - 1;


                indexes->at(indexesnumber++) = verticesnumber + index1 - number - 1;
                indexes->at(indexesnumber++) = verticesnumber + index2 - number - 1;
                indexes->at(indexesnumber++) = verticesnumber + index3 - number - 1;

                vsg::vec3 n = vsg::normalize(vsg::cross(vertices->at(finallindex2) - vertices->at(finallindex3), vertices->at(finallindex1) - vertices->at(finallindex3)));


                normals->at(indexesnumber - 3) = n;
                normals->at(indexesnumber - 2) = n;
                normals->at(indexesnumber - 1) = n;

             
          
             
            }
        }
    }

}



void OCCGeometry::createStepFile() {
 
    STEPControl_Writer writer;
    writer.Transfer(Shape, STEPControl_ShellBasedSurfaceModel);
    const Standard_CString& aFileName = "C:/Users/lxf11/Desktop/test.stp";
    if (writer.Write(aFileName))
        std::cout << "stp文件输出成功";

}