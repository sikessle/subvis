/**
 *
 * @brief Base class for all quad subdivision algorithms
 *
 *
 */


#ifndef SUBVIS_PLUGINS_SUBDIVISION_SD_QUAD_H
#define SUBVIS_PLUGINS_SUBDIVISION_SD_QUAD_H

#include "plugins/subdivision/sd_algorithm.h"

namespace subdivision {


class SdQuad : public SdAlgorithm {
 protected:
  void subdivide_input_mesh_write_output_mesh() override = 0;
  void init_mesh_members() override;
  void deinit_mesh_members() override;

  /**
   * @brief kPropVertexPointUpdated key to access surface mesh property that stores the updated vertex coordinates
   */
  const ::std::string kPropVertexPointUpdated = "v:point_updated";

  /**
   * @brief kPropFacePoint key to access surface mesh property that stores the face point coordinates
   */
  const ::std::string kPropFacePoint = "f:point";

  /**
   * @brief kPropEdgePoint key to access surface mesh property that stores the edge point coordinates
   */
  const ::std::string kPropEdgePoint = "e:point";


  Surface_mesh::Edge_property<Point> e_points_;
  Surface_mesh::Face_property<Point> f_points_;
  Surface_mesh::Vertex_property<Point> v_points_updated_;

  /**
   * @brief compute_mid_edge Compute the mid point of an edge (average of the two vertices).
   *                 The edge has to be in the input_mesh_.
   * @param mid_edge The result (mid of edge).
   * @param edge Edge to compute the mid point.
   */
  void compute_mid_edge(Point& mid_edge, const Surface_mesh::Edge& edge);

  /**
   * @brief compute_face_point Compute the average of all the points of the face.
   */
  void compute_face_point(Point& face_point, const Surface_mesh::Face& face);
};


} // namespace subdivision

#endif // SUBVIS_PLUGINS_SUBDIVISION_SD_QUAD_H


