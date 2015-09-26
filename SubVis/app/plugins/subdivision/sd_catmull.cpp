
#include <QDebug>
#include <exception>

#include "surface_mesh/IO.h"

#include "plugins/subdivision/debug.h"
#include "plugins/subdivision/sd_catmull.h"


namespace subdivision {

using surface_mesh::Surface_mesh;
using surface_mesh::Point;

void SdCatmull::subdivide_input_mesh_write_output_mesh() {
  this->init_mesh_members();
  this->add_all_face_points_output_mesh();
  this->add_all_edge_points_output_mesh();
  this->add_all_updated_vertex_points_output_mesh();
  this->add_all_faces_output_mesh();
  this->deinit_mesh_members();
}

void SdCatmull::init_mesh_members() {
  SdQuad::init_mesh_members();
  input_mesh_->add_vertex_property<Surface_mesh::Vertex>(kPropVertexIndexOutputV);
  input_mesh_->add_edge_property<Surface_mesh::Vertex>(kPropVertexIndexOutputE);
  input_mesh_->add_face_property<Surface_mesh::Vertex>(kPropVertexIndexOutputF);
  v_index_output_f_prop_ = input_mesh_->get_face_property<Surface_mesh::Vertex>
                           (kPropVertexIndexOutputF);
  v_index_output_e_prop_ = input_mesh_->get_edge_property<Surface_mesh::Vertex>
                           (kPropVertexIndexOutputE);
  v_index_output_v_prop_ = input_mesh_->get_vertex_property<Surface_mesh::Vertex>
                           (kPropVertexIndexOutputV);
}

void SdCatmull::deinit_mesh_members() {
  SdQuad::deinit_mesh_members();
  input_mesh_->remove_face_property(v_index_output_f_prop_);
  input_mesh_->remove_edge_property(v_index_output_e_prop_);
  input_mesh_->remove_vertex_property(v_index_output_v_prop_);
}

void SdCatmull::add_all_face_points_output_mesh() {
  Point face_point;
  for (const auto& face : input_mesh_->faces()) {
    this->compute_face_point(face_point, face);
    // store face point as property
    f_points_[face] = face_point;
    v_index_output_f_prop_[face] = result_mesh_->add_vertex(f_points_[face]);
    DEBUG_POINT(face_point, "Face Point")
  }
}

void SdCatmull::add_all_edge_points_output_mesh() {
  Point edge_point;
  for (const auto& edge : input_mesh_->edges()) {
    this->compute_edge_point(edge_point, edge);
    // store edge_point as property
    e_points_[edge] = edge_point;
    v_index_output_e_prop_[edge] = result_mesh_->add_vertex(e_points_[edge]);
    DEBUG_POINT(edge_point, "Edge Point");
  }
}

void SdCatmull::add_all_updated_vertex_points_output_mesh() {
  Point new_vertex_point;
  for (const auto& vertex : input_mesh_->vertices()) {
    this->compute_updated_vertex_point(new_vertex_point, vertex);
    // store new_vertex_point as property
    v_points_updated_[vertex] = new_vertex_point;
    v_index_output_v_prop_[vertex] = result_mesh_->add_vertex(
                                       v_points_updated_[vertex]);
    DEBUG_POINT(new_vertex_point, "New Vertex Point");
  }
}

void SdCatmull::add_all_faces_output_mesh() {
  for (const auto& face : input_mesh_->faces()) {
    this->add_splitted_face_to_output_mesh(face);
  }
}

void SdCatmull::compute_edge_point(Point& edge_point,
                                   const Surface_mesh::Edge& edge) const {
  if (input_mesh_->is_boundary(edge)) {
    this->compute_mid_edge(edge_point, edge);
  } else {
    const Surface_mesh::Vertex edge_vertex0 = input_mesh_->vertex(edge, 0);
    const Surface_mesh::Vertex edge_vertex1 = input_mesh_->vertex(edge, 1);
    const Surface_mesh::Face edge_face0 = input_mesh_->face(edge, 0);
    const Surface_mesh::Face edge_face1 = input_mesh_->face(edge, 1);
    edge_point = v_points_[edge_vertex0] + v_points_[edge_vertex1] +
                 f_points_[edge_face0] + f_points_[edge_face1];
    edge_point /= 4;
  }
}

void SdCatmull::compute_updated_vertex_point(Point& new_vertex_point,
    const Surface_mesh::Vertex& vertex) const {
  if (input_mesh_->is_boundary(vertex)) {
    this->compute_updated_vertex_point_boundary(new_vertex_point, vertex);
  } else {
    this->compute_updated_vertex_point_regular(new_vertex_point, vertex);
  }
}

void SdCatmull::compute_updated_vertex_point_regular(Point& new_vertex_point,
    const Surface_mesh::Vertex& vertex) const {
  // (Q/n) + (2R/n) + (S(n-3)/n)
  Point q, r, s;
  const unsigned int vertex_valence = input_mesh_->valence(vertex);
  this->avg_face_points(q, vertex);
  this->avg_mid_edges(r, vertex);
  s = v_points_[vertex];
  new_vertex_point = (q / vertex_valence) + (2 * r / vertex_valence) + (s *
                     (vertex_valence - 3) / vertex_valence);
}

void SdCatmull::compute_updated_vertex_point_boundary(Point& new_vertex_point,
    const Surface_mesh::Vertex& vertex) const {
  new_vertex_point = 3. / 4. * v_points_[vertex];
  for (const auto& halfedge : input_mesh_->halfedges(vertex)) {
    if (input_mesh_->is_boundary(input_mesh_->edge(halfedge))) {
      new_vertex_point += 1. / 8. * v_points_[input_mesh_->to_vertex(halfedge)];
    }
  }
}

void SdCatmull::add_splitted_face_to_output_mesh(const Surface_mesh::Face&
    face) {
  const int arraySize = 4;
  Surface_mesh::Vertex v_index_list[arraySize];
  Surface_mesh::Vertex e_index_list[arraySize];
  int i = 0;
  for (const auto& halfedge : input_mesh_->halfedges(face)) {
    if (i < arraySize) { // check if in array bounds
      v_index_list[i] = v_index_output_v_prop_[input_mesh_->from_vertex(halfedge)];
      e_index_list[i] = v_index_output_e_prop_[input_mesh_->edge(halfedge)];
    }
    ++i; // increment to compute face valence
  }
  const Surface_mesh::Vertex f_index = v_index_output_f_prop_[face];
  if (i == 3) { // triangle face
    result_mesh_->add_quad(v_index_list[0], e_index_list[0], f_index,
                           e_index_list[2]);
    result_mesh_->add_quad(v_index_list[1], e_index_list[1], f_index,
                           e_index_list[0]);
    result_mesh_->add_quad(v_index_list[2], e_index_list[2], f_index,
                           e_index_list[1]);
  } else if (i == 4) { // quad face
    result_mesh_->add_quad(v_index_list[0], e_index_list[0], f_index,
                           e_index_list[3]);
    result_mesh_->add_quad(v_index_list[1], e_index_list[1], f_index,
                           e_index_list[0]);
    result_mesh_->add_quad(v_index_list[2], e_index_list[2], f_index,
                           e_index_list[1]);
    result_mesh_->add_quad(v_index_list[3], e_index_list[3], f_index,
                           e_index_list[2]);
  } else { // error
    /// @todo error handling
    throw new std::runtime_error("Invalid mesh topology: " + i);
  }
}

void SdCatmull::avg_face_points(Point& avg_face_points,
                                const Surface_mesh::Vertex& vertex) const {
  avg_face_points = Point(0);
  int i = 0;
  for (const auto& face : input_mesh_->faces(vertex)) {
    avg_face_points += f_points_[face];
    ++i;
  }
  if (i != 0) {
    avg_face_points /= i;
  }
}

void SdCatmull::avg_mid_edges(Point& avg_mid_edges,
                              const Surface_mesh::Vertex& vertex) const {
  avg_mid_edges = Point(0);
  int i = 0;
  Point mid_edge;
  for (const auto& halfedge : input_mesh_->halfedges(vertex)) {
    this->compute_mid_edge(mid_edge, input_mesh_->edge(halfedge));
    avg_mid_edges += mid_edge;
    ++i;
  }
  if (i != 0) {
    avg_mid_edges /= i;
  }
}

} // namespace subdivision
