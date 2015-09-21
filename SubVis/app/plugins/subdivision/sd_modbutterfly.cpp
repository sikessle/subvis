

#include <cmath>
#include "plugins/subdivision/debug.h"

#include "plugins/subdivision/sd_modbutterfly.h"


namespace subdivision {

void SdModButterfly::compute_edge_point(Point& edge_point,
                                        const Surface_mesh::Edge& edge) {
  const unsigned int kOrdinaryValence = 6;
  const unsigned int kValence0 = input_mesh_->valence(input_mesh_->vertex(edge,
                                 0));
  const unsigned int kValence1 = input_mesh_->valence(input_mesh_->vertex(edge,
                                 1));
  if (kValence0 == kOrdinaryValence && kValence1 == kOrdinaryValence) {
    this->compute_edge_point_ordinary(edge_point, edge);
  } else if (kValence0 == kOrdinaryValence || kValence1 == kOrdinaryValence) {
    const Surface_mesh::Halfedge halfedge = kValence0 == kOrdinaryValence ?
                                            input_mesh_->halfedge(edge, 0) : input_mesh_->halfedge(edge, 1);
    this->compute_edge_point_one_extraordinary(edge_point, halfedge);
  } else {
    this->compute_edge_point_two_extraordinary(edge_point, edge);
  }
}

void SdModButterfly::compute_edge_point_one_extraordinary(Point& edge_point,
    const Surface_mesh::Halfedge& halfedge) {
  edge_point = (3. / 4. * v_points_[input_mesh_->from_vertex(halfedge)]);
  const unsigned int kValence = input_mesh_->valence(input_mesh_->from_vertex(
                                  halfedge));
  if (kValence == 3) {
    const Surface_mesh::Halfedge h1 = input_mesh_->ccw_rotated_halfedge(halfedge);
    const Surface_mesh::Halfedge h2 = input_mesh_->cw_rotated_halfedge(halfedge);
    edge_point += (5. / 12. * v_points_[input_mesh_->to_vertex(halfedge)]
                   - 1. / 12. * (v_points_[input_mesh_->to_vertex(h1)] +
                                 v_points_[input_mesh_->to_vertex(h2)]));
  } else if (kValence == 4) {
    const Surface_mesh::Halfedge h2 = input_mesh_->cw_rotated_halfedge(
                                        input_mesh_->cw_rotated_halfedge(halfedge));
    edge_point += (3. / 8. * v_points_[input_mesh_->to_vertex(
                                         halfedge)] - 1. / 8. * v_points_[input_mesh_->to_vertex(h2)]);
  } else if (kValence >= 5) {
    Surface_mesh::Halfedge rotated_halfedge = halfedge;
    unsigned int j = 0;
    // rotate counter-clockwise around start vertex
    do {
      const double s = (1. / 4. + std::cos(2. * M_PI * j / kValence) + 1. / 2. *
                        std::cos(4. * M_PI * j / kValence)) / kValence;
      edge_point += (s * v_points_[input_mesh_->to_vertex(rotated_halfedge)]);
      ++j;
      rotated_halfedge = input_mesh_->ccw_rotated_halfedge(rotated_halfedge);
    } while (rotated_halfedge != halfedge);
  } else { // error
    ;
  }
}

void SdModButterfly::compute_edge_point_two_extraordinary(Point& edge_point,
    const Surface_mesh::Edge& edge) {
  Point point0, point1;
  this->compute_edge_point_one_extraordinary(point0, input_mesh_->halfedge(edge,
      0));
  this->compute_edge_point_one_extraordinary(point1, input_mesh_->halfedge(edge,
      1));
  edge_point = (point0 + point1) / 2.;
}

} // namespace subdivision
