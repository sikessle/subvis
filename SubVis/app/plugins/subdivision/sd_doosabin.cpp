
#include <vector>
#include "plugins/subdivision/debug.h"
#include "plugins/subdivision/sd_doosabin.h"

namespace subdivision {

void SubdivDooSabin::subdivide_input_mesh_write_output_mesh() {
  this->init_mesh_members();

  // loop over all faces and compute face points (same as catmull clark face points)
  this->compute_all_face_points();
  // loop over all edges and compute edge points (different to catmull clark edge points!)
  this->compute_all_edge_points();
  // compute new vertex point
  this->compute_all_new_vertex_points();
  // compute and add faces to the result mesh
  this->compute_faces();

  this->deinit_mesh_members();
}

void SubdivDooSabin::init_mesh_members() {
  SubdivAlgorithm::init_mesh_members();
  input_mesh_->add_face_property<VertexToVertexMap>
  (kPropVertexIndexResultMapF);
  f_vertex_index_map_ = input_mesh_->get_face_property<VertexToVertexMap>
                        (kPropVertexIndexResultMapF);
}

void SubdivDooSabin::deinit_mesh_members() {
  SubdivAlgorithm::deinit_mesh_members();
  input_mesh_->remove_face_property(f_vertex_index_map_);
}

void SubdivDooSabin::compute_all_face_points() {
  Point face_point;
  for (const auto& face : input_mesh_->faces()) {
    this->compute_face_point(face_point, face);
    // add new face point to mesh
    f_points_[face] = face_point;
    DEBUG_POINT(face_point, "Face Point");
  }
}

void SubdivDooSabin::compute_all_edge_points() {
  Point edge_point;
  for (const auto& edge : input_mesh_->edges()) {
    this->compute_mid_edge(edge_point, edge);
    // store edge_point as property
    e_points_[edge] = edge_point;
    DEBUG_POINT(edge_point, "Edge Point");
  }
}

void SubdivDooSabin::compute_all_new_vertex_points() {
  // loop over all faces - every face creates 4 new vertex points
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

void SubdivDooSabin::compute_new_vertex_point(Point& new_vertex_point,
    const Surface_mesh::Vertex& vertex, const Surface_mesh::Face& face) {
  // compute new vertex point: average of four points (face point, two edge points and vertex)
  new_vertex_point = Point(0);
  // find halfedge with from vertex == vertex
  for (const Surface_mesh::Halfedge& halfedge : input_mesh_->halfedges(face)) {
    if (input_mesh_->from_vertex(halfedge) == vertex) {
      // halfedge found - do computation
      Surface_mesh::Edge e1, e2;
      e1 = input_mesh_->edge(halfedge);
      e2 = input_mesh_->edge(input_mesh_->prev_halfedge(halfedge));
      new_vertex_point = (v_points_[vertex] + f_points_[face] + e_points_[e1] +
                          e_points_[e2] ) / 4;
      break;
    }
  }
}

void SubdivDooSabin::compute_faces() {
  // compute face faces
  this->compute_faces_face();
  // compute edge faces
  this->compute_faces_edge();
  // compute vertex faces
  this->compute_faces_vertex();
}

void SubdivDooSabin::compute_faces_face() {
  std::vector<Surface_mesh::Vertex> vertices_vec;
  for (const auto& face : input_mesh_->faces()) {
    for (const auto& vertex : input_mesh_->vertices(face)) {
      vertices_vec.push_back(f_vertex_index_map_[face].at(vertex));
    }
    result_mesh_->add_face(vertices_vec);
    vertices_vec.clear();
  }
}

void SubdivDooSabin::compute_faces_edge() {
  std::vector<Surface_mesh::Vertex> vertices_vec;
  Surface_mesh::Face f0, f1;
  Surface_mesh::Vertex v0, v1;
  for (const auto& edge : input_mesh_->edges()) {
    f0 = input_mesh_->face(edge, 0);
    f1 = input_mesh_->face(edge, 1);
    v0 = input_mesh_->vertex(edge, 0);
    v1 = input_mesh_->vertex(edge, 1);
    // get coordinates of the subdivision vertex points
    vertices_vec.push_back(f_vertex_index_map_[f0].at(v0));
    vertices_vec.push_back(f_vertex_index_map_[f0].at(v1));
    vertices_vec.push_back(f_vertex_index_map_[f1].at(v1));
    vertices_vec.push_back(f_vertex_index_map_[f1].at(v0));
    result_mesh_->add_face(vertices_vec);
    vertices_vec.clear();
  }
}

void SubdivDooSabin::compute_faces_vertex() {
  std::vector<Surface_mesh::Vertex> vertices_vec;
  for (const auto& vertex : input_mesh_->vertices()) {
    for (const auto& face : input_mesh_->faces(vertex)) {
      vertices_vec.push_back(f_vertex_index_map_[face].at(vertex));
    }
    result_mesh_->add_face(vertices_vec);
    vertices_vec.clear();
  }
}


} // namespace subdivision
