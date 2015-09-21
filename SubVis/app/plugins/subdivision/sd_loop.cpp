
#include <exception>
#include <QDebug>
#include "plugins/subdivision/debug.h"
#include "plugins/subdivision/sd_loop.h"

namespace subdivision {

void SdLoop::subdivide_input_mesh_write_output_mesh() {
  this->init_mesh_members();

  this->compute_all_odd_vertices();
  this->compute_all_even_vertices();
  this->compute_all_faces();

  this->deinit_mesh_members();
}

void SdLoop::init_mesh_members() {
  SdAlgorithm::init_mesh_members();
  input_mesh_->add_vertex_property<Surface_mesh::Vertex>
  (kPropVertexIndexResultV);
  input_mesh_->add_edge_property<Surface_mesh::Vertex>
  (kPropVertexIndexResultE);
  v_index_result_v_prop_ =
    input_mesh_->get_vertex_property<Surface_mesh::Vertex>(kPropVertexIndexResultV);
  v_index_result_e_prop_ = input_mesh_->get_edge_property<Surface_mesh::Vertex>
                           (kPropVertexIndexResultE);
}

void SdLoop::deinit_mesh_members() {
  SdAlgorithm::deinit_mesh_members();
  input_mesh_->remove_edge_property(v_index_result_e_prop_);
  input_mesh_->remove_vertex_property(v_index_result_v_prop_);
}

void SdLoop::compute_all_odd_vertices() {
  Point odd_vertex_point;
  for (const auto& edge : input_mesh_->edges()) {
    this->compute_odd_vertex(odd_vertex_point, edge);
    e_points_[edge] = odd_vertex_point;
    v_index_result_e_prop_[edge] = result_mesh_->add_vertex(e_points_[edge]);
    DEBUG_POINT(odd_vertex_point, "Odd Vertex Point");
  }
}

void SdLoop::compute_all_even_vertices() {
  Point even_vertex_point;
  for (const auto& vertex : input_mesh_->vertices()) {
    this->compute_even_vertex(even_vertex_point, vertex);
    v_points_updated_[vertex] = even_vertex_point;
    v_index_result_v_prop_[vertex] = result_mesh_->add_vertex(
                                       v_points_updated_[vertex]);
    DEBUG_POINT(even_vertex_point, "Even Vertex Point");
  }
}

void SdLoop::compute_all_faces() {
  for (const auto& face : input_mesh_->faces()) {
    this->compute_new_faces(face);
  }
}

void SdLoop::compute_odd_vertex(Point& odd_vertex,
                                    const Surface_mesh::Edge& edge) {
  odd_vertex = Point(0);
  Surface_mesh::Vertex edge_vertex0, edge_vertex1, face_vertex0, face_vertex1;
  Surface_mesh::Halfedge h_face0, h_face1;
  edge_vertex0 = input_mesh_->vertex(edge, 0);
  edge_vertex1 = input_mesh_->vertex(edge, 1);
  h_face0 = input_mesh_->halfedge(edge, 0);
  h_face1 = input_mesh_->halfedge(edge, 1);
  face_vertex0 = input_mesh_->to_vertex(input_mesh_->next_halfedge(h_face0));
  face_vertex1 = input_mesh_->to_vertex(input_mesh_->next_halfedge(h_face1));
  odd_vertex = 3 * (v_points_[edge_vertex0] + v_points_[edge_vertex1]) +
               (v_points_[face_vertex0] + v_points_[face_vertex1]);
  odd_vertex /= 8.;
}

void SdLoop::compute_even_vertex(Point& even_vertex,
                                     const Surface_mesh::Vertex& vertex) {
  even_vertex = Point(0);
  Point sum_surrounding_vertices = Point(0);
  // n - number of surrounding vertices connected to the vertex by an edge
  unsigned int n = 0;
  for (const auto& halfedge : input_mesh_->halfedges(vertex)) {
    sum_surrounding_vertices += v_points_[input_mesh_->to_vertex(halfedge)];
    ++n; // count number of surrounding vertices
  }
  const double kBeta = this->compute_beta(n);
  even_vertex = v_points_[vertex] * (1 - n * kBeta) + sum_surrounding_vertices *
                kBeta;
}

void SdLoop::compute_new_faces(const Surface_mesh::Face& face) {
  const int kArraySize = 3;
  Surface_mesh::Vertex v_index_list[kArraySize];
  Surface_mesh::Vertex e_index_list[kArraySize];
  unsigned int i = 0;
  for (const auto& halfedge : input_mesh_->halfedges(face)) {
    if (i < kArraySize) { // check if in array bounds
      v_index_list[i] = v_index_result_v_prop_[input_mesh_->from_vertex(halfedge)];
      e_index_list[i] = v_index_result_e_prop_[input_mesh_->edge(halfedge)];
    }
    ++i;
  }
  result_mesh_->add_triangle(e_index_list[0], e_index_list[1], e_index_list[2]);
  result_mesh_->add_triangle(v_index_list[0], e_index_list[0], e_index_list[2]);
  result_mesh_->add_triangle(v_index_list[1], e_index_list[1], e_index_list[0]);
  result_mesh_->add_triangle(v_index_list[2], e_index_list[2], e_index_list[1]);
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
