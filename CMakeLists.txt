cmake_minimum_required (VERSION 3.7)

project (PyCAD
    VERSION 0.1.0
    DESCRIPTION "demo"
    LANGUAGES CXX C
)


# set libraries path
set(QT_PATH C:/Qt/Qt5.12.12/5.12.12/msvc2017_64)
set(OCC_PATH C:/OpenCASCADE-7.7.0-vc14-64/opencascade-7.7.0)



# required cpp 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set project structure
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
aux_source_directory (${PROJECT_SOURCE_DIR}/src SRC_LIST)


# Qt
set(CMAKE_PREFIX_PATH ${QT_PATH}/lib/cmake)
find_package(Qt5 REQUIRED COMPONENTS Widgets)

set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)


# vulkan
find_package(Vulkan)
include_directories(${Vulkan_INCLUDE_DIR})

# vsg and vsgQt libs
include_directories(${PROJECT_SOURCE_DIR}/thirdParty/vsg/include)
include_directories(${PROJECT_SOURCE_DIR}/thirdParty/vsgQt/include)
link_directories(${PROJECT_SOURCE_DIR}/thirdParty/vsg/lib)
link_directories(${PROJECT_SOURCE_DIR}/thirdParty/vsgQt/lib)


# python
find_package(Python3 REQUIRED COMPONENTS Development)
include_directories(${Python3_INCLUDE_DIRS})

# pybind11
include_directories(${PROJECT_SOURCE_DIR}/thirdParty/pybind11/include)


# occ
set(CMAKE_PREFIX_PATH ${OCC_PATH})
find_package(OpenCASCADE REQUIRED)
include_directories(${OpenCASCADE_INCLUDE_DIR})
link_directories(${OpenCASCADE_LIBRARY_DIR})


# add executable
add_executable (${PROJECT_NAME} ${SRC_LIST})


# link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Widgets)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_link_libraries(${PROJECT_NAME} PRIVATE vsgQt.lib vsg.lib ${Vulkan_LIBRARY})
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE vsgQtd.lib vsgd.lib ${Vulkan_LIBRARY})
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE ${Python3_LIBRARIES})
#target_link_libraries(${PROJECT_NAME} PRIVATE ${OpenCASCADE_LIBRARIES})
target_link_libraries(${PROJECT_NAME} PRIVATE 
    TKBin.lib
    TKBinL.lib
    TKBinTObj.lib
    TKBinXCAF.lib
    TKBO.lib
    TKBool.lib
    TKBRep.lib
    TKCAF.lib
    TKCDF.lib
    TKDCAF.lib
    TKDraw.lib
    TKernel.lib
    TKFeat.lib
    TKFillet.lib
    TKG2d.lib
    TKG3d.lib
    TKGeomAlgo.lib
    TKGeomBase.lib
    TKHLR.lib
    TKIGES.lib
    TKLCAF.lib
    TKMath.lib
    TKMesh.lib
    TKMeshVS.lib
    TKOffset.lib
    TKOpenGl.lib
    TKOpenGlTest.lib
    TKPrim.lib
    TKQADraw.lib
    TKRWMesh.lib
    TKService.lib
    TKShHealing.lib
    TKStd.lib
    TKStdL.lib
    TKSTEP.lib
    TKSTEP209.lib
    TKSTEPAttr.lib
    TKSTEPBase.lib
    TKSTL.lib
    TKTObj.lib
    TKTObjDRAW.lib
    TKTopAlgo.lib
    TKTopTest.lib
    TKV3d.lib
    TKVCAF.lib
    TKViewerTest.lib
    TKVRML.lib
    TKXCAF.lib
    TKXDEDRAW.lib
    TKXDEIGES.lib
    TKXDESTEP.lib
    TKXMesh.lib
    TKXml.lib
    TKXmlL.lib
    TKXmlTObj.lib
    TKXmlXCAF.lib
    TKXSBase.lib
    TKXSDRAW.lib
)
