#pragma once

#include "PySharing.h"

//vsg
#include <vsgQt/Window.h>
#include<vsg/all.h>
//occ
#include "OCCGeometry.h"



class PCBView {

public:
	vsgQt::Window *window;
	
	void createWindow(vsg::ref_ptr<vsg::Group> scene);
	void createPipline(OCCGeometry geometry);
	vsg::ref_ptr<vsg::Group> createLight(vsg::ref_ptr<vsg::Group> vsgScene);
	vsg::ref_ptr<vsg::ShaderSet> readShader(const vsg::Path& vertFile, const vsg::Path& fragFile);
	void repaint(OCCGeometry occgeo);


	
private:

	vsg::ref_ptr<vsg::ShaderSet> shader;
	vsg::ref_ptr<vsg::Group> scene;
	vsg::ref_ptr<vsgQt::Viewer> viewer;
	vsg::ref_ptr<vsg::StateGroup> stateGroup;


};