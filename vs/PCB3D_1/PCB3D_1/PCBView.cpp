#include "PCBView.h"


class info :public vsg::Visitor {

    void apply(Object& object) override
    {
        object.traverse(*this);
    }
    void apply(vsg::RasterizationState& rs) override

    {
    //    rs.polygonMode = VK_POLYGON_MODE_LINE;
        rs.cullMode = VK_CULL_MODE_NONE;
    }
};

void PCBView::createPipline(OCCGeometry geometry) {
    scene = vsg::Group::create();
    shader = vsg::ShaderSet::create();
    shader = readShader("../res/shaders/standard.vert", "../res/shaders/standard_phong.frag");
    vsg::ref_ptr<vsg::vec4Value> default_color;
    vsg::ref_ptr<vsg::PhongMaterialValue> default_material;

  
    default_color = vsg::vec4Value::create(vsg::vec4{ 0.9, 0.9, 0.9, 1.0 });
    default_material = vsg::PhongMaterialValue::create();
    default_material->value().ambient.set(0.9, 0.9, 0.9, 1.0);
    default_material->value().diffuse.set(0.55, 0.55, 0.55, 1.0);
    default_material->value().specular.set(0.7, 0.7, 0.7, 1.0);
    //std::cout << mat->value().shininess;//默认值为100
    default_material->value().shininess = 35;


    //default pipeline
    auto graphicsPipelineConfig = vsg::GraphicsPipelineConfigurator::create(shader);
    vsg::DataList vertexArrays;
    auto drawCommands = vsg::Commands::create();

    //传入模型几何参数
    graphicsPipelineConfig->assignArray(vertexArrays, "vsg_Vertex", VK_VERTEX_INPUT_RATE_VERTEX, geometry.vertices);
    graphicsPipelineConfig->assignArray(vertexArrays, "vsg_Normal", VK_VERTEX_INPUT_RATE_VERTEX, geometry.normals);
    //graphicsPipelineConfig->assignArray(vertexArrays, "vsg_TexCoord0", VK_VERTEX_INPUT_RATE_VERTEX, texcoords);

    graphicsPipelineConfig->assignArray(vertexArrays, "vsg_Color", VK_VERTEX_INPUT_RATE_INSTANCE, default_color);
    graphicsPipelineConfig->assignDescriptor("material", default_material);
    //绑定索引
    drawCommands->addChild(vsg::BindVertexBuffers::create(graphicsPipelineConfig->baseAttributeBinding, vertexArrays));

    //   drawCommands->addChild(vsg::BindIndexBuffer::create(indicesVector[protoIndex[instance->ProtoID()->str()]]));
    drawCommands->addChild(vsg::BindIndexBuffer::create(geometry.indexes));
    drawCommands->addChild(vsg::DrawIndexed::create(geometry.indexes->size(), 1, 0, 0, 0));

    graphicsPipelineConfig->init();
    info info0;
    graphicsPipelineConfig->accept(info0);
    stateGroup = vsg::StateGroup::create();
    graphicsPipelineConfig->copyTo(stateGroup);
    //stateGroup->add(graphicsPipelineConfig);
    stateGroup->addChild(drawCommands);


    
    scene->addChild(stateGroup);
    createWindow(scene);


}



vsg::ref_ptr<vsg::Group> PCBView:: createLight(vsg::ref_ptr<vsg::Group> vsgScene){
    auto group = vsg::Group::create();
    group->addChild(scene);
    vsg::ref_ptr<vsg::AmbientLight> ambientLight;
    ambientLight = vsg::AmbientLight::create();
    ambientLight->name = "ambient";
    ambientLight->color.set(1.0, 1.0, 1.0);
    ambientLight->intensity = 0.95;
    group->addChild(ambientLight);
    //directional lights
    auto directionalLight = vsg::DirectionalLight::create();
    directionalLight->name = "directional";
    directionalLight->color.set(1.0, 1.0, 1.0);
    directionalLight->intensity = 0.7;
    directionalLight->direction = vsg::normalize(vsg::vec3(0.9, 1.0, -1.0));
    group->addChild(directionalLight);

    auto directionalLight2 = vsg::DirectionalLight::create();
    directionalLight2->name = "2nd directional";
    directionalLight2->color.set(1.0, 1.0, 1.0);
    directionalLight2->intensity = 0.7;
    directionalLight2->direction = vsg::normalize(vsg::vec3(-1.9, -1.0, 1.0));
    group->addChild(directionalLight2);
    return group;
}



