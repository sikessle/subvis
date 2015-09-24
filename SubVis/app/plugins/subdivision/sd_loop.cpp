
#include <exception>
#include <QDebug>
#include "plugins/subdivision/debug.h"
#include "plugins/subdivision/sd_loop.h"

namespace subdivision {

void SdLoop::subdivide_input_mesh_write_output_mesh() {
  this->init_mesh_members();
  this->compute_all_odd_vertices();
  this->compute_all_even_vertices();
  this->add_all_faces_output_mesh();
  this->deinit_mesh_members();
}

void SdLoop::compute_all_odd_vertices() {
  Point odd_vertex_point;
  for (const auto& edge : input_mesh_->edges()) {
    this->compute_odd_vertex(odd_vertex_point, edge);
    v_index_output_e_prop_[edge] = result_mesh_->add_vertex(odd_vertex_point);
    DEBUG_POINT(odd_vertex_point, "Odd Vertex Point");
  }
}

void SdLoop::compute_all_even_vertices() {
  Point even_vertex_point;
  for (const auto& vertex : input_mesh_->vertices()) {
    this->compute_even_vertex(even_vertex_point, vertex);
    v_index_output_v_prop_[vertex] = result_mesh_->add_vertex(even_vertex_point);
    DEBUG_POINT(even_vertex_point, "Even Vertex Point");
  }
}

void SdLoop::compute_odd_vertex(Point& odd_vertex,
                                const Surface_mesh::Edge& edge) {
  if (input_mesh_->is_boundary(edge)) {
    this->compute_odd_vertex_boundary(odd_vertex, edge);
  } else {
    this->compute_odd_vertex_regular(odd_vertex, edge);
  }
}

void SdLoop::compute_odd_vertex_regular(Point& odd_vertex,
                                        const Surface_mesh::Edge& edge) {
  const Surface_mesh::Vertex edge_vertex0 = input_mesh_->vertex(edge, 0);
  const Surface_mesh::Vertex edge_vertex1 = input_mesh_->vertex(edge, 1);
  const Surface_mesh::Vertex outer_vertex0 = this->get_outer_vertex_triangle(
        input_mesh_->halfedge(edge, 0));
  const Surface_mesh::Vertex outer_vertex1 = this->get_outer_vertex_triangle(
        input_mesh_->halfedge(edge, 1));
  odd_vertex = 3. * (v_points_[edge_vertex0] + v_points_[edge_vertex1]) +
               (v_points_[outer_vertex0] + v_points_[outer_vertex1]);
  odd_vertex /= 8.;
}

void SdLoop::compute_odd_vertex_boundary(Point& odd_vertex,
    const Surface_mesh::Edge& edge) {
  this->compute_mid_edge(odd_vertex, edge);
}

void SdLoop::compute_even_vertex(Point& even_vertex,
                                 const Surface_mesh::Vertex& vertex) {
  if (input_mesh_->is_boundary(vertex)) {
    this->compute_even_vertex_boundary(even_vertex, vertex);
  } else {
    this->compute_even_vertex_regular(even_vertex, vertex);
  }
}

void SdLoop::compute_even_vertex_regular(Point& even_vertex,
    const Surface_mesh::Vertex& vertex) {
  Point sum_surrounding_vertices = Point(0);
  // n - number of surrounding vertices connected to the vertex by an edge
  unsigned int n = 0;
  for (const auto& halfedge : input_mesh_->halfedges(vertex)) {
    sum_surrounding_vertices += v_points_[input_mesh_->to_vertex(halfedge)];
    ++n; // count number of surrounding vertices
  }
  const double beta = this->compute_beta(n);
  even_vertex = v_points_[vertex] * (1 - n * beta) + sum_surrounding_vertices *
                beta;
}

void SdLoop::compute_even_vertex_boundary(Point& even_vertex,
    const Surface_mesh::Vertex& vertex) {
  even_vertex = 3. / 4. * v_points_[vertex];
  for (const auto& halfedge : input_mesh_->halfedges(vertex)) {
    if (input_mesh_->is_boundary(input_mesh_->edge(halfedge))) {
      even_vertex += (1. / 8. * v_points_[input_mesh_->to_vertex(halfedge)]);
    }
  }
}

double SdLoop::compute_beta(unsigned int n) const {
  if (n == 3) {
    return 3. / 16.;
  } else if (n > 3) {
    return 3. / (8. * n);
  } else { // Error
    throw new std::runtime_error("Invalid number of surrounding vertices: " + n);
  }
}

} // namespace subdivision
