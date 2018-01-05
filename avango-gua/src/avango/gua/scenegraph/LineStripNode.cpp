#include <avango/gua/scenegraph/LineStripNode.hpp>

#if defined(AVANGO_DISTRIBUTION_SUPPORT)
#include <avango/gua/network/NetTransform.h>
#endif

#include <avango/Base.h>
#include <functional>

AV_FC_DEFINE(av::gua::LineStripNode);

AV_FIELD_DEFINE(av::gua::SFLineStripNode);
AV_FIELD_DEFINE(av::gua::MFLineStripNode);

av::gua::LineStripNode::LineStripNode(std::shared_ptr< ::gua::node::LineStripNode> guanode)
    : GeometryNode(guanode)
    , m_guaLineStripNode(guanode)
{
  AV_FC_ADD_ADAPTOR_FIELD(Geometry,
                        std::bind(&LineStripNode::getGeometryCB, this,std::placeholders::_1),
                        std::bind(&LineStripNode::setGeometryCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(Material,
                      std::bind(&LineStripNode::getMaterialCB, this,std::placeholders::_1),
                      std::bind(&LineStripNode::setMaterialCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(RenderToGBuffer,
                      std::bind(&LineStripNode::getRenderToGBufferCB, this,std::placeholders::_1),
                      std::bind(&LineStripNode::setRenderToGBufferCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(RenderToStencilBuffer,
                      std::bind(&LineStripNode::getRenderToStencilBufferCB, this,std::placeholders::_1),
                      std::bind(&LineStripNode::setRenderToStencilBufferCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(RenderVolumetric,
                      std::bind(&LineStripNode::getRenderVolumetricCB, this,std::placeholders::_1),
                      std::bind(&LineStripNode::setRenderVolumetricCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(RenderAsPoints,
                      std::bind(&LineStripNode::getRenderAsPointsCB, this,std::placeholders::_1),
                      std::bind(&LineStripNode::setRenderAsPointsCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(ScreenSpaceLineWidth,
                      std::bind(&LineStripNode::getScreenSpaceLineWidthCB, this,std::placeholders::_1),
                      std::bind(&LineStripNode::setScreenSpaceLineWidthCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(WasCreatedEmpty,
                      std::bind(&LineStripNode::getWasCreatedEmptyCB, this,std::placeholders::_1),
                      std::bind(&LineStripNode::setWasCreatedEmptyCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(Vertices,
                      std::bind(&LineStripNode::getVerticesCB, this,std::placeholders::_1),
                      std::bind(&LineStripNode::setVerticesCB, this,std::placeholders::_1));




  if (guanode->get_material()) {
    m_Material = av::Link<av::gua::Material>(new av::gua::Material(guanode->get_material()));
  }


  m_Vertex = av::Link<av::gua::LineStripVertex>(new av::gua::LineStripVertex());
  

}

void av::gua::LineStripNode::clearVertices() const
{
  m_guaLineStripNode->clear_vertices();
}

void av::gua::LineStripNode::popBackVertex() const
{
  m_guaLineStripNode->pop_back_vertex();
}

void av::gua::LineStripNode::popFrontVertex() const
{
  m_guaLineStripNode->pop_front_vertex();
}

void av::gua::LineStripNode::pushVertex(float x_pos, float y_pos, float z_pos,
                                        float col_r, float col_g, float col_b, float col_a,
                                        float thickness) const
{
  m_guaLineStripNode->push_vertex(x_pos, y_pos, z_pos,
                                  col_r, col_g, col_b, col_a,
                                  thickness);
}


#if defined(AVANGO_DISTRIBUTION_SUPPORT)

void av::gua::LineStripNode::on_distribute(av::gua::NetTransform& netNode) 
{
    GeometryNode::on_distribute(netNode);

    if (m_Material.isValid()) {
      m_Material->on_distribute(netNode);
    }

    netNode.distributeFieldContainer(m_Material);
    netNode.distributeFieldContainer(m_Vertex);
}

void av::gua::LineStripNode::on_undistribute(av::gua::NetTransform& netNode) 
{
    GeometryNode::on_undistribute(netNode);

    if (m_Material.isValid()) {
      m_Material->on_undistribute(netNode);
    }

    netNode.undistributeFieldContainer(m_Material);
    netNode.undistributeFieldContainer(m_Vertex);
}
#endif

void
av::gua::LineStripNode::initClass()
{
  if (!isTypeInitialized()) {
    av::gua::GeometryNode::initClass();

    AV_FC_INIT(av::gua::GeometryNode, av::gua::LineStripNode, true);

    SFLineStripNode::initClass("av::gua::SFLineStripNode", "av::Field");
    MFLineStripNode::initClass("av::gua::MFLineStripNode", "av::Field");

    sClassTypeId.setDistributable(true);
  }
}

void
av::gua::LineStripNode::getGeometryCB(const SFString::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaLineStripNode->get_geometry_description();
}

void
av::gua::LineStripNode::setGeometryCB(const SFString::SetValueEvent& event)
{
  m_guaLineStripNode->set_geometry_description(event.getValue());
}

void
av::gua::LineStripNode::getMaterialCB(const SFMaterial::GetValueEvent& event)
{
  if (m_Material.isValid()) {
    *(event.getValuePtr()) = m_Material;
  }
}

void
av::gua::LineStripNode::setMaterialCB(const SFMaterial::SetValueEvent& event)
{
  if (event.getValue().isValid()) {
    m_Material = event.getValue();
    m_guaLineStripNode->set_material(m_Material->getGuaMaterial());
  }
}

void
av::gua::LineStripNode::getRenderToGBufferCB(const SFBool::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaLineStripNode->get_render_to_gbuffer();
}

void
av::gua::LineStripNode::setRenderToGBufferCB(const SFBool::SetValueEvent& event)
{
  m_guaLineStripNode->set_render_to_gbuffer(event.getValue());
}

void
av::gua::LineStripNode::getRenderToStencilBufferCB(const SFBool::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaLineStripNode->get_render_to_stencil_buffer();
}

void
av::gua::LineStripNode::setRenderToStencilBufferCB(const SFBool::SetValueEvent& event)
{
  m_guaLineStripNode->set_render_to_stencil_buffer(event.getValue());
}

void av::gua::LineStripNode::getRenderVolumetricCB(const SFBool::GetValueEvent& event)
{
  //*(event.getValuePtr()) = m_guaLineStripNode->get_render_volumetric();
}

void av::gua::LineStripNode::setRenderVolumetricCB(const SFBool::SetValueEvent& event)
{
  m_guaLineStripNode->set_render_volumetric(event.getValue());
}

void av::gua::LineStripNode::getRenderAsPointsCB(const SFBool::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaLineStripNode->get_render_vertices_as_points();
}

void av::gua::LineStripNode::setRenderAsPointsCB(const SFBool::SetValueEvent& event)
{
  m_guaLineStripNode->set_render_vertices_as_points(event.getValue());
}

void av::gua::LineStripNode::getScreenSpaceLineWidthCB(const SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaLineStripNode->get_screen_space_line_width();
}

void av::gua::LineStripNode::setScreenSpaceLineWidthCB(const SFFloat::SetValueEvent& event)
{
  m_guaLineStripNode->set_screen_space_line_width(event.getValue());
}

void av::gua::LineStripNode::getWasCreatedEmptyCB(const SFBool::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaLineStripNode->get_was_created_empty();
}

void av::gua::LineStripNode::setWasCreatedEmptyCB(const SFBool::SetValueEvent& event)
{
  m_guaLineStripNode->set_was_created_empty(event.getValue());
}

void av::gua::LineStripNode::getVerticesCB(const SFLineStripVertex::GetValueEvent& event)
{
  if (m_Vertex.isValid()) {
    *(event.getValuePtr()) = m_Vertex;
  }
}

void av::gua::LineStripNode::setVerticesCB(const SFLineStripVertex::SetValueEvent& event)
{

  
  if (event.getValue().isValid()) {
    m_Vertex = event.getValue();

    std::cout << "READY!!!!\n";

    //auto is_it = m_Vertex->getGuaLineStripVertex();

    //m_guaLineStripNode->set_material(m_Material->getGuaMaterial());
  } else {
    std::cout << "NOT READY!!!!\n";
  }

/*
  std::cout << "CALLED THE EVIL SET VERTICES!!!\n";

  m_guaLineStripNode->clear_vertices();

  auto bla = event.getValue();


  *bla;*/
/*
  for (auto line_strip_vertex : event.getValue()) {

    if(!line_strip_vertex.isValid()) {
      std::cout << "LINESTRIPVERTEX IS NULLPTR!!!!\n";
    } else {
      auto gua_ls_vertex = line_strip_vertex->getGuaLineStripVertex();
    }
                                            //getGuaLineStripVertex()
    //m_guaLineStripNode->push_vertex(*gua_ls_vertex);

    m_guaLineStripNode->push_vertex({1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f});
    m_guaLineStripNode->push_vertex({-1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f});
  }
*/
  //m_guaLineStripNode->set_was_created_empty(event.getValue());
}

std::shared_ptr< ::gua::node::LineStripNode>
av::gua::LineStripNode::getGuaLineStripNode() const {
  return m_guaLineStripNode;
}