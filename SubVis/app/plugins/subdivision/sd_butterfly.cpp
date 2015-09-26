
#include <cmath>
#include "plugins/subdivision/debug.h"

#include "plugins/subdivision/sd_butterfly.h"

namespace subdivision {

void SdButterfly::subdivide_input_mesh_write_output_mesh() {
  this->init_mesh_members();
  this->add_all_vertices_output_mesh();
  this->add_all_edge_points_output_mesh();
  this->add_all_faces_output_mesh();
  this->deinit_mesh_members();
}

void SdButterfly::add_all_vertices_output_mesh() {
  for (const auto& vertex : input_mesh_->vertices()) {
    v_index_output_v_prop_[vertex] = result_mesh_->add_vertex(v_points_[vertex]);
  }
}

void SdButterfly::add_all_edge_points_output_mesh() {
  Point edge_point;
  for (const auto& edge : input_mesh_->edges()) {
    this->compute_edge_point(edge_point, edge);
    v_index_output_e_prop_[edge] = result_mesh_->add_vertex(edge_point);
    DEBUG_POINT(edge_point, "New Interpolated Point");
  }
}

void SdButterfly::compute_edge_point(Point& edge_point,
                                     const Surface_mesh::Edge& edge) const {
  if (input_mesh_->is_boundary(edge)) {
    this->compute_edge_point_boundary(edge_point, edge);
  } else {
    this->compute_edge_point_ordinary(edge_point, edge);
  }
}

void SdButterfly::compute_edge_point_ordinary(Point& edge_point,
    const Surface_mesh::Edge& edge) const {
  const Surface_mesh::Halfedge h0 = input_mesh_->halfedge(edge, 0);
  const Surface_mesh::Halfedge h1 = input_mesh_->halfedge(edge, 1);
  const Surface_mesh::Vertex a0 = input_mesh_->vertex(edge, 0);
  const Surface_mesh::Vertex a1 = input_mesh_->vertex(edge, 1);
  const Surface_mesh::Vertex b0 = this->get_outer_vertex_triangle(h0);
  const Surface_mesh::Vertex b1 = this->get_outer_vertex_triangle(h1);
  const Surface_mesh::Vertex c0 = this->get_outer_vertex_triangle(
                                    input_mesh_->opposite_halfedge(input_mesh_->next_halfedge(h0)));
  const Surface_mesh::Vertex c1 = this->get_outer_vertex_triangle(
                                    input_mesh_->opposite_halfedge(input_mesh_->prev_halfedge(h0)));
  const Surface_mesh::Vertex c2 = this->get_outer_vertex_triangle(
                                    input_mesh_->opposite_halfedge(input_mesh_->next_halfedge(h1)));
  const Surface_mesh::Vertex c3 = this->get_outer_vertex_triangle(
                                    input_mesh_->opposite_halfedge(input_mesh_->prev_halfedge(h1)));
  edge_point = 1. / 2. * (v_points_[a0] + v_points_[a1]) + 1. / 8. *
               (v_points_[b0] + v_points_[b1])
               - 1. / 16. * (v_points_[c0] + v_points_[c1] + v_points_[c2] + v_points_[c3]);
}

void SdButterfly::compute_edge_point_boundary(Point& edge_point,
    const Surface_mesh::Edge& edge) const {
  Surface_mesh::Halfedge h0 = this->get_next_boundary_halfedge(
                                input_mesh_->halfedge(edge, 0));
  const Surface_mesh::Halfedge h1 = this->get_next_boundary_halfedge(
                                      input_mesh_->halfedge(edge, 1));
  const Surface_mesh::Vertex vertex0 = input_mesh_->vertex(edge, 0);
  const Surface_mesh::Vertex vertex1 = input_mesh_->vertex(edge, 1);
  edge_point = -1. / 16. * (v_points_[input_mesh_->to_vertex(
                                        h0)] + v_points_[input_mesh_->to_vertex(h1)]) + 9. / 16. *
               (v_points_[vertex0] + v_points_[vertex1]);
}

surface_mesh::Surface_mesh::Halfedge SdButterfly::get_next_boundary_halfedge(
  const Surface_mesh::Halfedge halfedge) const {
  // get opposite halfedge to rotate around vertex
  Surface_mesh::Halfedge rotated_halfedge = input_mesh_->opposite_halfedge(
        halfedge);
  // rotate counter-clockwise around start vertex
  do {
    if (input_mesh_->is_boundary(rotated_halfedge)) { // halfedge found
      break;
    }
    rotated_halfedge = input_mesh_->ccw_rotated_halfedge(rotated_halfedge);
  } while (rotated_halfedge != halfedge);
  return rotated_halfedge;
}

} // namespace subdivision
