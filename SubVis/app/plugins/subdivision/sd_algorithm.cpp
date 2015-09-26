
#include "plugins/subdivision/debug.h"
#include "plugins/subdivision/sd_algorithm.h"

namespace subdivision {

using surface_mesh::Surface_mesh;
using surface_mesh::Point;

SdAlgorithm::~SdAlgorithm() {
}

std::unique_ptr<Surface_mesh> SdAlgorithm::subdivide(
  const Surface_mesh& mesh,
  int steps) {
  result_mesh_.reset(new Surface_mesh);
  input_mesh_.reset(new Surface_mesh{mesh});

  for (int i = 0; i < steps; i++) {
    result_mesh_->clear();
    DEBUG_MESH(*input_mesh_.get(), "input mesh")
    subdivide_input_mesh_write_output_mesh();
    DEBUG_MESH(*result_mesh_.get(), "result mesh")
    // input mesh is now the previous result mesh
    input_mesh_.reset(new Surface_mesh{ *result_mesh_.get()});
  }

  // free memory
  input_mesh_.reset(nullptr);

  return std::move(result_mesh_);
}

const Surface_mesh& SdAlgorithm::get_result_mesh() {
  return *result_mesh_.get();
}

void SdAlgorithm::init_mesh_members() {
  v_points_ = input_mesh_->get_vertex_property<Point>(kPropVertexPoint);
}

void SdAlgorithm::deinit_mesh_members() {
  // vertex point property with key kPropVertexPoint is maintained by default
}

void SdAlgorithm::compute_mid_edge(Point& mid_edge,
                                   const Surface_mesh::Edge& edge) const {
  const Surface_mesh::Vertex edge_vertex0 = input_mesh_->vertex(edge, 0);
  const Surface_mesh::Vertex edge_vertex1 = input_mesh_->vertex(edge, 1);
  mid_edge = v_points_[edge_vertex0] + v_points_[edge_vertex1];
  mid_edge /= 2.;
}

} // namespace subdivision
