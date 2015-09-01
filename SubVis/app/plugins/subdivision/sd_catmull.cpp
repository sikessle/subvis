

#include <QDebug>
#include <exception>

#include "surface_mesh/IO.h"

#include "plugins/subdivision/debug.h"
#include "plugins/subdivision/sd_catmull.h"


namespace subdivision {

using surface_mesh::Surface_mesh;
using surface_mesh::Point;

void SubdivCatmull::subdivide_specific_algorithm() {
  this->init_mesh_members();
  // loop over all faces and compute face points
  this->compute_all_face_points();
  // loop over all edges and compute edge points
  this->compute_all_edge_points();
  // update all vertices
  this->compute_all_new_vertex_points();
  // create faces in new mesh
  Surface_mesh::Face_iterator fit;
  for (fit = input_mesh_->faces_begin(); fit != input_mesh_->faces_end(); ++fit) {
    this->compute_new_faces(*fit);
  }
  this->deinit_mesh_members();
}

void SubdivCatmull::init_mesh_members() {
  SubdivAlgorithm::init_mesh_members();
  input_mesh_->add_vertex_property<Surface_mesh::Vertex>
  (kPropVertexIndexResultV);
  input_mesh_->add_edge_property<Surface_mesh::Vertex>
  (kPropVertexIndexResultE);
  input_mesh_->add_face_property<Surface_mesh::Vertex>
  (kPropVertexIndexResultF);
  v_index_result_f_prop_ = input_mesh_->get_face_property<Surface_mesh::Vertex>
                             (kPropVertexIndexResultF);
  v_index_result_e_prop_ = input_mesh_->get_edge_property<Surface_mesh::Vertex>
                             (kPropVertexIndexResultE);
  v_index_result_v_prop_ =
    input_mesh_->get_vertex_property<Surface_mesh::Vertex>(kPropVertexIndexResultV);
}

void SubdivCatmull::deinit_mesh_members() {
  SubdivAlgorithm::deinit_mesh_members();
  input_mesh_->remove_face_property(v_index_result_f_prop_);
  input_mesh_->remove_edge_property(v_index_result_e_prop_);
  input_mesh_->remove_vertex_property(v_index_result_v_prop_);
}

void SubdivCatmull::compute_all_face_points() {
  Surface_mesh::Face_iterator fit;
  Point face_point;
  for (fit = input_mesh_->faces_begin(); fit != input_mesh_->faces_end(); ++fit) {
    this->compute_face_point(face_point, *fit);
    // store face point as property
    f_points_[*fit] = face_point;
    v_index_result_f_prop_[*fit] = result_mesh_->add_vertex(f_points_[*fit]);
    DEBUG_POINT(face_point, "Face Point")
  }
}

void SubdivCatmull::compute_all_edge_points() {
  Surface_mesh::Edge_iterator eit;
  Point edge_point;
  for (eit = input_mesh_->edges_begin(); eit != input_mesh_->edges_end(); ++eit) {
    this->compute_edge_point(edge_point, *eit);
    // store edge_point as property
    e_points_[*eit] = edge_point;
    v_index_result_e_prop_[*eit] = result_mesh_->add_vertex(e_points_[*eit]);
    DEBUG_POINT(edge_point, "Edge Point");
  }
}

void SubdivCatmull::compute_all_new_vertex_points() {
  Surface_mesh::Vertex_iterator vit;
  Point new_vertex_point;
  for (vit = input_mesh_->vertices_begin(); vit != input_mesh_->vertices_end();
       ++vit) {
    this->compute_new_vertex_point(new_vertex_point, *vit);
    // store result in kSurfMeshPropVertexPointUpdated
    v_points_updated_[*vit] = new_vertex_point;
    v_index_result_v_prop_[*vit] = result_mesh_->add_vertex(
                                       v_points_updated_[*vit]);
    DEBUG_POINT(new_vertex_point, "New Vertex Point");
  }
}

void SubdivCatmull::compute_edge_point(Point& edge_point,
                                       const Surface_mesh::Edge& edge) {
  // init result with zero
  edge_point = Point(0);
  // get properties
  // get all coordinates
  Surface_mesh::Vertex edge_vertex0, edge_vertex1;
  Surface_mesh::Face edge_face0, edge_face1;
  edge_vertex0 = input_mesh_->vertex(edge, 0);
  edge_vertex1 = input_mesh_->vertex(edge, 1);
  edge_face0 = input_mesh_->face(edge, 0);
  edge_face1 = input_mesh_->face(edge, 1);
  // compute edge point
  edge_point = v_points_[edge_vertex0] + v_points_[edge_vertex1] +
               f_points_[edge_face0] + f_points_[edge_face1];
  edge_point /= 4;
}

void SubdivCatmull::compute_new_vertex_point(Point& new_vertex_point,
    const Surface_mesh::Vertex& vertex) {
  // compute new vertex point: (Q/n) + (2R/n) + (S(n-3)/n)
  Point q, r, s;
  unsigned int vertex_valence = input_mesh_->valence(vertex);
  // average of the surrounding face points
  this->avg_face_points(q, vertex);
  // average of all surround edge midpoints
  this->avg_mid_edges(r, vertex);
  // old control point
  s = v_points_[vertex];
  // compute new vertex point
  new_vertex_point = (q / vertex_valence) + (2 * r / vertex_valence) + (s *
                     (vertex_valence - 3) / vertex_valence);
}

void SubdivCatmull::compute_new_faces(const Surface_mesh::Face& face) {
  const int kArraySize = 4;
  Surface_mesh::Vertex v_index_list[kArraySize];
  Surface_mesh::Vertex e_index_list[kArraySize];
  int i = 0;
  Surface_mesh::Halfedge_around_face_circulator hc = input_mesh_->halfedges(face);
  for (const Surface_mesh::Halfedge& h : hc) {
    if (i < kArraySize) { // check if in array bounds
      v_index_list[i] = v_index_result_v_prop_[input_mesh_->from_vertex(h)];
      e_index_list[i] = v_index_result_e_prop_[input_mesh_->edge(h)];
    }
    ++i; // increment to compute face valence
  }
  const Surface_mesh::Vertex f_index = v_index_result_f_prop_[face];
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
    throw new std::runtime_error("Invalid mesh topology: " + i);
  }
  // TODO
}

void SubdivCatmull::avg_face_points(Point& avg_face_points,
                                    const Surface_mesh::Vertex& vertex) {
  avg_face_points = Point(0);
  int i = 0;
  Surface_mesh::Face_around_vertex_circulator fc = input_mesh_->faces(vertex);
  for (const Surface_mesh::Face& f : fc) {
    avg_face_points += f_points_[f];
    ++i;
  }
  if (i != 0) {
    avg_face_points /= i;
  }
}

void SubdivCatmull::avg_mid_edges(Point& avg_mid_edges,
                                  const Surface_mesh::Vertex& vertex) {
  avg_mid_edges = Point(0);
  int i = 0;
  Point mid_edge;
  Surface_mesh::Halfedge_around_vertex_circulator hc = input_mesh_->halfedges(
        vertex);
  for (const Surface_mesh::Halfedge& h : hc) {
    this->compute_mid_edge(mid_edge, input_mesh_->edge(h));
    avg_mid_edges += mid_edge;
    ++i;
  }
  if (i != 0) {
    avg_mid_edges /= i;
  }
}

} // namespace subdivision
