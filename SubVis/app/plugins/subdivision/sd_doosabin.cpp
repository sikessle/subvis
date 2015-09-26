
#include <vector>
#include "plugins/subdivision/debug.h"
#include "plugins/subdivision/sd_doosabin.h"

namespace subdivision {

void SdDooSabin::subdivide_input_mesh_write_output_mesh() {
  this->init_mesh_members();
  this->add_all_face_points_output_mesh();
  this->add_all_edge_points_output_mesh();
  this->add_all_new_vertex_points_output_mesh();
  this->add_all_faces_output_mesh();
  this->deinit_mesh_members();
}

void SdDooSabin::init_mesh_members() {
  SdQuad::init_mesh_members();
  input_mesh_->add_face_property<VertexToVertexMap>
  (kPropVertexIndexOutputMapF);
  f_vertex_index_map_ = input_mesh_->get_face_property<VertexToVertexMap>
                        (kPropVertexIndexOutputMapF);
}

void SdDooSabin::deinit_mesh_members() {
  SdQuad::deinit_mesh_members();
  input_mesh_->remove_face_property(f_vertex_index_map_);
}

void SdDooSabin::add_all_face_points_output_mesh() {
  Point face_point;
  for (const auto& face : input_mesh_->faces()) {
    this->compute_face_point(face_point, face);
    // store face point as property
    f_points_[face] = face_point;
    DEBUG_POINT(face_point, "Face Point");
  }
}

void SdDooSabin::add_all_edge_points_output_mesh() {
  Point edge_point;
  for (const auto& edge : input_mesh_->edges()) {
    this->compute_mid_edge(edge_point, edge);
    // store edge_point as property
    e_points_[edge] = edge_point;
    DEBUG_POINT(edge_point, "Edge Point");
  }
}

void SdDooSabin::add_all_new_vertex_points_output_mesh() {
  Point new_vertex_point;
  for (const auto& face : input_mesh_->faces()) {
    for (const auto& vertex : input_mesh_->vertices(face)) {
      this->compute_new_vertex_point(new_vertex_point, vertex, face);
      // add new vertex to result mesh and store the index of the vertex as property in the input mesh
      f_vertex_index_map_[face][vertex] = result_mesh_->add_vertex(new_vertex_point);
      DEBUG_POINT(new_vertex_point, "New Vertex Point");
    }
  }
}

void SdDooSabin::compute_new_vertex_point(Point& new_vertex_point,
    const Surface_mesh::Vertex& vertex, const Surface_mesh::Face& face) const {
  new_vertex_point = Point(0);
  // find halfedge with from vertex == vertex
  for (const Surface_mesh::Halfedge& halfedge : input_mesh_->halfedges(face)) {
    if (input_mesh_->from_vertex(halfedge) == vertex) {
      // halfedge found - do computation
      const Surface_mesh::Edge e1 = input_mesh_->edge(halfedge);
      const Surface_mesh::Edge e2 = input_mesh_->edge(input_mesh_->prev_halfedge(
                                      halfedge));
      new_vertex_point = (v_points_[vertex] + f_points_[face] + e_points_[e1] +
                          e_points_[e2] ) / 4;
      break;
    }
  }
}

void SdDooSabin::add_all_faces_output_mesh() {
  this->add_all_faces_output_mesh_face();
  this->add_all_faces_output_mesh_edge();
  this->add_all_faces_output_mesh_vertex();
}

void SdDooSabin::add_all_faces_output_mesh_face() {
  std::vector<Surface_mesh::Vertex> vertices_vec;
  for (const auto& face : input_mesh_->faces()) {
    for (const auto& vertex : input_mesh_->vertices(face)) {
      vertices_vec.push_back(f_vertex_index_map_[face].at(vertex));
    }
    result_mesh_->add_face(vertices_vec);
    vertices_vec.clear();
  }
}

void SdDooSabin::add_all_faces_output_mesh_edge() {
  Surface_mesh::Face f0, f1;
  Surface_mesh::Vertex v0, v1;
  for (const auto& edge : input_mesh_->edges()) {
    if (!input_mesh_->is_boundary(edge)) {
      f0 = input_mesh_->face(edge, 0);
      f1 = input_mesh_->face(edge, 1);
      v0 = input_mesh_->vertex(edge, 0);
      v1 = input_mesh_->vertex(edge, 1);
      result_mesh_->add_quad(f_vertex_index_map_[f0].at(v0),
                             f_vertex_index_map_[f0].at(v1), f_vertex_index_map_[f1].at(v1),
                             f_vertex_index_map_[f1].at(v0));
    }
  }
}

void SdDooSabin::add_all_faces_output_mesh_vertex() {
  std::vector<Surface_mesh::Vertex> vertices_vec;
  for (const auto& vertex : input_mesh_->vertices()) {
    for (const auto& face : input_mesh_->faces(vertex)) {
      vertices_vec.push_back(f_vertex_index_map_[face].at(vertex));
    }
    if (vertices_vec.size() > 2) {
      result_mesh_->add_face(vertices_vec);
    }
    vertices_vec.clear();
  }
}

} // namespace subdivision
