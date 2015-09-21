
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

void SdQuad::compute_mid_edge(Point& mid_edge,
                              const Surface_mesh::Edge& edge) {
  Surface_mesh::Vertex edge_vertex0, edge_vertex1;
  edge_vertex0 = input_mesh_->vertex(edge, 0);
  edge_vertex1 = input_mesh_->vertex(edge, 1);
  mid_edge = v_points_[edge_vertex0] + v_points_[edge_vertex1];
  mid_edge /= 2.;
}

} // namespace subdivision

