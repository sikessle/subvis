
#include "plugins/subdivision/debug.h"
#include "plugins/subdivision/sd_loop.h"

namespace subdivision {

void SubdivLoop::subdivide_specific_algorithm() {
  this->add_mesh_properties();
  this->init_mesh_members();


  this->remove_mesh_properties();
}

void SubdivLoop::add_mesh_properties() {
  input_mesh_->add_edge_property<Point>(kPropEdgePoint);
  input_mesh_->add_vertex_property<Point>(kPropVertexPointUpdated);
}

void SubdivLoop::init_mesh_members() {
  SubdivAlgorithm::init_mesh_members();
  e_points_ = input_mesh_->get_edge_property<Point>(kPropEdgePoint);
  v_points_updated_ = input_mesh_->get_vertex_property<Point>
                      (kPropVertexPointUpdated);
}

void SubdivLoop::remove_mesh_properties() {
  input_mesh_->remove_edge_property(e_points_);
  input_mesh_->remove_face_property(v_points_updated_);
}


} // namespace subdivision
