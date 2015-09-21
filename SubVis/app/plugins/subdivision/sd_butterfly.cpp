
#include <cmath>
#include "plugins/subdivision/debug.h"

#include "plugins/subdivision/sd_butterfly.h"

namespace subdivision {

void SdButterfly::subdivide_input_mesh_write_output_mesh() {
  this->init_mesh_members();
  this->compute_vertices_result();
  this->compute_all_edge_points();
  this->compute_all_faces();
  this->deinit_mesh_members();
}

void SdButterfly::compute_vertices_result() {
  for (const auto& vertex : input_mesh_->vertices()) {
    v_index_result_v_prop_[vertex] = result_mesh_->add_vertex(v_points_[vertex]);
  }
}

void SdButterfly::compute_all_edge_points() {
  Point edge_point;
  for (const auto& edge : input_mesh_->edges()) {
    this->compute_edge_point(edge_point, edge);
    v_index_result_e_prop_[edge] = result_mesh_->add_vertex(edge_point);
    DEBUG_POINT(edge_point, "New Interpolated Point");
  }
}

void SdButterfly::compute_edge_point(Point& edge_point,
                                     const Surface_mesh::Edge& edge) {
  // TODO override for modified butterfly
  this->compute_edge_point_ordinary(edge_point, edge);
}

void SdButterfly::compute_edge_point_ordinary(Point& edge_point,
    const Surface_mesh::Edge& edge) {
  edge_point = Point(0);
  Surface_mesh::Halfedge h0 = input_mesh_->halfedge(edge, 0);
  Surface_mesh::Halfedge h1 = input_mesh_->halfedge(edge, 1);
  Surface_mesh::Vertex a0 = input_mesh_->vertex(edge, 0);
  Surface_mesh::Vertex a1 = input_mesh_->vertex(edge, 1);
  Surface_mesh::Vertex b0 = this->get_outer_vertex_triangle(h0);
  Surface_mesh::Vertex b1 = this->get_outer_vertex_triangle(h1);
  Surface_mesh::Vertex c0 = this->get_outer_vertex_triangle(
                              input_mesh_->opposite_halfedge(input_mesh_->next_halfedge(h0)));
  Surface_mesh::Vertex c1 = this->get_outer_vertex_triangle(
                              input_mesh_->opposite_halfedge(input_mesh_->prev_halfedge(h0)));
  Surface_mesh::Vertex c2 = this->get_outer_vertex_triangle(
                              input_mesh_->opposite_halfedge(input_mesh_->next_halfedge(h1)));
  Surface_mesh::Vertex c3 = this->get_outer_vertex_triangle(
                              input_mesh_->opposite_halfedge(input_mesh_->prev_halfedge(h1)));
  edge_point = 1. / 2. * (v_points_[a0] + v_points_[a1]) + 1. / 8. *
               (v_points_[b0] + v_points_[b1])
               - 1. / 16. * (v_points_[c0] + v_points_[c1] + v_points_[c2] + v_points_[c3]);
}

} // namespace subdivision
