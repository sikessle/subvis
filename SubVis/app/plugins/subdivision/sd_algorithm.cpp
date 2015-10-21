
#include "plugins/subdivision/debug.h"
#include "plugins/subdivision/sd_algorithm.h"

namespace subdivision {

using Surface_mesh = surface_mesh::Surface_mesh;
using Point = surface_mesh::Point;

SdAlgorithm::~SdAlgorithm() {
}

void SdAlgorithm::subdivide_threaded(const Surface_mesh& mesh,
  std::function<void(std::unique_ptr<Surface_mesh>)> callback,
  int steps) {
  output_mesh_.reset(new Surface_mesh);
  input_mesh_.reset(new Surface_mesh{mesh});
  callback_ = callback;

  for (int i = 0; i < steps; i++) {
    output_mesh_->clear();
    DEBUG_MESH(*input_mesh_.get(), "input mesh")
    subdivide_input_mesh_write_output_mesh();
    DEBUG_MESH(*output_mesh_.get(), "result mesh")
    // input mesh is now the previous result mesh
    input_mesh_.reset(new Surface_mesh{ *output_mesh_.get()});
  }

  // free memory
  input_mesh_.reset(nullptr);

  callback_(std::move(output_mesh_));
}

const Surface_mesh& SdAlgorithm::get_result_mesh() {
  return *output_mesh_.get();
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

void SdAlgorithm::compute_new_boundary_vertex_coordinate(
  Point& new_vertex_point, const Surface_mesh::Vertex& vertex) const {
  // check if vertex belongs to a face
  if (input_mesh_->valence(vertex) > 0) {
    const Surface_mesh::Halfedge h0 = find_halfedge_of_boundary_edge_ccw(vertex);
    const Surface_mesh::Halfedge h1 = find_next_halfedge_of_boundary_edge_ccw(h0);
    new_vertex_point = 3. / 4. * v_points_[vertex] + 1. / 8. *
                       (v_points_[input_mesh_->to_vertex(h0)] + v_points_[input_mesh_->to_vertex(h1)]);
  } else {
    new_vertex_point = v_points_[vertex];
  }
}

Surface_mesh::Halfedge SdAlgorithm::get_valid_halfedge_of_boundary_edge(
  const Surface_mesh::Edge& edge) const {
  return input_mesh_->is_boundary(input_mesh_->halfedge(edge,
                                  0)) ? input_mesh_->halfedge(edge, 1) : input_mesh_->halfedge(edge, 0);
}

Surface_mesh::Halfedge SdAlgorithm::find_halfedge_of_boundary_edge_ccw(
  const Surface_mesh::Vertex& vertex) const {
  return this->find_next_halfedge_of_boundary_edge_ccw(input_mesh_->halfedge(
           vertex));
}

Surface_mesh::Halfedge SdAlgorithm::find_next_halfedge_of_boundary_edge_ccw(
  const Surface_mesh::Halfedge start_halfedge) const {
  Surface_mesh::Halfedge rotated_halfedge = start_halfedge;
  do {
    rotated_halfedge = input_mesh_->ccw_rotated_halfedge(rotated_halfedge);
    if (input_mesh_->is_boundary(input_mesh_->edge(rotated_halfedge))) {
      // boundary found
      return rotated_halfedge;
    }
  } while (rotated_halfedge != start_halfedge);
  throw std::runtime_error("No boundary edge.");
}

} // namespace subdivision
