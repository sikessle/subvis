#include "plugins/subdivision/debug.h"
#include "plugins/subdivision/types.h"

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

  this->remove_mesh_properties();
}

void SubdivDooSabin::add_mesh_properties() {
  // add properties that are necessary for doo sabin
  input_mesh_->add_face_property<Point>(kSurfMeshPropFacePoint);
  input_mesh_->add_edge_property<Point>(kSurfMeshPropEdgePoint);
}

void SubdivDooSabin::init_mesh_members() {
  SubdivAlgorithm::init_mesh_members();
  f_points_ = input_mesh_->get_face_property<Point>(kSurfMeshPropFacePoint);
  e_points_ = input_mesh_->get_edge_property<Point>(kSurfMeshPropEdgePoint);
}

void SubdivDooSabin::remove_mesh_properties() {
  input_mesh_->remove_face_property(f_points_);
  input_mesh_->remove_edge_property(e_points_);
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
    vc_end = vc;
    do {
      this->compute_new_vertex_point(new_vertex_point, *vc, *fit);
      // TODO

      result_mesh_->add_vertex(new_vertex_point);
      DEBUG_POINT(new_vertex_point, "New Vertex Point");
    } while (++vc != vc_end);
  }
}

void SubdivDooSabin::compute_new_vertex_point(Point& new_vertex_point,
    const Surface_mesh::Vertex& vertex, const Surface_mesh::Face& face) {
  // compute new vertex point: average of four points (face point, two edge points and vertex)
  Surface_mesh::Halfedge_around_face_circulator hc, hc_end;
  hc = input_mesh_->halfedges(face);
  hc_end = hc;
  do {
    if (input_mesh_->from_vertex(*hc) == vertex) {
      break;
    }
  } while (++hc != hc_end);
  Surface_mesh::Edge e1, e2;
  e1 = input_mesh_->edge(*hc);
  e2 = input_mesh_->edge(input_mesh_->prev_halfedge(*hc));
  new_vertex_point = (v_points_[vertex] + f_points_[face] + e_points_[e1] +
                      e_points_[e2] ) / 4;
}

} // namespace subdivision
