
#include <exception>
#include "plugins/subdivision/debug.h"
#include "plugins/subdivision/sd_loop.h"

namespace subdivision {

void SubdivLoop::subdivide_input_mesh_write_output_mesh() {
  this->init_mesh_members();

  this->compute_all_odd_vertices();
  this->compute_all_even_vertices();
  this->compute_all_faces();

  this->deinit_mesh_members();
}

void SubdivLoop::init_mesh_members() {
  SubdivAlgorithm::init_mesh_members();
  input_mesh_->add_vertex_property<Surface_mesh::Vertex>
  (kPropVertexIndexResultV);
  input_mesh_->add_edge_property<Surface_mesh::Vertex>
  (kPropVertexIndexResultE);
  v_index_result_v_prop_ =
    input_mesh_->get_vertex_property<Surface_mesh::Vertex>(kPropVertexIndexResultV);
  v_index_result_e_prop_ = input_mesh_->get_edge_property<Surface_mesh::Vertex>
                           (kPropVertexIndexResultE);
}

void SubdivLoop::deinit_mesh_members() {
  SubdivAlgorithm::deinit_mesh_members();
  input_mesh_->remove_edge_property(v_index_result_e_prop_);
  input_mesh_->remove_vertex_property(v_index_result_v_prop_);
}

void SubdivLoop::compute_all_odd_vertices() {
  Surface_mesh::Edge_iterator eit;
  Point edge_point;
  for (eit = input_mesh_->edges_begin(); eit != input_mesh_->edges_end(); ++eit) {
    this->compute_odd_vertex(edge_point, *eit);
    e_points_[*eit] = edge_point;
    v_index_result_e_prop_[*eit] = result_mesh_->add_vertex(e_points_[*eit]);
    DEBUG_POINT(edge_point, "Odd Vertex Point");
  }
}

void SubdivLoop::compute_all_even_vertices() {
  Surface_mesh::Vertex_iterator vit;
  Point even_vertex_point;
  for (vit = input_mesh_->vertices_begin(); vit != input_mesh_->vertices_end();
       ++vit) {
    this->compute_even_vertex(even_vertex_point, *vit);
    v_points_updated_[*vit] = even_vertex_point;
    v_index_result_v_prop_[*vit] = result_mesh_->add_vertex(
                                     v_points_updated_[*vit]);
    DEBUG_POINT(even_vertex_point, "Even Vertex Point");
  }
}

void SubdivLoop::compute_all_faces() {
  // TODO
}

void SubdivLoop::compute_odd_vertex(Point& odd_vertex,
                                    const Surface_mesh::Edge& edge) {
  odd_vertex = Point(0);
  Surface_mesh::Vertex edge_vertex0, edge_vertex1, face_vertex0, face_vertex1;
  Surface_mesh::Halfedge h_face0, h_face1;
  edge_vertex0 = input_mesh_->vertex(edge, 0);
  edge_vertex1 = input_mesh_->vertex(edge, 1);
  h_face0 = input_mesh_->halfedge(edge, 0);
  h_face1 = input_mesh_->halfedge(edge, 1);
  face_vertex0 = input_mesh_->from_vertex(input_mesh_->prev_halfedge(h_face0));
  face_vertex1 = input_mesh_->from_vertex(input_mesh_->prev_halfedge(h_face1));
  odd_vertex = 3 * v_points_[edge_vertex0] +  3 * v_points_[edge_vertex1] +
               v_points_[face_vertex0] + v_points_[face_vertex0];
  odd_vertex /= 8;
}

void SubdivLoop::compute_even_vertex(Point& even_vertex,
                                     const Surface_mesh::Vertex& vertex) {
  even_vertex = Point(0);
  Point sum_surrounding_vertices = Point(0);
  // n - number of surrounding vertices connected to the vertex by an edge
  unsigned int n = 0;
  Surface_mesh::Halfedge_around_vertex_circulator hc = input_mesh_->halfedges(
        vertex);
  for (const Surface_mesh::Halfedge& h : hc) {
    sum_surrounding_vertices += v_points_[input_mesh_->to_vertex(h)];
    ++n; // count number of surrounding vertices
  }
  const double kBeta = this->compute_beta(n);
  even_vertex = v_points_[vertex] * (1 - n * kBeta) + sum_surrounding_vertices *
                kBeta;
}

double SubdivLoop::compute_beta(unsigned int n) const {
  if (n == 3) {
    return 3. / 16.;
  } else if (n > 3) {
    return 3. / (8. * n);
  } else { // Error
    throw new std::runtime_error("Invalid number of surrounding vertices: " + n);
  }
}

} // namespace subdivision
