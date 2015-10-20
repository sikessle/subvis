
#include <vector>
#include "plugins/subdivision/debug.h"
#include "plugins/subdivision/sd_doosabin.h"

namespace subdivision {

using Surface_mesh = surface_mesh::Surface_mesh;

void SdDooSabin::subdivide_input_mesh_write_output_mesh() {
  this->init_mesh_members();
  this->compute_all_face_points();
  this->compute_all_edge_points();
  this->add_all_new_vertex_points_output_mesh();
  this->add_all_new_boundary_vertex_points_output_mesh();
  this->add_all_faces_output_mesh();
  this->deinit_mesh_members();
}

void SdDooSabin::init_mesh_members() {
  SdQuad::init_mesh_members();
  input_mesh_->add_face_property<VertexToVertexMap>(kPropVertexIndexOutputMapF);
  input_mesh_->add_edge_property<VertexToVertexMap>
  (kPropVertexIndexOutputBoundaryE);
  v_index_map_output_f_prop_ = input_mesh_->get_face_property<VertexToVertexMap>
                               (kPropVertexIndexOutputMapF);
  v_index_map_output_e_prop_ = input_mesh_->get_edge_property<VertexToVertexMap>
                               (kPropVertexIndexOutputBoundaryE);
}

void SdDooSabin::deinit_mesh_members() {
  SdQuad::deinit_mesh_members();
  input_mesh_->remove_face_property(v_index_map_output_f_prop_);
  input_mesh_->remove_edge_property(v_index_map_output_e_prop_);
}

void SdDooSabin::compute_all_face_points() {
  Point face_point;
  for (const auto& face : input_mesh_->faces()) {
    this->compute_face_point(face_point, face);
    // store face point as property
    f_points_[face] = face_point;
    DEBUG_POINT(face_point, "Face Point");
  }
}

void SdDooSabin::compute_all_edge_points() {
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
      v_index_map_output_f_prop_[face][vertex] = output_mesh_->add_vertex(
            new_vertex_point);
      DEBUG_POINT(new_vertex_point, "New Vertex Point");
    }
  }
}

void SdDooSabin::add_all_new_boundary_vertex_points_output_mesh() {
  Point new_vertex_point;
  for (const auto& edge : input_mesh_->edges()) {
    if (input_mesh_->is_boundary(edge)) {
      const Surface_mesh::Halfedge halfedge = input_mesh_->halfedge(edge, 0);
      const Surface_mesh::Vertex from_vertex = input_mesh_->from_vertex(halfedge);
      const Surface_mesh::Vertex to_vertex = input_mesh_->to_vertex(halfedge);
      this->compute_boundary_vertex_point(new_vertex_point, halfedge);
      v_index_map_output_e_prop_[edge][from_vertex] = output_mesh_->add_vertex(
            new_vertex_point);
      DEBUG_POINT(new_vertex_point, "New Boundary Vertex Point 0");
      this->compute_boundary_vertex_point(new_vertex_point,
                                          input_mesh_->opposite_halfedge(halfedge));
      v_index_map_output_e_prop_[edge][to_vertex] = output_mesh_->add_vertex(
            new_vertex_point);
      DEBUG_POINT(new_vertex_point, "New Boundary Vertex Point 1");
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
                          e_points_[e2] ) / 4.;
      break;
    }
  }
}

void SdDooSabin::compute_boundary_vertex_point(Point& new_vertex_point,
    const Surface_mesh::Halfedge& halfedge) const {
  new_vertex_point = v_points_[input_mesh_->from_vertex(halfedge)] * 3. / 4. +
                     v_points_[input_mesh_->to_vertex(halfedge)] * 1. / 4.;
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
      vertices_vec.push_back(v_index_map_output_f_prop_[face].at(vertex));
    }
    output_mesh_->add_face(vertices_vec);
    vertices_vec.clear();
  }
}

void SdDooSabin::add_all_faces_output_mesh_edge() {
  for (const auto& edge : input_mesh_->edges()) {
    const Surface_mesh::Vertex v0 = input_mesh_->vertex(edge, 0);
    const Surface_mesh::Vertex v1 = input_mesh_->vertex(edge, 1);
    if (input_mesh_->is_boundary(edge)) {
      const Surface_mesh::Face face = input_mesh_->face(
                                        this->get_valid_halfedge_of_boundary_edge(edge));
      output_mesh_->add_quad(v_index_map_output_f_prop_[face].at(v0),
                             v_index_map_output_f_prop_[face].at(v1),
                             v_index_map_output_e_prop_[edge].at(v1),
                             v_index_map_output_e_prop_[edge].at(v0));
    } else {
      const Surface_mesh::Face f0 = input_mesh_->face(edge, 0);
      const Surface_mesh::Face f1 = input_mesh_->face(edge, 1);
      output_mesh_->add_quad(v_index_map_output_f_prop_[f0].at(v0),
                             v_index_map_output_f_prop_[f0].at(v1),
                             v_index_map_output_f_prop_[f1].at(v1),
                             v_index_map_output_f_prop_[f1].at(v0));
    }
  }
}

void SdDooSabin::add_all_faces_output_mesh_vertex() {
  std::vector<Surface_mesh::Vertex> vertices_vec;
  for (const auto& vertex : input_mesh_->vertices()) {
    for (const auto& halfedge : input_mesh_->halfedges(vertex)) {
      const Surface_mesh::Edge boundary_edge = input_mesh_->edge(halfedge);
      if (input_mesh_->is_boundary(boundary_edge)) {
        vertices_vec.push_back(v_index_map_output_e_prop_[boundary_edge].at(vertex));
      }
      // only use halfedges that belong to a valid face
      if (!input_mesh_->is_boundary(halfedge)) {
        vertices_vec.push_back(v_index_map_output_f_prop_[input_mesh_->face(
                                 halfedge)].at(vertex));
      }
    }
    if (vertices_vec.size() > 2) {
      output_mesh_->add_face(vertices_vec);
    }
    vertices_vec.clear();
  }
}

} // namespace subdivision
