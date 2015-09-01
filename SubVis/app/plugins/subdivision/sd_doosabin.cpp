
#include <vector>
#include "plugins/subdivision/debug.h"
#include "plugins/subdivision/sd_doosabin.h"

namespace subdivision {

void SubdivDooSabin::subdivide_specific_algorithm() {
  // read from input_mesh_
  // write to result_mesh_
  this->add_mesh_properties();
  this->init_mesh_members();

  // loop over all faces and compute face points (same as catmull clark face points)
  this->compute_all_face_points();
  // loop over all edges and compute edge points (different to catmull clark edge points!)
  this->compute_all_edge_points();
  // compute new vertex point
  this->compute_all_new_vertex_points();
  // compute and add faces to the result mesh
  this->compute_faces();

  this->remove_mesh_properties();
}

void SubdivDooSabin::add_mesh_properties() {
  // add properties that are necessary for doo sabin
  input_mesh_->add_face_property<Point>(kPropFacePoint);
  input_mesh_->add_edge_property<Point>(kPropEdgePoint);

  input_mesh_->add_face_property<VertexToVertexMap>
  (kPropVertexIndexResultMapF);
}

void SubdivDooSabin::init_mesh_members() {
  SubdivAlgorithm::init_mesh_members();
  f_points_ = input_mesh_->get_face_property<Point>(kPropFacePoint);
  e_points_ = input_mesh_->get_edge_property<Point>(kPropEdgePoint);
  f_vertex_index_map_ = input_mesh_->get_face_property<VertexToVertexMap>
                        (kPropVertexIndexResultMapF);
}

void SubdivDooSabin::remove_mesh_properties() {
  input_mesh_->remove_face_property(f_points_);
  input_mesh_->remove_edge_property(e_points_);
  input_mesh_->remove_face_property(f_vertex_index_map_);
}

void SubdivDooSabin::compute_all_face_points() {
  Surface_mesh::Face_iterator fit;
  Point face_point;
  for (fit = input_mesh_->faces_begin(); fit != input_mesh_->faces_end(); ++fit) {
    this->compute_face_point(face_point, *fit);
    // add new face point to mesh
    f_points_[*fit] = face_point;
    DEBUG_POINT(face_point, "Face Point");
  }
}

void SubdivDooSabin::compute_all_edge_points() {
  Surface_mesh::Edge_iterator eit;
  Point edge_point;
  for (eit = input_mesh_->edges_begin(); eit != input_mesh_->edges_end(); ++eit) {
    this->mid_edge(edge_point, *eit);
    // store edge_point as property
    e_points_[*eit] = edge_point;
    DEBUG_POINT(edge_point, "Edge Point");
  }
}

void SubdivDooSabin::compute_all_new_vertex_points() {
  // loop over all faces - every face creates 4 new vertex points
  Surface_mesh::Face_iterator fit;
  Surface_mesh::Vertex_around_face_circulator vc, vc_end;
  Point new_vertex_point;
  for (fit = input_mesh_->faces_begin(); fit != input_mesh_->faces_end(); ++fit) {
    vc = input_mesh_->vertices(*fit);
    for (const Surface_mesh::Vertex& v : vc) {
      this->compute_new_vertex_point(new_vertex_point, v, *fit);
      // add new vertex to result mesh and store the index of the vertex as property in the input mesh
      f_vertex_index_map_[*fit][v] = result_mesh_->add_vertex(new_vertex_point);
      DEBUG_POINT(new_vertex_point, "New Vertex Point");
    }
  }
}

void SubdivDooSabin::compute_new_vertex_point(Point& new_vertex_point,
    const Surface_mesh::Vertex& vertex, const Surface_mesh::Face& face) {
  // compute new vertex point: average of four points (face point, two edge points and vertex)
  Surface_mesh::Halfedge_around_face_circulator hc = input_mesh_->halfedges(face);
  for (const Surface_mesh::Halfedge& h : hc) {
    if (input_mesh_->from_vertex(h) == vertex) {
      break;
    }
  }
  Surface_mesh::Edge e1, e2;
  e1 = input_mesh_->edge(*hc);
  e2 = input_mesh_->edge(input_mesh_->prev_halfedge(*hc));
  new_vertex_point = (v_points_[vertex] + f_points_[face] + e_points_[e1] +
                      e_points_[e2] ) / 4;
}

void SubdivDooSabin::compute_faces() {
  // compute face faces
  this->compute_faces_face();
  // compute edge faces
  // TODO why not working ?
  this->compute_faces_edge();
  // compute vertex faces
  this->compute_faces_vertex();
}

void SubdivDooSabin::compute_faces_face() {
  std::vector<Surface_mesh::Vertex> vertices_vec;
  Surface_mesh::Face_iterator fit;
  for (fit = input_mesh_->faces_begin(); fit != input_mesh_->faces_end(); ++fit) {
    Surface_mesh::Vertex_around_face_circulator vc = input_mesh_->vertices(*fit);
    for (const Surface_mesh::Vertex& v : vc) {
      vertices_vec.push_back(f_vertex_index_map_[*fit].at(v));
    }
    result_mesh_->add_face(vertices_vec);
    vertices_vec.clear();
  }
}

void SubdivDooSabin::compute_faces_edge() {
  std::vector<Surface_mesh::Vertex> vertices_vec;
  Surface_mesh::Edge_iterator eit;
  Surface_mesh::Face f0, f1;
  Surface_mesh::Vertex v0, v1;
  for (eit = input_mesh_->edges_begin(); eit != input_mesh_->edges_end(); ++eit) {
    f0 = input_mesh_->face(*eit, 0);
    f1 = input_mesh_->face(*eit, 1);
    v0 = input_mesh_->vertex(*eit, 0);
    v1 = input_mesh_->vertex(*eit, 1);
    // get coordinates of the subdivision vertex points
    vertices_vec.push_back(f_vertex_index_map_[f0].at(v0));
    vertices_vec.push_back(f_vertex_index_map_[f0].at(v1));
    vertices_vec.push_back(f_vertex_index_map_[f1].at(v0));
    vertices_vec.push_back(f_vertex_index_map_[f1].at(v1));
    result_mesh_->add_face(vertices_vec);
    vertices_vec.clear();
  }
}

void SubdivDooSabin::compute_faces_vertex() {
  std::vector<Surface_mesh::Vertex> vertices_vec;
  Surface_mesh::Vertex_iterator vit;
  for (vit = input_mesh_->vertices_begin(); vit != input_mesh_->vertices_end();
       ++vit) {
    Surface_mesh::Face_around_vertex_circulator fc = input_mesh_->faces(*vit);
    for (const Surface_mesh::Face& f : fc) {
      vertices_vec.push_back(f_vertex_index_map_[f].at(*vit));
    }
    result_mesh_->add_face(vertices_vec);
    vertices_vec.clear();
  }
}


} // namespace subdivision
