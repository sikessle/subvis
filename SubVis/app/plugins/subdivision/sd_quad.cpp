
#include "plugins/subdivision/debug.h"
#include "plugins/subdivision/sd_quad.h"

namespace subdivision {

void SdQuad::init_mesh_members() {
  SdAlgorithm::init_mesh_members();
  // add properties to mesh
  input_mesh_->add_face_property<Point>(kPropFacePoint);
  input_mesh_->add_edge_property<Point>(kPropEdgePoint);
  input_mesh_->add_vertex_property<Point>(kPropVertexPointUpdated);
  // init members
  e_points_ = input_mesh_->get_edge_property<Point>(kPropEdgePoint);
  f_points_ = input_mesh_->get_face_property<Point>(kPropFacePoint);
  v_points_updated_ = input_mesh_->get_vertex_property<Point>
                      (kPropVertexPointUpdated);
}

void SdQuad::deinit_mesh_members() {
  SdAlgorithm::deinit_mesh_members();
  input_mesh_->remove_face_property(f_points_);
  input_mesh_->remove_edge_property(e_points_);
  input_mesh_->remove_vertex_property(v_points_updated_);
}

void SdQuad::compute_face_point(Point& face_point,
                                const Surface_mesh::Face& face) {
  face_point = Point(0);
  int i = 0;
  for (const auto& vertex : input_mesh_->vertices(face)) {
    face_point += v_points_[vertex];
    ++i;
  }
  if (i != 0) {
    face_point /= i;
  }
}

} // namespace subdivision

