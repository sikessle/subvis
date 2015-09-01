
#include "plugins/subdivision/debug.h"
#include "plugins/subdivision/sd_algorithm.h"

namespace subdivision {

using surface_mesh::Surface_mesh;
using surface_mesh::Point;

SubdivAlgorithm::~SubdivAlgorithm() {

}

std::unique_ptr<Surface_mesh> SubdivAlgorithm::subdivide(Surface_mesh& mesh,
    int steps) {
  result_mesh_.reset(new Surface_mesh);
  input_mesh_.reset(new Surface_mesh{mesh});

  for (int i = 0; i < steps; i++) {
    result_mesh_->clear();
    DEBUG_MESH(*input_mesh_.get(), "input mesh")
    subdivide_specific_algorithm();
    DEBUG_MESH(*result_mesh_.get(), "result mesh")
    // input mesh is now the previous result mesh
    input_mesh_.reset(new Surface_mesh{ * (result_mesh_.get())});
  }

  // free memory
  input_mesh_.reset(nullptr);

  return std::move(result_mesh_);
}

Surface_mesh& SubdivAlgorithm::result_mesh() {
  return *(result_mesh_.get());
}

void SubdivAlgorithm::init_mesh_members() {
  // add properties to mesh
  input_mesh_->add_face_property<Point>(kPropFacePoint);
  input_mesh_->add_edge_property<Point>(kPropEdgePoint);
  input_mesh_->add_vertex_property<Point>(kPropVertexPointUpdated);
  // (vertex point property with key kPropVertexPoint is maintained by default)
  // init members
  v_points_ = input_mesh_->get_vertex_property<Point>(kPropVertexPoint);
  e_points_ = input_mesh_->get_edge_property<Point>(kPropEdgePoint);
  f_points_ = input_mesh_->get_face_property<Point>(kPropFacePoint);
  v_points_updated_ = input_mesh_->get_vertex_property<Point>
                      (kPropVertexPointUpdated);
}

void SubdivAlgorithm::deinit_mesh_members() {
  input_mesh_->remove_face_property(f_points_);
  input_mesh_->remove_edge_property(e_points_);
  input_mesh_->remove_vertex_property(v_points_updated_);
}

void SubdivAlgorithm::compute_face_point(Point& face_point,
    const Surface_mesh::Face& face) {
  // init result with zero
  face_point = Point(0);
  // initialize circulators
  Surface_mesh::Vertex_around_face_circulator vc = input_mesh_->vertices(face);
  int i = 0;
  for (const Surface_mesh::Vertex& v : vc) {
    face_point += v_points_[v];
    ++i;
  }
  if (i != 0) {
    face_point /= i;
  }
}

void SubdivAlgorithm::compute_mid_edge(Point& mid_edge,
                                       const Surface_mesh::Edge& edge) {
  Surface_mesh::Vertex edge_vertex0, edge_vertex1;
  edge_vertex0 = input_mesh_->vertex(edge, 0);
  edge_vertex1 = input_mesh_->vertex(edge, 1);
  mid_edge = v_points_[edge_vertex0] + v_points_[edge_vertex1];
  mid_edge /= 2;
}

} // namespace subdivision