vsg::ref_ptr<vsg::ShaderSet> PCBView::readShader(const vsg::Path& vertFile, const vsg::Path& fragFile) {

    vsg::ref_ptr<vsg::ShaderSet> shaderSet;
    auto options = vsg::Options::create();
    auto phong_vertexShader = vsg::read_cast<vsg::ShaderStage>(vertFile, options);
    auto phong_fragShader = vsg::read_cast<vsg::ShaderStage>(fragFile, options);
    shaderSet = vsg::createPhongShaderSet();
    if (shaderSet && phong_vertexShader && phong_fragShader)
    {
        // replace shaders
        shaderSet->stages.clear();
        shaderSet->stages.push_back(phong_vertexShader);
        shaderSet->stages.push_back(phong_fragShader);

        // clear prebuilt variants
       shaderSet->variants.clear();

    }
    else
    {
        std::cout << "Could not create shaders." << std::endl;
    }

    return shaderSet;
}




void PCBView::createWindow(vsg::ref_ptr<vsg::Group> scene)
{
    //main window
    viewer = vsgQt::Viewer::create();
    auto windowTraits = vsg::WindowTraits::create();
    QWindow* parent = nullptr;
    window = new vsgQt::Window(viewer, windowTraits, parent);
    window->initializeWindow();

    // if this is the first window to be created, use its device for future window creation.
    if (!windowTraits->device) windowTraits->device = window->windowAdapter->getOrCreateDevice();

    // compute the bounds of the scene graph to help position camera
    uint32_t width = window->traits->width;
    uint32_t height = window->traits->height;
    vsg::ComputeBounds computeBounds;
    scene->accept(computeBounds);
    vsg::dvec3 centre = (computeBounds.bounds.min + computeBounds.bounds.max) * 0.5;
    double radius = vsg::length(computeBounds.bounds.max - computeBounds.bounds.min) * 0.6;
    double nearFarRatio = 0.001;

    //build main camera

    auto lookAt = vsg::LookAt::create(centre + vsg::dvec3(0.0, -radius * 3.5, 0.0), centre, vsg::dvec3(0.0, 0.0, 1.0));
    vsg::ref_ptr<vsg::ProjectionMatrix> perspective;

    perspective = vsg::Perspective::create(
        30.0,
        static_cast<double>(width) /
        static_cast<double>(height),
        nearFarRatio * radius, radius * 100.0);

    auto camera = vsg::Camera::create(perspective, lookAt, vsg::ViewportState::create(VkExtent2D{ width, height }));


    //light
    scene = createLight(scene);

    auto view = vsg::View::create(camera, scene);
    auto renderGraph = vsg::RenderGraph::create(*window, view);
    renderGraph->clearValues[0].color = { {0.9f, 0.9f, 0.9f, 1.0f} }; //background color
    auto commandGraph = vsg::CommandGraph::create(*window, renderGraph);
    commandGraph->addChild(renderGraph);

    viewer->addRecordAndSubmitTaskAndPresentation({ commandGraph });

    //add trackball event
    auto trackball = vsg::Trackball::create(camera);
    trackball->addWindow(*window);
    viewer->addEventHandler(trackball);

    viewer->compile();
}


void PCBView::repaint(OCCGeometry occgeo) {
    vsg::ref_ptr<vsg::vec4Value> default_color;
    vsg::ref_ptr<vsg::PhongMaterialValue> default_material;


    default_color = vsg::vec4Value::create(vsg::vec4{ 0.9, 0.9, 0.9, 1.0 });
    default_material = vsg::PhongMaterialValue::create();
    default_material->value().ambient.set(0.9, 0.9, 0.9, 1.0);
    default_material->value().diffuse.set(0.55, 0.55, 0.55, 1.0);
    default_material->value().specular.set(0.7, 0.7, 0.7, 1.0);
    default_material->value().shininess = 35;

    //执行重新绘制的代码----------------------------------------------------------------
    auto graphicsPipelineConfig = vsg::GraphicsPipelineConfigurator::create(shader);
    vsg::DataList vertexArrays;
    //传入模型几何参数
    graphicsPipelineConfig->assignArray(vertexArrays, "vsg_Vertex", VK_VERTEX_INPUT_RATE_VERTEX, occgeo.vertices);
    graphicsPipelineConfig->assignArray(vertexArrays, "vsg_Normal", VK_VERTEX_INPUT_RATE_VERTEX, occgeo.normals);
    graphicsPipelineConfig->assignArray(vertexArrays, "vsg_Color", VK_VERTEX_INPUT_RATE_INSTANCE, default_color);
    graphicsPipelineConfig->assignDescriptor("material", default_material);
    graphicsPipelineConfig->init();
    graphicsPipelineConfig->copyTo(stateGroup);
    //绑定索引
    auto drawCommands = vsg::Commands::create();
    drawCommands->addChild(vsg::BindVertexBuffers::create(graphicsPipelineConfig->baseAttributeBinding, vertexArrays));
    drawCommands->addChild(vsg::BindIndexBuffer::create(occgeo.indexes));
    drawCommands->addChild(vsg::DrawIndexed::create(occgeo.indexes->size(), 1, 0, 0, 0));

    info info0;
    graphicsPipelineConfig->accept(info0);

    stateGroup->children[0] = drawCommands;


    viewer->compile();
}